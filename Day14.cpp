#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "TemplatedUtilities.h"

using u64 = unsigned long long;


void WriteAtAddresses(std::map<u64, u64>& mem, u64 val, const std::string& mask, size_t where)
{
    while (where < mask.size()  && mask[where] != 'X')
        ++where;
    if (where >= mask.size())
    {
        u64 address = std::stoll(mask, nullptr, 2);
        mem[address] = val;
        return;
    }
    std::string nMask = mask;
    nMask[where] = '0';
    WriteAtAddresses(mem, val, nMask, where + 1);
    nMask[where++] = '1';
    WriteAtAddresses(mem, val, nMask, where);
}


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
            WriteAtAddresses(memory2, part2Val, newMask, 0);
        }
    }

    u64 part1 = 0ull;
    for (const auto& memPoint : memory1)
        part1 += memPoint.second;

    std::cout << "Part 2:" << part1 << std::endl;

    u64 part2 = 0ull;
    for (const auto& memPoint : memory2)
        part2 += memPoint.second;

    std::cout << "Part 2: "<< part2 << std::endl;
    return 0;
}