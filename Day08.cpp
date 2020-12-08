#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <string_view>
#include <array>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day08.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::string data;
    std::vector<std::pair<size_t, int>> instructions;

    constexpr std::array<std::string_view, 3> inst{ "nop", "acc", "jmp" };

    while (in >> data)
    {
        size_t i = std::distance(inst.cbegin(), std::find(inst.cbegin(), inst.cend(), data));
        in >> data;
        int value = atoi(data.c_str());
        instructions.emplace_back(i, value);
    }

    size_t PC = 0;
    int acc = 0;
    std::set<size_t> seenPC;

    auto executeLine = [&seenPC, &instructions, &PC, &acc]()
    {
        if (seenPC.find(PC) != seenPC.cend())
            return false;
        seenPC.insert(PC);
        switch (instructions[PC].first)
        {
        case 1:
            acc += instructions[PC].second;
        case 0:
            ++PC;
            break;
        default:
            PC += instructions[PC].second;
            break;
        }
        return true;
    };

    while (true)
        if (!executeLine())
            break;

    std::cout << "Part 1: " << acc << std::endl;

    auto switchInstruction = [](std::pair<size_t, int>& p) { p.first = (p.first==0 ? 2 : 0); };

    for (size_t i = 0; i < instructions.size(); ++i)
    {
        if (instructions[i].first == 1)
            continue;
        seenPC.clear();
        PC = 0;
        acc = 0;
        
        switchInstruction(instructions[i]);
        
        while (PC != instructions.size())
            if (!executeLine())
                break;
        
        if (PC == instructions.size())
        {
            std::cout << "Part 2: " << acc << std::endl;
            break;
        }
        else
            switchInstruction(instructions[i]);
    }

    return 0;
}
