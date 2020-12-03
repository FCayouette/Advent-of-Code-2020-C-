#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day02.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    int r1 = 0, r3 = 0, r5 = 0, r7 = 0, r05 = 0, y = 0;
    long long r1d1 = 0, r3d1 = 0, r5d1 = 0, r7d1 = 0, r1d2 = 0;
    std::string row;

    while (in >> row)
    {
        static int maxX = row.size();
    
        if (row[r1] == '#')
            ++r1d1;
        if (row[r3] == '#')
            ++r3d1;
        if (row[r5] == '#')
            ++r5d1;
        if (row[r7] == '#')
            ++r7d1;
        if (!y && row[r05] == '#')
            ++r1d2;

        r1 = (r1 + 1) % maxX;
        r3 = (r3 + 3) % maxX;
        r5 = (r5 + 5) % maxX;
        r7 = (r7 + 7) % maxX;
        if (y++)
        {
            r05 = (r05 + 1) % maxX;
            y = 0;
        }
    }
    std::cout << "Part 1: " << r3d1 << std::endl;
    std::cout << "Part 2: " << r1d1 * r3d1 * r5d1 * r7d1 * r1d2 << std::endl;
    return 0;
}
