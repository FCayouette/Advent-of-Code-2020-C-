#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day10.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::string value;
    std::vector<int> values{ 0 };
    std::vector<char> locked{ 1 };

    while (in >> value)
    {
        values.push_back(stoi(value));
        locked.push_back(0);
    }

    std::sort(values.begin(), values.end());
    values.push_back(values.back() + 3);
    locked.push_back(1);
    size_t ones = 0, threes = 0;
    for (size_t i = 1; i < values.size(); ++i)
    {
        if (values[i - 1] + 1 == values[i])
            ++ones;
        else
        {
            ++threes;
            locked[i - 1] = locked[i] = 1;
        }
    }

    std::cout << "Part 1: " << ones * threes << std::endl;
    unsigned long long part2 = 1ull;
    
    size_t iter = 1;
    while (iter != locked.size())
    {
        if (locked[iter])
            ++iter;
        else
        {
            int count = 1;
            while (!locked[++iter])
                ++count;
            if (count == 1) part2 *= 2ull;
            else if (count == 2) part2 *= 4ull;
            else if (count == 3) part2 *= 7ull;
        }
    }

    std::cout << "Part 2: " << part2 << std::endl;
    return 0;
}
