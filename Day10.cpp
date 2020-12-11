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

    while (in >> value)
        values.push_back(stoi(value));
    
    std::sort(values.begin(), values.end());
    values.push_back(values.back() + 3);
    size_t ones = 0, threes = 0, count = 0;
    unsigned long long part2 = 1ull;
    
    for (size_t i = 1; i < values.size(); ++i)
    {
        if (int diff = values[i] - values[i - 1];
            diff == 1)
        {
            ++ones;
            ++count;
        }
        else
        {
            ++threes;
            if (count > 0)
            {
                --count;
                part2 *= (count < 2 ? count == 1 ? 2 : 1 : count == 2 ? 4 : 7);
                count = 0;
            }
        }
    }

    std::cout << "Part 1: " << ones * threes << std::endl << "Part 2: " << part2 << std::endl;
    return 0;
}
