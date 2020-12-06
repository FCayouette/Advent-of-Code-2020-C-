#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day05.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	std::string seat;
	std::vector<size_t> ids;

	while (in >> seat)
	{
		size_t id = 0;
		for (size_t i = 0; i < seat.size(); ++i)
			id = id * 2 + (seat[i] == (i < 7 ? 'B' : 'R'));
		ids.push_back(id);
	}

	std::sort(ids.begin(), ids.end());
	std::cout << "Part 1: " << ids.back() << std::endl;

	size_t low = 0, high = ids.size() - 1, diff = ids[0];
	do
	{
		size_t mid = (low + high) / 2;
		if (mid + diff == ids[mid])
			low = mid + 1;
		else
			high = mid - 1;
	} while (low <= high);
	std::cout << "Part 2: " << low + diff << std::endl;

	return 0;
}
