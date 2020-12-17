#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "TemplatedUtilities.h"

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day17.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::set<Point3D> cubes, work;
    std::set<Point4D> cubes4, work4;
    std::string line;

    int x = 0, y = 0;
    int minX = 0, minY = 0, minZ = 0, minW = 0, maxX = 0, maxY = 0, maxZ = 0, maxW = 0;
    while (in >> line)
    {
        for (int x = 0; x < line.size(); ++x)
            if (line[x] == '#')
            {
                cubes.insert(Point3D(x, y, 0));
                cubes4.insert(Point4D(x, y, 0, 0));
                GrowToEncompass(minX, maxX, x);
                GrowToEncompass(minY, maxY, y);
            }
        ++y;
    }

    auto CountActiveAround = [&cubes](int x, int y, int z)
    {
        size_t count = 0;
        for (int i = x - 1; i <= x + 1; ++i)
            for (int j = y - 1; j <= y + 1; ++j)
                for (int k = z - 1; k <= z + 1; ++k)
                    if (!(i == x && j == y && k == z) && cubes.find(Point3D(i, j, k)) != cubes.cend())
                        ++count;
        return count;
    };

    auto Count4D = [&cubes4](int x, int y, int z, int w)
    {
        size_t count = 0;
        for (int i = x - 1; i <= x + 1; ++i)
            for (int j = y - 1; j <= y + 1; ++j)
                for (int k = z - 1; k <= z + 1; ++k)
                    for (int l = w - 1; l <= w + 1; ++l)
                        if (!(i == x && j == y && k == z && l == w) && cubes4.find(Point4D(i, j, k, l)) != cubes4.cend())
                            ++count;
        return count;
    };


    for (int i = 0; i < 6; ++i)
    {
        --minX;
        --minY;
        --minZ;
        --minW;
        ++maxX;
        ++maxY;
        ++maxZ;
        ++maxW;
        for (int x = minX; x <= maxX; ++x)
            for (int y = minY; y <= maxY; ++y)
                for (int z = minZ; z <= maxZ; ++z)
                {
                    for (int w = minW; w <= maxW; ++w)
                    {
                        size_t count4 = Count4D(x, y, z, w);
                        if (cubes4.find(Point4D(x, y, z, w)) == cubes4.cend())
                        {
                            if (count4 == 3)
                                work4.insert(Point4D(x, y, z, w));
                        }
                        else
                        {
                            if (count4 == 2 || count4 == 3)
                                work4.insert(Point4D(x, y, z, w));
                        }
                    }
                    size_t count = CountActiveAround(x, y, z);
                    if (cubes.find(Point3D(x, y, z)) == cubes.cend())
                    {
                        if (count == 3)
                            work.insert(Point3D(x, y, z));
                    }
                    else
                    {
                        if (count == 2 || count == 3)
                            work.insert(Point3D(x, y, z));
                    }

                    
                }

        std::swap(cubes, work);
        work.clear();
        std::swap(cubes4, work4);
        work4.clear();
    }

    std::cout << "Part 1: " << cubes.size() << std::endl;
    std::cout << "Part 2: " << cubes4.size() << std::endl;
    return 0;
}