#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <numeric>

using Borders = std::array<size_t, 4>;
using u64 = unsigned long long;

class Tile
{
public:
    Tile() = default;
    Tile(size_t n, std::vector<std::string>&& values) 
        : num(n)
        , tile(std::move(values))
    {
        ComputeBorders();
    }
    Tile& operator=(const Tile& t) = default;

    size_t GetTileNumber() const { return num; }
    const Borders& GetBorders() const { return borderValues; }
    const Borders& GetReversedBorders() const { return reversedValues; }
    const std::string& GetLine(size_t l) const { return tile[l]; }

    void FlipHorizontal()
    {
        std::reverse(tile.begin(), tile.end());
        ComputeBorders();
    }

    void FlipVertical()
    {
        for (std::string& s : tile)
            std::reverse(s.begin(), s.end());
        ComputeBorders();
    }

    void Rotate()
    {
        std::vector<std::string> newTile;
        for (size_t i = 0; i < 10; ++i)
        {
            std::string s = tile[0];
            for (int j = 9; j >= 0; --j)
                s[9 - j] = tile[j][i];
            newTile.emplace_back(std::move(s));
        }
        std::swap(tile, newTile);
        ComputeBorders();
    }
    void ComputeBorders()
    {
        size_t up = 0, down = 0, left = 0, right = 0;
        for (size_t i = 0; i < 10; ++i)
        {
            up = up * 2 + (tile[0][i] == '#');
            down = down * 2 + (tile[9][i] == '#');
            left = left * 2 + (tile[i][0] == '#');
            right = right * 2 + (tile[i][9] == '#');
        }
        borderValues = { up, right, down, left };

        right = left = down = up = 0;
        for (int i = 9; i >= 0; --i)
        {
            up = up * 2 + (tile[0][i] == '#');
            down = down * 2 + (tile[9][i] == '#');
            left = left * 2 + (tile[i][0] == '#');
            right = right * 2 + (tile[i][9] == '#');
        }
        reversedValues = { up, right, down, left };
    }

private:
    size_t num;
    std::vector<std::string> tile;
    Borders borderValues;
    Borders reversedValues;
};


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day20.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::vector<Tile> tiles;
    std::string line;
    while (in >> line)
    {
        in >> line;
        size_t tileNum = stoi(line);
        std::vector<std::string> tile;
        tile.reserve(10);
        for (size_t i = 0; i < 10; ++i)
        {
            in >> line;
            tile.push_back(line);
        }
        tiles.emplace_back(tileNum, std::move(tile));
    }
    std::map<size_t, std::vector<size_t>> matchingSides;
    
    for (Tile& t : tiles)
    {
        size_t num = t.GetTileNumber();
        for (size_t v : t.GetBorders())
            matchingSides[v].push_back(num);
        for (size_t v : t.GetReversedBorders())
            matchingSides[v].push_back(num);
    }

    std::map<size_t, size_t> aloneCounts;
    for (const auto& ms : matchingSides)
        if (ms.second.size() == 1)
            aloneCounts[ms.second.front()]++;
    
    u64 part1 = 1ull;
    size_t upperLeftIndex = 0;
    for (const auto& ac : aloneCounts)
        if (ac.second == 4)
        {
            part1 *= ac.first;
            if (!upperLeftIndex) upperLeftIndex = ac.first;
        }
    std::cout << "Part 1: " << part1 << std::endl;

    // Actual retiling
    const size_t side = static_cast<size_t>(std::sqrtf((float)tiles.size()));
    std::vector<std::vector<Tile>> retile;
    retile.resize(side);
    for (size_t i = 0; i < side; ++i)
        retile[i].resize(side);

    auto GetNextTile = [&matchingSides, &retile, &tiles](size_t x, size_t y, size_t dir)
    {
        const std::vector<size_t>& t = matchingSides[retile[x][y].GetBorders()[dir]];
        size_t index = t[0] == retile[x][y].GetTileNumber() ? t[1] : t[0];
        return std::find_if(tiles.begin(), tiles.end(), [index](const Tile& t) { return t.GetTileNumber() == index; });
    };

    {
        auto first = std::find_if(tiles.begin(), tiles.end(), [upperLeftIndex](const Tile& t) { return t.GetTileNumber() == upperLeftIndex; });
        if (matchingSides[first->GetBorders()[0]].size() != 1)
            first->FlipHorizontal();
        if (matchingSides[first->GetBorders()[3]].size() != 1)
            first->FlipVertical();
        retile[0][0] = *first;
        for (size_t i = 1; i < side-1; ++i)
        {
            auto current = GetNextTile(0, i - 1, 1);
            while (matchingSides[current->GetBorders()[0]].size() != 1)
                current->Rotate();
            if (current->GetBorders()[3] != retile[0][i - 1].GetBorders()[1])
                current->FlipVertical();
            retile[0][i] = *current;
        }
        auto last = GetNextTile(0, side - 2, 1);
        size_t mustMatch = retile[0][side - 2].GetBorders()[1];
        while (last->GetBorders()[3] != mustMatch && last->GetReversedBorders()[3] != mustMatch)
            last->Rotate();
        if (last->GetBorders()[3] != mustMatch)
            last->FlipHorizontal();
        retile[0][side - 1] = *last;
    }
    for (size_t s = 1; s < side; ++s)
        for (size_t i = 0; i < side; ++i)
        {
            auto current = GetNextTile(s - 1, i, 2);
            size_t mustMatch = retile[s - 1][i].GetBorders()[2];
            while (current->GetBorders()[0] != mustMatch && current->GetReversedBorders()[0] != mustMatch)
                current->Rotate();
            if (current->GetBorders()[0] != mustMatch)
                current->FlipVertical();
            retile[s][i] = *current;
        }

    std::vector<std::string> image;
    auto RotateImage = [&image, dim = side * 8]()
    {
        std::vector<std::string> newImage;
        for (size_t i = 0; i < dim; ++i)
        {
            std::string s = image[0];
            for (int j = dim - 1; j >= 0; --j)
                s[dim - 1 - j] = image[j][i];
            newImage.emplace_back(std::move(s));
        }
        std::swap(image, newImage);
    };

    const std::string line0("                  # ");
    const std::string line1("#    ##    ##    ###");
    const std::string line2(" #  #  #  #  #  #   ");
    const size_t monster0X = line0.size() - 2;
    const size_t monsterSize = 1 + std::count(line1.cbegin(), line1.cend(), '#') + std::count(line2.cbegin(), line2.cend(), '#');


    for (size_t x = 0; x < side; ++x)
        for (size_t i = 1; i < 9; ++i)
        {
            std::string l;
            for (size_t y = 0; y < side; ++y)
                l += retile[x][y].GetLine(i).substr(1, 8);
            image.emplace_back(std::move(l));
        }

    size_t monsterCount = 0;
    for (size_t flips = 0; !monsterCount && flips < 4; ++flips)
    {
        if (flips & 0x1) std::reverse(image.begin(), image.end());
        if (flips & 0x2) for (auto& s : image) std::reverse(s.begin(), s.end());
        for (int i = 0; !monsterCount && i < 4; ++i)
        {
            for (size_t y = 0; y < image.size() - 2; ++y)
                for (size_t x = 0; x < image.size() - line0.size(); ++x)
                    if (image[y][x + monster0X] == '#')
                    {
                        bool valid = true;
                        for (size_t j = 0; valid && j < line1.size(); ++j)
                            valid = line1[j] == ' ' || image[y + 1][x + j] == '#';
                        if (valid)
                            for (size_t j = 0; valid && j < line2.size(); ++j)
                                valid = line2[j] == ' ' || image[y + 2][x + j] == '#';
                        if (valid)
                            ++monsterCount;
                    }
            RotateImage();
        }
        if (flips & 0x1) std::reverse(image.begin(), image.end());
        if (flips & 0x2) for (auto& s : image) std::reverse(s.begin(), s.end());
    }

    size_t part2 = std::accumulate(image.cbegin(), image.cend(), 0u, [](size_t total, const std::string& s) {return total += std::count(s.cbegin(), s.cend(), '#'); });
    part2 -= monsterCount * monsterSize;
    std::cout << "Part 2: " << part2 << std::endl;
    return 0;

}