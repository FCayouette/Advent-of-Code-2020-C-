#include <iostream>
#include <fstream>
#include <string>
#include <numeric>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day12.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    size_t earliest, pos, count = 0;
    std::string line;
    in >> earliest >> line;
    using u64 = unsigned long long;
    size_t bestLine = 0, bestDelta = earliest;
    u64 bestStart = 0, bestFreq = 0;

    do
    {
        pos = line.find(','); 
        std::string val = line.substr(0, pos);
        if (val.size() != 1 || val[0] != 'x')
        {
            size_t busNum = stoi(val);
            size_t wait = busNum - (earliest % busNum);
            if (wait < bestDelta)
            {
                bestLine = busNum;
                bestDelta = wait;
            }
            if (bestFreq != 0)
            {
                u64 freq = std::lcm(bestFreq, (u64)busNum);
                u64 x = freq - bestFreq + bestStart + count;
                while (x % (u64)busNum)
                    x -= bestFreq;
                bestStart = x - count;
                bestFreq = freq;
            }
            else bestFreq = busNum;
        }
        line = line.substr(pos + 1);
        ++count;
    } while (pos != std::string::npos);

    std::cout << "Part 1: " << bestLine * bestDelta << std::endl << "Part 2: " << bestStart << std::endl;
    return 0;
}