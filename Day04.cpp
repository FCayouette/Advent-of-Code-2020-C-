#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <array>
#include <algorithm>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day03.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    
    std::map<std::string, std::string> fields;

    std::array<std::string, 7> requiredFields = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    std::array<std::string, 7> colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    
    int part1 = 0, part2 = 0;

    auto Validate = [&fields, &requiredFields, &colors, &part1, &part2]()
    {
        if (std::all_of(requiredFields.cbegin(), requiredFields.cend(), [&fields](const std::string& field) { return fields.find(field) != fields.cend(); }))
        {
            ++part1;
            if (int byr = atoi(fields["byr"].c_str());
                byr >= 1920 && byr <= 2002)
                if (int iyr = atoi(fields["iyr"].c_str());
                    iyr >= 2010 && iyr <= 2020)
                    if (int eyr = atoi(fields["eyr"].c_str());
                        eyr >= 2020 && eyr <= 2030)
                    {
                        std::string height = fields["hgt"];
                        bool valid = false;
                        if (height.find("in") != std::string::npos)
                        {
                            int val = atoi(height.substr(0, height.size() - 2).c_str());
                            valid = val >= 59 && val <= 76;
                        }
                        else if (height.find("cm") != std::string::npos)
                        {
                            int val = atoi(height.substr(0, height.size() - 2).c_str());
                            valid = val >= 150 && val <= 193;
                        }
                        if (valid)
                        {
                            if (const std::string& hcl = fields["hcl"];
                                hcl.size() == 7 && hcl[0] == '#')
                            {
                                if (std::any_of(colors.cbegin(), colors.cend(), [&val = fields["ecl"]](const std::string& s){return val == s;}))
                                    if (const std::string& pid = fields["pid"];
                                        pid.size() == 9)
                                        ++part2;
                            }
                        }
                    }      
        }
    };

    std::array<char, 100> buffer;
    while (in.getline(&buffer[0], 256))
    {
        if (strlen(&buffer[0]) == 0)
        {
            Validate();
            fields.clear();
        }
        else
        {
            std::string line = &buffer[0];
            size_t start = 0, end;
            do
            {
                std::string field = line.substr(start, 3);
                end = line.find(' ', start + 4);
                std::string value = line.substr(start + 4, end == std::string::npos ? std::string::npos : end - start - 4);
                if (end != std::string::npos)
                    start = end + 1;
                fields[field] = value;
            } while (end != std::string::npos);
        }
    }
    Validate();
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;
    return 0;
}
