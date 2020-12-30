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

    Point part1, part2, origin, waypoint(1, 10);
    char dir = 1; // East

    std::string instruction;
    while (in >> instruction)
    {
        char c = instruction[0];
        int val = stoi(instruction.substr(1));
        switch (c)
        {
        case 'L':
            do {
                dir = (dir + 3) % coords.size();
                waypoint = Point(waypoint.y, -waypoint.x);
            } while ((val -= 90) > 0);
            break;
        case 'R':
            do {
                dir = (dir + 1) % coords.size();
                waypoint = Point(-waypoint.y, waypoint.x);
            } while ((val -= 90) > 0);
            break;
        case 'F':
            part1 += directions[dir] * val;
            part2 += waypoint * val;
            break;
        default:
        {
            Point delta = directions[std::distance(coords.cbegin(), std::find(coords.cbegin(), coords.cend(), c))] * val;
            part1 += delta;
            waypoint += delta;
        }
        }
    }

    std::cout << "Part 1: " << ManhattanDistance(part1, origin) << std::endl << "Part 2: " << ManhattanDistance(part2, origin) << std::endl;
    return 0;
}