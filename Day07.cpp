#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <array>
#include <utility>
#include <string>
#include <string_view>

using BagCount = std::vector<std::pair<int, std::string>>;
using Rules = std::map<std::string, BagCount>;

bool CanContainGold(const Rules& rules, const std::string& color)
{	
	const BagCount& rule = rules.find(color)->second;
	if (rule.empty())
		return false;
	for (const auto& pair : rule)
		if (pair.second == "shiny gold" || CanContainGold(rules, pair.second))
			return true;
	return false;
}

size_t CountInGold(const Rules& rules, const std::string& color)
{
	const BagCount& rule = rules.find(color)->second;
	size_t result = 1;
	for (const auto& pair : rule)
		result += pair.first * CountInGold(rules, pair.second);
	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day07.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	Rules rules;

	std::array<char, 256> buffer;
	std::string_view contains(" bags contain"), noOther = ("no other bags."), bags (" bag");

	while (in.getline(&buffer[0], 256))
	{
		std::string line = &buffer[0];
		size_t index = line.find(contains);
		std::string color = line.substr(0, index);
		line = line.substr(index + contains.size()+1);
		rules[color] = std::vector<std::pair<int, std::string>>();
		if (line != noOther)
			while (true)
			{
				index = line.find(bags);
				std::string pair = line.substr(0, index);
				index = pair.find(' ');
				int num = atoi(pair.substr(0, index).c_str());
				rules[color].push_back(std::make_pair(num, pair.substr(index + 1)));
				index = line.find(", ");
				if (index != std::string::npos)
					line = line.substr(index + 2);
				else break;
			}
	}

	int part1 = 0;
	for (const auto& pair : rules)
		if (CanContainGold(rules, pair.first))
			++part1;

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << CountInGold(rules, "shiny gold")-1 << std::endl;

	return 0;
}