#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <set>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day16.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::array<char, 256> buffer;
    char state = 0;
    std::vector<std::pair<size_t, size_t>> valueRanges; //inclusive
    std::vector<size_t> ticketValues;
    std::vector<std::vector<size_t>> validTickets;
    size_t rangeCount = 0;

    std::vector<std::set<size_t>> possibilities;

    size_t part1 = 0;
    while (in.getline(&buffer[0], 256))
    {
        if (state == 0)
        {
            if (strlen(&buffer[0]) == 0)
            {
                std::set<size_t> allPossibilities;
                for (size_t i = 0; i < rangeCount; ++i)
                    allPossibilities.insert(i);
                for (size_t i = 0; i < rangeCount; ++i)
                    possibilities.push_back(allPossibilities);
                ++state;
            }
            else
            {
                std::string line = &buffer[0];
                size_t a, b, c, d;
                std::string name = line.substr(0, line.find(':'));
                line = line.substr(line.find(':') + 2);
                sscanf_s(line.c_str(), "%d-%d or %d-%d", &a, &b, &c, &d);
                valueRanges.emplace_back(a, b);
                valueRanges.emplace_back(c, d);
                ++rangeCount;
            }
        }
        else if (state == 1)
            state++;
        else if (state == 2)
        {
            std::string line = &buffer[0];
            while (true)
            {
                size_t pos = line.find(',');
                ticketValues.push_back(stoi(line.substr(0, pos)));
                if (pos == std::string::npos)
                    break;
                line = line.substr(pos + 1);
            }
            ++state;
        }
        else if (state == 3 || state == 4)
            ++state;
        else {
            if (strlen(&buffer[0]) == 0)
                break;
            std::string line = &buffer[0];
            bool validTicket = true;
            std::vector<size_t> potentialTicket;
            while (true)
            {
                size_t pos = line.find(',');
                size_t val = stoi(line.substr(0, pos));
                potentialTicket.push_back(val);
                if (std::none_of(valueRanges.cbegin(), valueRanges.cend(), [&val](const auto& p) { return p.first <= val && val <= p.second; }))
                {
                    part1 += val;
                    validTicket = false;
                }
                if (pos == std::string::npos)
                    break;
                line = line.substr(pos + 1);
            }
            if (validTicket)
                validTickets.emplace_back(std::move(potentialTicket));
        }
    }

    std::cout << "Part 1: " << part1 << std::endl;

    for (const auto& t : validTickets)
    {
        for (size_t i = 0; i < t.size(); ++i)
        {
            size_t val = t[i];
            for (size_t r = 0; r < valueRanges.size(); r += 2)
                if (!((valueRanges[r].first <= val && val <= valueRanges[r].second) || (valueRanges[r + 1].first <= val && val <= valueRanges[r + 1].second)))
                    possibilities[r / 2].erase(i);
        }
    }

    unsigned long long part2 = 1ull;
    for (size_t k = 0; k < possibilities.size(); ++k)
        for (size_t i = 0; i < possibilities.size(); ++i)
            if (possibilities[i].size() == 1)
            {
                size_t val = *possibilities[i].cbegin();
                if (i < 6)
                    part2 *= ticketValues[val];
                for (auto& s : possibilities)
                    s.erase(val);
                break;
            }
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}