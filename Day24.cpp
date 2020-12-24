#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include "TemplatedUtilities.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day24.exe Datafilename" << std::endl;
        return -1;
    }
    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;
    std::array<Point, 6> deltas{ Point(-1,0), Point(0,-1), Point(1,-1), Point(1, 0), Point(0, 1), Point(-1,1) };//e ne nw w sw se

    std::map<Point, size_t> floor;

    std::string line;
    while (in >> line)
    {
        size_t tokenLength = 0;
        Point tile(0, 0);
        do
        {
            if (char c = line[0];
                c == 'e' || c == 'w')
            {
                tile += deltas[c =='e' ? 0 : 3];
                tokenLength = 1;
            }
            else
            {
                char c2 = line[1];
                if (c == 'n')
                    tile += deltas[c2 == 'e' ? 1 : 2];
                else
                    tile += deltas[c2 == 'e' ? 5 : 4];
                tokenLength = 2;
            }

            line = line.substr(tokenLength);
        } while (!line.empty());
        ++floor[tile];
    }

    std::set<Point> blackTiles, work;
    for (const auto& t : floor)
        if (t.second % 2)
            blackTiles.insert(t.first);
    std::cout << "Part 1: " << blackTiles.size() << std::endl;

    std::vector<Point> adjacencies;

    for (size_t turns = 0; turns < 100; ++turns)
    {
        adjacencies.reserve(6 * blackTiles.size());
        for (const Point& p : blackTiles)
            std::for_each(deltas.cbegin(), deltas.cend(), [&adjacencies, &p](const Point& d) { adjacencies.push_back(p + d); });

        std::sort(adjacencies.begin(), adjacencies.end());

        for (const Point& p : blackTiles)
            if (auto iter = std::find(adjacencies.cbegin(), adjacencies.cend(), p);
                iter != adjacencies.cend())
                if (auto iter2 = std::find_if(iter, adjacencies.cend(), [&p](const Point a) { return a != p; }); 
                    std::distance(iter, iter2) < 3)
                    work.insert(p);
            
        for (auto iter = adjacencies.cbegin(); iter != adjacencies.cend();)
        {
            auto iter2 = std::find_if(iter, adjacencies.cend(), [p3d = *iter](const Point& p) { return p != p3d; }); 
            if (std::distance(iter, iter2) == 2 && blackTiles.find(*iter) == blackTiles.cend())
                work.insert(*iter);
            iter = iter2;
        }

        std::swap(blackTiles, work);
        work.clear();
        adjacencies.clear();
    }

    std::cout << "Part 2: " << blackTiles.size() << std::endl;

    return 0;
}