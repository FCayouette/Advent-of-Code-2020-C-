#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day01.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	std::vector<int> values;
	int val, part1 = 0, part2 = 0;
	
	while ((!part1 || !part2) && in >> val)
	{
		int comp = 2020 - val;
		if (!part1 && std::binary_search(values.cbegin(), values.cend(), comp))
			part1 = comp * val;
		else if (!part2)
			for (auto first = values.cbegin(); first != values.cend() && comp > *first * 2; ++first)
				if (std::binary_search(std::next(first), values.cend(), comp - *first))
					part2 = val * *first * (comp - *first);
		values.push_back(val);
		for (size_t i = values.size(); --i > 0 && values[i] < values[i - 1];)
			std::swap(values[i], values[i - 1]);
	}

	std::cout << "Part 1: " << part1 << std::endl << "Part 2: " << part2 << std::endl;
	in.close();
	return 0;
}
