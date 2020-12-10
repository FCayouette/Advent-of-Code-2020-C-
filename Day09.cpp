#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day09.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	std::vector<u64> values;
	u64 num = 0;
	for (int i = 0; i < 25; ++i)
	{
		in >> num;
		values.push_back(num);
	}

	u64 part1 = 0;

	while (in >> num)
	{
		if (!part1)
		{
			bool found = false;
			for (size_t i = values.size() - 25; i < values.size() && !found; ++i)
				for (size_t j = i + 1; j < values.size(); ++j)
					if (values[i] + values[j] == num)
					{
						found = true;
						break;
					}

			if (!found)
				part1 = num;
		}
		values.push_back(num);
	}
	std::cout << "Part 1: " << part1 << std::endl;

	u64 runningSum = values[0];
	size_t low = 0, high = 1;
	while (runningSum != part1)
	{
		while (runningSum < part1) runningSum += values[high++];
		while (runningSum > part1) runningSum -= values[low++];
	}

	std::sort(values.begin() + low, values.begin() + high);
	std::cout << "Part 2: " << values[low] + values[high-1] << std::endl;
	return 0;
}