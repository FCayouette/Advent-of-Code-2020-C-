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

    std::string line;
    int low, high, part1 = 0, part2 = 0;
    while (in >> line)
    {
        sscanf_s(line.c_str(), "%d-%d", &low, &high);
        in >> line;
        char letter = line[0];
        in >> line;
        if (auto val = std::count(line.cbegin(), line.cend(), letter); 
            val >= low && val <= high)
            ++part1;
        if ((line[low - 1] == letter) + (line[high - 1] == letter) == 1)
            ++part2;
    }

    std::cout << "Part1: " << part1 << std::endl << "Part 2: " << part2 << std::endl;;

    return 0;
}
