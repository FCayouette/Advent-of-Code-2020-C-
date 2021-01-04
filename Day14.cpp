#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <unordered_map>
#include "TemplatedUtilities.h"

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day14.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::unordered_map<u64, u64> memory1, memory2;
    std::string start, equal, val, mask, part2Mask;

    auto WriteAddresses = y_combinator([&memory2, &part2Mask](auto&& WriteAddresses, u64 val, size_t where) {
        while (where < part2Mask.size() && part2Mask[where] != 'X')
            ++where;
        if (where >= part2Mask.size())
        {
            u64 address = std::stoll(part2Mask, nullptr, 2);
            memory2[address] = val;
            return;
        }
        part2Mask[where] = '0';
        WriteAddresses(val, where + 1);
        part2Mask[where] = '1';
        WriteAddresses(val, where);
        part2Mask[where] = 'X';
        });

    while (in >> start >> equal >> val)
    {
        if (start[1] == 'a')
            mask = val;
        else
        {
            int index = stoi(start.substr(4, start.find(']' - 4)));
            u64 value = stoll(val), part2Val = value;
            u64 currentBit = 1;
            for (auto iter = mask.crbegin(); iter != mask.crend(); ++iter, currentBit *= 2ull)
            {
                if (*iter == '1')
                    value |= currentBit;
                else if (*iter == '0')
                    value &= ~currentBit;
            }
            memory1[index] = value;

            // part 2
            part2Mask = mask;
            currentBit = 1ull;
            for (auto iter = part2Mask.rbegin(); iter != part2Mask.rend(); ++iter, currentBit*=2ull)
                if (*iter == '0' && index & currentBit)
                        *iter = '1';
            WriteAddresses(part2Val, 0);
        }
    }

    std::cout << "Part 1: " << std::accumulate(memory1.cbegin(), memory1.cend(), 0ull, [](u64 l, const auto& p) { return l + p.second; }) << std::endl;
    std::cout << "Part 2: " << std::accumulate(memory2.cbegin(), memory2.cend(), 0ull, [](u64 l, const auto& p) { return l + p.second; }) << std::endl;
    return 0;
}