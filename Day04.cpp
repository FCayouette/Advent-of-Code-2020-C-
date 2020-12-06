#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <array>
#include <algorithm>
#include <string_view>
#include <charconv>
#include <functional>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day03.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;
    
    std::array<std::string, 7> requiredFields = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    constexpr std::array<std::string_view, 7> colors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

    std::map<std::string_view, std::function<bool(const std::string&)>> validators = 
    {
        { "cid", [](const std::string&) { return true; } },
        { "byr", [](const std::string& val) { int byr = std::stoi(val); return byr >= 1920 && byr <= 2002; } },
        { "iyr", [](const std::string& val) { int iyr = std::stoi(val); return iyr >= 2010 && iyr <= 2020; } },
        { "eyr", [](const std::string& val) { int eyr = std::stoi(val); return eyr >= 2020 && eyr <= 2030; } },
        { "hcl", [](const std::string& val) { return val.size() == 7 && val[0] == '#'; } },
        { "pid", [](const std::string& val) { return val.size() == 9; } },
        { "ecl", [&colors](const std::string& val) { return std::any_of(colors.cbegin(), colors.cend(), [&val](const std::string_view& c) { return c == val; }); } },
        { "hgt", [](const std::string& val) { 
            if (val.ends_with("in"))
            {
                long num;
                std::from_chars(&val[0], &val[val.size() - 2], num);
                return num >= 59 && num <= 76;
            }
            else if (val.ends_with("cm"))
            {
                long num;
                std::from_chars(&val[0], &val[val.size() - 2], num);
                return num >= 150 && num <= 193;
            }
            return false; } }
    };

    std::map<std::string, std::string> fields;
    int part1 = 0, part2 = 0;

    auto Validate = [&fields, &requiredFields, &colors, &part1, &part2, &validators]()
    {
        if (std::all_of(requiredFields.cbegin(), requiredFields.cend(), [&fields](const std::string& field) { return fields.find(field) != fields.cend(); }))
        {
            ++part1;
            if (std::all_of(fields.cbegin(), fields.cend(), [&validators](const auto& field) { return validators.find(field.first)->second(field.second); }))
                ++part2;
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
                fields[field] = line.substr(start + 4, end == std::string::npos ? end : end - start - 4);
                if (end != std::string::npos)
                    start = end + 1;
            } while (end != std::string::npos);
        }
    }
    Validate(); // Must check last entry as there is no empty line at the end of the data file
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;
    return 0;
}
