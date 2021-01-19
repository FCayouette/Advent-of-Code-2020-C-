#include <iostream>
#include <fstream>
#include <string_view>
#include <array>
#include <algorithm>

using i64 = long long;

i64 EvalParentheses(std::string_view& str, i64(*evaluator)(std::string_view))
{
    size_t count = 1, index = 0;
    while (count && ++index)
        switch (str[index])
        {
        case '(': ++count; break;
        case ')': --count;
        }
    
    i64 result = evaluator(str.substr(1, index - 1));
    str = str.substr(index);
    return result;
}

i64 Evaluate1(std::string_view str)
{
    i64 result = 0ll;
    bool isAdd = true;
    while (true)
    {
        i64 val = str[0] == '(' ? EvalParentheses(str, Evaluate1) : str[0] - '0';
        result = isAdd ? result + val : result * val;
        if (str.size() < 4)
            return result;
        isAdd = str[2] == '+';
        str = str.substr(4);
    }
}

i64 Evaluate2(std::string_view str)
{
    i64 result = 0ll;
    while (true)
    {
        result += str[0] == '(' ? EvalParentheses(str, Evaluate2) : str[0] - '0';
        if (str.size() < 4)
            return result;
        if (str[2] == '*')
            return result * Evaluate2(str.substr(4));
        str = str.substr(4);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day18.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    i64 part1 = 0, part2 = 0;
    std::array<char, 256> buffer;
    while (in.getline(&buffer[0], 256))
    {
        std::string_view line(&buffer[0]);
        part1 += Evaluate1(line);
        part2 += Evaluate2(line);
    }
    std::cout << "Part 1: " << part1 << std::endl << "Part 2: " << part2 << std::endl;
    return 0;
}