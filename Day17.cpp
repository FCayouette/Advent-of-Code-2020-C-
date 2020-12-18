#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

constexpr int Pow(int base, int exp) { int result = 1; for (int i = 0; i < exp; ++i) result *= base; return result; }

template <int N, typename T>
void Iterate(std::vector<std::array<short, N>>& active, std::vector<std::array<short, N>>& tmp, std::vector<std::array<short, N>>& old, T&& Consider)
{
    constexpr int factor = (Pow(3, N) - 1);
    tmp.reserve(active.size() * factor);
    old.clear();
    for (const std::array<short, N>& p : active) Consider(p, tmp);
    std::sort(tmp.begin(), tmp.end());
    size_t l = 0, h;
    while (l < tmp.size())
    {
        h = l + 1;
        while (h < tmp.size() && tmp[l] == tmp[h]) ++h;
        if (size_t diff = h - l;
            diff == 3 || diff == 2 && std::binary_search(active.cbegin(), active.cend(), tmp[l]))
            old.push_back(tmp[l]);
        l = h;
    }
    std::swap(active, old);
    tmp.clear();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day17.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    using P3 = std::array<short, 3>;
    using P4 = std::array<short, 4>;
    std::vector<P3> activeCubes, newCubes, toConsider;
    std::vector<P4> activeHyper, newHypers, consideredHyper;

    short y = 0;
    std::string line;
    while (in >> line)
    {
        for (short x = 0; x < line.size(); ++x)
            if (line[x] == '#')
            {
                activeCubes.push_back({ y, x, 0 });
                activeHyper.push_back({ y, x, 0, 0 });
            }
        ++y;
    }

    for (int i = 0; i < 6; ++i)
    {
        Iterate(activeCubes, toConsider, newCubes, [](const P3& p, std::vector<P3>& toConsider) {
                for (short x = p[0] - 1; x <= p[0] + 1; ++x)
                    for (short y = p[1] - 1; y <= p[1] + 1; ++y)
                        for (short z = p[2] - 1; z <= p[2] + 1; ++z)
                            if (x != p[0] || y != p[1] || z != p[2])
                                toConsider.push_back({ x, y, z }); });
        Iterate(activeHyper, consideredHyper, newHypers, 
            [](const P4& p, std::vector<P4>& toConsider) {
                for (short x = p[0] - 1; x <= p[0] + 1; ++x)
                    for (short y = p[1] - 1; y <= p[1] + 1; ++y)
                        for (short z = p[2] - 1; z <= p[2] + 1; ++z)
                            for (short w = p[3] - 1; w <= p[3] + 1; ++w)
                                if (x != p[0] || y != p[1] || z != p[2] || w != p[3])
                                    toConsider.push_back({ x, y, z, w }); });
    }

    std::cout << "Part 1: " << activeCubes.size() << std::endl << "Part 2: " << activeHyper.size() << std::endl;
    return 0;
}