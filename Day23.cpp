#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>

struct Link
{
    Link* next = nullptr, * prev = nullptr;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day23.exe labeling" << std::endl;
        return -1;
    }

    std::string labeling(argv[1]);
    std::vector<Link> cups(10);

    auto PlayGame = [&cups, &labeling](size_t nTurns)
    {
        size_t current = 0, nCups = cups.size();
        for (size_t i = 1; i < cups.size(); ++i)
        {
            size_t val = i - 1 < labeling.size() ? labeling[i - 1] - '0' : i;

            if (!current)
            {
                current = val;
                cups[current].next = cups[current].prev = &cups[current];
            }
            else
            {
                Link* last = cups[current].prev;
                last->next = &cups[val];
                cups[val].prev = last;
                cups[val].next = &cups[current];
                cups[current].prev = &cups[val];
            }
        }

        std::array<size_t, 3> pickedUp;
        while (nTurns--)
        {
            Link* l0 = cups[current].next, * l1 = l0->next, * l2 = l1->next;
            pickedUp[0] = l0 - &cups[0];
            pickedUp[1] = l1 - &cups[0];
            pickedUp[2] = l2 - &cups[0];
            size_t destVal = (current + nCups - 3) % (nCups - 1) + 1;
            while (std::find(pickedUp.cbegin(), pickedUp.cend(), destVal) != pickedUp.cend())
                destVal = (destVal + nCups - 3) % (nCups - 1) + 1;

            cups[current].next = l2->next;
            l2->next->prev = &cups[current];
            Link& destination = cups[destVal], * dn = destination.next;
            destination.next = l0;
            l0->prev = &destination;
            l2->next = dn;
            dn->prev = l2;
            current = cups[current].next - &cups[0];
        }
    };

    PlayGame(100);
    Link* startLink = &cups[1], *currentLink = startLink->next;
    std::cout << "Part 1: ";
    while (currentLink != startLink)
    {
        std::cout << currentLink - &cups[0];
        currentLink = currentLink->next;
    }
    std::cout << std::endl;
    cups.resize(1000001);
    PlayGame(10000000);
    std::cout << "Part 2: " << (unsigned long long)(cups[1].next - &cups[0]) * (cups[1].next->next - &cups[0]) << std::endl;

    return 0;
}