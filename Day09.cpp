#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <algorithm>

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day07.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	std::deque<u64> nums;
	std::vector<u64> values;
	u64 num = 0;
	for (int i = 0; i < 25; ++i)
	{
		in >> num;
		nums.push_back(num);
		values.push_back(num);
	}

	u64 part1 = 0;

	while (in >> num)
	{
		bool found = false;
		for (size_t i = 0; i < nums.size() && !found; ++i)
			for (size_t j = i + 1; j < nums.size(); ++j)
				if (nums[i] + nums[j] == num)
				{
					found = true;
					break;
				}

		if (!found && !part1)
			part1 = num;
		
		if (!part1)
		{
			nums.pop_front();
			nums.push_back(num);
		}
		values.push_back(num);
	}
	std::cout << "Part 1: " << part1 << std::endl;
	bool found = false;
	for (size_t i = 0; i < values.size() && !found; ++i)
	{
		u64 runningSum = 0ull;
		for (size_t j = i; j < values.size() && runningSum < part1; ++j)
		{
			runningSum += values[j];
			if (runningSum == part1)
			{
				std::sort(values.begin() + i, values.begin() + j + 1);
				std::cout << "Part 2: " << values[i] + values[j] << std::endl;
				found = true;
				break;
			}
		}
	}

	return 0;
}