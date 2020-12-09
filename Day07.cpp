#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <array>
#include <utility>
#include <string>
#include <string_view>
#include <unordered_map>

// Allows of recursive lambdas
template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day07.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	std::unordered_map<std::string, size_t> colorCode;
	size_t totalColors = 0;
	std::vector<std::vector<std::pair<size_t, size_t>>> fastRules;

	auto GetColorCode = [&colorCode, &totalColors, &fastRules](const std::string& color)
	{
		if (auto iter = colorCode.find(color);
			iter != colorCode.cend())
			return iter->second;
		size_t result = totalColors;
		fastRules.emplace_back();
		colorCode[color] = totalColors++;
		return result;
	};
	
	// Make sure we have shiny gold as color 0
	GetColorCode("shiny gold");

	std::array<char, 256> buffer;
	constexpr std::string_view contains(" bags contain"), noOther = ("no other bags."), bags (" bag");

	while (in.getline(&buffer[0], buffer.size()))
	{
		std::string line = &buffer[0];
		size_t index = line.find(contains);
		size_t colorCode = GetColorCode(line.substr(0, index));
		line = line.substr(index + contains.size()+1);
		if (line != noOther)
			while (true)
			{
				index = line.find(bags);
				std::string pair = line.substr(0, index);
				index = pair.find(' ');
				int num = atoi(pair.substr(0, index).c_str());
				size_t targetCode = GetColorCode(pair.substr(index + 1));
				fastRules[colorCode].emplace_back(num, targetCode);
				index = line.find(", ");
				if (index != std::string::npos)
					line = line.substr(index + 2);
				else break;
			}
	}

	auto CanContainShinyGold = y_combinator([&fastRules](auto&& CanContainShinyGold, size_t color) -> bool
		{
			const std::vector<std::pair<size_t, size_t>>& contains = fastRules[color];
			for (const auto& pair : contains)
				if (!pair.second || CanContainShinyGold(pair.second))
					return true;
			return false;
		});

	auto CountInColor = y_combinator([&fastRules](auto&& CountInColor, size_t color) -> size_t
		{
			size_t result = 1;
			for (const auto& pair : fastRules[color])
				result += pair.first * CountInColor(pair.second);
			return result;
		});

	int part1 = 0;
	for (size_t i = 1; i < fastRules.size(); ++i)
		if (CanContainShinyGold(i))
			++part1;

	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << CountInColor(0) - 1 << std::endl;
	return 0;
}