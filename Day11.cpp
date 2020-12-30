#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <numeric>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day11.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::vector<std::string> layout, work, backup;
    std::string line;
    while (in >> line)
        layout.emplace_back(line);

    backup = work = layout;
    bool change = false;
    int maxX = layout.size(), maxY = layout.front().size();

    auto CountAdjacent = [&layout, maxX, maxY](int x, int y) ->int
    {
        int result = 0;
        for (int i = std::max(0, x - 1); i < std::min(maxX, x + 2); ++i)
            for (int j = std::max(0, y - 1); j < std::min(maxY, y + 2); ++j)
                if ((i != x || j != y) && layout[i][j] == '#')
                    ++result;
        return result;
    };

    do
    {
        change = false;
        for (int x = 0; x < maxX; ++x)
            for (int y = 0; y < maxY; ++y)
                if (layout[x][y] != '.')
                {
                    int count = CountAdjacent(x, y);
                    if (layout[x][y] == 'L')
                    {
                        if (!count)
                        {
                            work[x][y] = '#';
                            change = true;
                        }
                        else work[x][y] = 'L';
                    }
                    else
                    {
                        if (count >= 4)
                        {
                            work[x][y] = 'L';
                            change = true;
                        }
                        else work[x][y] = '#';
                    }
                }
        std::swap(layout, work);
    } while (change);

    auto Count = [&layout]() { return std::accumulate(layout.cbegin(), layout.cend(), 0, [](size_t x, const std::string& s) { return x + std::count(s.cbegin(), s.cend(), '#'); }); };

    std::cout << "Part 1: " << Count() << std::endl;
    
    auto CountVisible = [&layout, maxX, maxY](int x, int y) -> int
    {
        auto inside = [maxX, maxY](int x, int y) -> bool
        {
            if (x < 0 || y < 0 || x >= maxX || y >= maxY)
                return false;
            return true;
        };

        int result = 0;
        constexpr std::array<std::pair<int, int>, 8> dir = 
        { std::make_pair(-1, -1), std::make_pair(0, -1), std::make_pair(1, -1), std::make_pair(1, 0), std::make_pair(1, 1), 
            std::make_pair(0, 1), std::make_pair(-1, 1), std::make_pair(-1, 0) };

        for (const auto d : dir)
        {
            int nx = x + d.first, ny = y + d.second;
            while (inside(nx, ny))
            {
                if (layout[nx][ny] != '.')
                {
                    if (layout[nx][ny] == '#')
                        ++result;
                    break;
                }
                nx += d.first;
                ny += d.second;
            }
        }
        return result;
    };

    std::swap(layout, backup);
    do
    {
        change = false;
        for (int x = 0; x < maxX; ++x)
            for (int y = 0; y < maxY; ++y)
                if (layout[x][y] != '.')
                {
                    int count = CountVisible(x, y);
                    if (layout[x][y] == 'L')
                    {
                        if (!count)
                        {
                            work[x][y] = '#';
                            change = true;
                        }
                        else work[x][y] = 'L';
                    }
                    else
                    {
                        if (count >= 5)
                        {
                            work[x][y] = 'L';
                            change = true;
                        }
                        else work[x][y] = '#';
                    }
                }
        std::swap(layout, work);
    } while (change);

    std::cout << "Part 2: " << Count() << std::endl;
    return 0;
}
