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
		size_t row = 0, col = 0;
		for (size_t i = 0; i < 7; ++i)
			row = row * 2 + (seat[i] == 'B' ? 1 : 0);
		for (size_t i = 7; i < seat.size(); ++i)
			col = col * 2 + (seat[i] == 'R' ? 1 : 0);
		
		ids.push_back(row * 8 + col);
	}

	std::sort(ids.begin(), ids.end());
	std::cout << "Part 1: " << ids.back() << std::endl;

	for (size_t i = 1; i < ids.size(); ++i)
		if (ids[i - 1] + 1 != ids[i])
		{
			std::cout << "Part 2: " << ids[i - 1] + 1 << std::endl;
			break;
		}

	return 0;
}
