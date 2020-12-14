#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <numeric>
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

    std::map<u64, u64> memory1, memory2;
    std::string start, equal, val, mask;

    auto WriteAddresses = y_combinator([&memory2](auto&& WriteAddresses, u64 val, const std::string& mask, size_t where) {
        while (where < mask.size() && mask[where] != 'X')
            ++where;
        if (where >= mask.size())
        {
            u64 address = std::stoll(mask, nullptr, 2);
            memory2[address] = val;
            return;
        }
        std::string nMask = mask;
        nMask[where] = '0';
        WriteAddresses(val, nMask, where + 1);
        nMask[where++] = '1';
        WriteAddresses(val, nMask, where);
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
            std::string newMask = mask;
            currentBit = 1ull;
            for (auto iter = newMask.rbegin(); iter != newMask.rend(); ++iter, currentBit*=2ull)
                if (*iter == '0' && index & currentBit)
                        *iter = '1';
            WriteAddresses(part2Val, newMask, 0);
        }
    }

    std::cout << "Part 1:" << std::accumulate(memory1.cbegin(), memory1.cend(), 0ull, [](u64 l, const auto& p) { return l + p.second; }) << std::endl;
    std::cout << "Part 2: " << std::accumulate(memory2.cbegin(), memory2.cend(), 0ull, [](u64 l, const auto& p) { return l + p.second; }) << std::endl;
    return 0;
}