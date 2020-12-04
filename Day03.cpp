#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <numeric>
#include <functional>

struct Accumulator
{
    constexpr Accumulator(int X, int Y=1) : dx(X), dy(Y) {}
    void check(const std::string& line)
    {
        if (!y && line[x] == '#')
            ++total;
        if (++y == dy)
        {
            x = (x + dx) % line.size();
            y = 0;
        }
    }
    constexpr operator long long() const { return total; }
private:
    int x = 0, y = 0, dx, dy;
    long long total = 0;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day03.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::array<Accumulator, 5> ratios = { 3, 1, 5, 7, Accumulator(1, 2) };
    std::string row;
    while (in >> row)
        for (Accumulator& acc : ratios)
            acc.check(row);
  
    std::cout << "Part 1: " << (long long)ratios.front() << std::endl;
    std::cout << "Part 2: " << std::accumulate(ratios.cbegin(), ratios.cend(), 1ll, std::multiplies()) << std::endl;
        
    return 0;
}
