#include <iostream>
#include <fstream>
#include <string>
#include "TemplatedUtilities.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day12.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    Point position, origin;
    char dir = 1; // East
    std::vector<std::pair<char, int>> instructions;

    std::string instruction;
    while (in >> instruction)
    {
        char c = instruction[0];
        int val = stoi(instruction.substr(1));
        instructions.emplace_back(c, val);
        switch (c)
        {
        case 'L':
            do dir = dir == 0 ? 3 : dir - 1;
            while ((val -= 90) > 0);
            break;
        case 'R':
            do dir = (dir + 1) % coords.size();
            while ((val -= 90) > 0);
            break;
        default: position += directions[(c != 'F') ? std::distance(coords.cbegin(), std::find(coords.cbegin(), coords.cend(), c)) : dir] * val;
        }
    }

    std::cout << "Part 1: " << ManhattanDistance(position, origin) << std::endl;

    position = origin;
    Point waypoint(1, 10);

    for (std::pair<char, int> inst : instructions)
        switch (inst.first)
        {
        case 'L':
            do waypoint = Point(waypoint.y, -waypoint.x);
            while ((inst.second -= 90) > 0);
            break;
        case 'R':
            do waypoint = Point(-waypoint.y, waypoint.x);
            while ((inst.second -= 90) > 0);
            break;
        case 'F': position += waypoint * inst.second; break;
        default: waypoint += directions[std::distance(coords.cbegin(), std::find(coords.cbegin(), coords.cend(), inst.first))] * inst.second;
        }

    std::cout << "Part 2: " << ManhattanDistance(position, origin) << std::endl;

    return 0;
}