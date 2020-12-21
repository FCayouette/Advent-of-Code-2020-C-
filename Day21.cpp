#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day21.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::map<std::string, std::vector<std::string>> allPos;
    std::map<std::string, size_t> ingCounts;
    std::string data, part2;
    bool allergens = false;

    std::vector<std::string> lineIng, lineAllergens;
    size_t part1 = 0;
    while (in >> data)
    {
        if (data[0] == '(')
            allergens = true;
        else if (!allergens)
        {
            lineIng.push_back(data);
            ++ingCounts[data];
            ++part1;
        }
        else
        {
            lineAllergens.push_back(data.substr(0, data.size() - 1));
            allergens = data[data.size() - 1] == ',';
            if (!allergens)
            {
                std::sort(lineIng.begin(), lineIng.end());
                for (const std::string& a : lineAllergens)
                {
                    if (allPos.find(a) == allPos.cend())
                        allPos[a] = lineIng;
                    else
                    {
                        std::vector<std::string>& curPos = allPos[a];
                        std::vector<std::string> newPos;
                        std::set_intersection(lineIng.cbegin(), lineIng.cend(), curPos.cbegin(), curPos.cend(), std::back_inserter(newPos));
                        allPos[a] = newPos;
                    }
                }
                lineIng.clear();
                lineAllergens.clear();
            }
        }
    }
    std::vector<std::pair<std::string, std::string>> matches;

    while (allPos.size())
    {
        std::map<std::string, std::vector<std::string>>::iterator iter = allPos.begin();
        while (iter->second.size() != 1)
            ++iter;
        std::string toDelete = iter->second.front();
        part1 -= ingCounts[toDelete];
        matches.emplace_back(iter->first, toDelete);
        allPos.erase(iter);
        for (auto& x : allPos)
            if (auto extra = std::find(x.second.begin(), x.second.end(), toDelete);
                extra != x.second.end())
                x.second.erase(extra);
    }    
    std::cout << "Part 1: " << part1 << std::endl;

    std::sort(matches.begin(), matches.end());
    for (size_t i = 0; i < matches.size() - 1; ++i)
        part2 += matches[i].second + ',';
    part2 += matches.back().second;
    std::cout << part2 << std::endl;
    return 0;
}