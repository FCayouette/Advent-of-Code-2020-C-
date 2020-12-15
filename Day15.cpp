#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day15.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::string line;
    in >> line;
    size_t comma = 0, count = 0;
    std::unordered_map<int, std::pair<size_t, size_t>> numbers;
    int num;
    while (true)
    {
        num = stoi(line.substr(0, comma = line.find(',')));
        numbers[num] = std::make_pair(count, count);
        ++count;
        if (comma == std::string::npos)
            break;
        line = line.substr(comma + 1);
    }

    while (count < 30000000)
    {
        num = numbers[num].first - numbers[num].second;
        if (auto iter = numbers.find(num);
            iter == numbers.cend())
            numbers[num] = std::make_pair(count, count);
        else
        {
            iter->second.second = iter->second.first;
            iter->second.first = count;
        }
        if (++count == 2020)
            std::cout << "Part 1: " << num << std::endl;
    }

    std::cout << "Part 2: " << num << std::endl;

    return 0;
}

