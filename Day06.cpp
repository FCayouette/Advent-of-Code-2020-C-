#include <iostream>
#include <fstream>
#include <string>
#include <array>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Day06.exe DataFilename" << std::endl;
		return -1;
	}

	std::ifstream in(argv[1], std::ios::in);
	if (!in) return -1;

	std::array<int, 26> histogram;
	histogram.fill(0);
	int part1 = 0, part2 = 0, groupCount = 0;
	
	auto CountGroup = [&part1, &part2, &groupCount, &histogram]()
	{
		for (int i : histogram)
		{
			if (i != 0)
				++part1;
			if (i == groupCount)
				++part2;
		}
	};

	std::array<char, 30> line;
	while (in.getline(&line[0], 30))
	{
		if (strlen(&line[0]) == 0)
		{
			CountGroup();
			histogram.fill(0);
			groupCount = 0;
		}
		else
		{
			int i = 0;
			while (line[i] != '\0')
				++histogram[line[i++] - 'a'];
			++groupCount;
		}
	}
	CountGroup();
	
	std::cout << "Part 1: " << part1 << std::endl;
	std::cout << "Part 2: " << part2 << std::endl;
	return 0;
}
