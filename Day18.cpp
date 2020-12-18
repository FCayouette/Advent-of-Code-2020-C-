#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
using i64 = long long;

i64 Evaluate(std::string str)
{
    i64 result = 0;
    bool first = true, isAdd = false;
    while (str.size() > 0)
    {
        i64 val;
        if (str[0] == '(')
        {
            size_t count = 1, index = 1;
            do
            {
                if (str[index] == '(')
                    ++count;
                else if (str[index] == ')')
                    --count;
                ++index;
            } while (count);
            val = Evaluate(str.substr(1, index - 2));
            str = str.substr(std::min(str.size(), index -1));
        }
        else
            val = str[0] - '0';

        if (first) {
            result = val;
            first = false;
        }
        else if (isAdd) result += val;
        else result *= val;

        if (str.size() < 3) return result;
        str = str.substr(2);
        isAdd = str[0] == '+';
        str = str.substr(2);
    }
    return result;
}

i64 Evaluate2(std::string str)
{
    i64 result = 0;
    bool first = true, isAdd = false;
    while (str.size() > 0)
    {
        i64 val;
        if (str[0] == '(')
        {
            size_t count = 1, index = 1;
            do
            {
                if (str[index] == '(')
                    ++count;
                else if (str[index] == ')')
                    --count;
                ++index;
            } while (count);
            val = Evaluate2(str.substr(1, index - 2));
            str = str.substr(std::min(str.size(), index - 1));
        }
        else
            val = str[0] - '0';

        if (first) {
            result = val;
            first = false;
        }
        else result += val;

        if (str.size() < 3) return result;
        str = str.substr(2);

        if (str[0] == '*')
            return result * Evaluate2(str.substr(2));
        str = str.substr(2);
    }
    return result;
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
        std::string line(&buffer[0]);
        part1 += Evaluate(line);
        part2 = Evaluate2(line);
    }
    std::cout << "Part 1: " << part1 << std::endl << "Part 2 " << part2 << std::endl;
    return 0;
}