#include <iostream>
#include <fstream>
#include <vector>

using uint = unsigned int;

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
	uint val;
	while (in >> val)
		values.push_back(val);

	bool found = false;

	for (uint i = 0; i < values.size() - 1 && !found; ++i)
		for (uint j = i + 1, first = values[i]; j < values.size(); ++j)
			if (first + values[j] == 2020u)
			{
				std::cout << "Part 1: " << first * values[j] << std::endl;
				found = true;
				break;
			}

	found = false;
	for (uint i = 0; i < values.size() - 2 && !found; ++i)
		for (uint j = i + 1, first = values[i]; j < values.size() - 1 && !found; ++j)
			for (uint k = j + 1, second = values[j], two = first + second; k < values.size(); ++k)
				if (two + values[k] == 2020u)
				{
					std::cout << "Part 2: " << first * second * values[k] << std::endl;
					found = true;
					break;
				}
	return 0;
}
