#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <array>
#include <set>
#include <numeric>

using Decks = std::array<std::deque<size_t>, 2>;

void ShiftCards(Decks& players, size_t winner)
{
    if (winner == 0)
    {
        players[0].push_back(players[0].front());
        players[0].push_back(players[1].front());
    }
    else
    {
        players[1].push_back(players[1].front());
        players[1].push_back(players[0].front());
    }
    players[0].pop_front();
    players[1].pop_front();
}

size_t Recurse(Decks& players, bool firstRound = false)
{
    std::set<std::string> repeatCheck;

    auto RepeatCheck = [&players, &repeatCheck]()
    {
        std::string check;
        for (size_t c : players[0])
            check += std::to_string(c) + ',';
        check += ':';
        for (size_t c : players[1])
            check += std::to_string(c) + ',';
        if (repeatCheck.find(check) != repeatCheck.cend())
            return true;
        repeatCheck.insert(check);
        return false;
    };

    while (!players[0].empty() && !players[1].empty())
    {
        if (RepeatCheck())
            return 0;
        size_t winner;
        if (players[0].front() <= players[0].size() - 1 &&
            players[1].front() <= players[1].size() - 1)
        {
            Decks newDecks = players;
            newDecks[0].pop_front();
            newDecks[1].pop_front();
            newDecks[0].resize(players[0].front());
            newDecks[1].resize(players[1].front());
            winner = Recurse(newDecks);
        }
        else
            winner = players[0].front() < players[1].front() ? 1 : 0;

        ShiftCards(players, winner);
    }

    size_t winner = players[0].empty() ? 1 : 0, mul = 1;
    if (firstRound)
        std::cout << "Part 2: " << std::accumulate(players[winner].crbegin(), players[winner].crend(), 0, [&mul](size_t t, size_t c) { return t + c * mul++; }) << std::endl;
    return winner;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day22.exe Datafilename" << std::endl;
        return -1;
    }
    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::array<std::deque<size_t>, 2> players, part2;
    
    std::string line;
    in >> line >> line;
    size_t player = 0;
    while (in >> line)
    {
        if (line[0] == 'P')
        {
            ++player;
            in >> line;
        }
        else
            players[player].push_back(stoi(line));
    }

    part2 = players;

    while (!players[0].empty() && !players[1].empty())
        ShiftCards(players, players[0].front() < players[1].front());
    
    size_t winner = players[0].empty() ? 1 : 0, mul = 1;
    std::cout << "Part 1: " << std::accumulate(players[winner].crbegin(), players[winner].crend(), 0u, [&mul](size_t t, size_t card) { return t + card * mul++; }) << std::endl;

    Recurse(part2, true);

    return 0;
}