#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include <string>

class Node
{
public:
    Node(size_t v = 0, Node* n = nullptr, Node* p = nullptr) : val(v), next(n), prev(p) {}
    size_t val;
    Node* next, * prev;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day03.exe labeling" << std::endl;
        return -1;
    }

    std::string labeling("315679824");
    std::vector<char> cups;
    std::vector<Node*> nodePtrs(1000001);
    Node* buffer = nullptr;
    for (size_t i = 0; i < 1000000; ++i)
    {
        Node* n;
        if (i < labeling.size())
        {
            cups.push_back(labeling[i]-'0');
            n = new Node(labeling[i]-'0');
        }
        else
            n = new Node(i + 1);
        nodePtrs[n->val] = n;
        if (buffer == nullptr)
        {
            buffer = n;
            buffer->next = buffer;
            buffer->prev = buffer;
        }
        else
        {
            Node* last = buffer->prev;
            last->next = n;
            n->prev = last;
            n->next = buffer;
            buffer->prev = n;
        }
    }

    std::array<char, 3> pickedUp;
    for (size_t turn = 0, current = 0; turn < 100; ++turn)
    {
        char val = cups[current];
        bool atEnd = false;
        for (size_t i = 0; i < 3; ++i)
        {
            if (current + 1 == cups.size())
                atEnd = true;
            if (atEnd)
            {
                pickedUp[i] = cups[0];
                cups.erase(cups.begin());
            }
            else
            {
                pickedUp[i] = cups[current + 1];
                cups.erase(cups.begin() + current + 1);
            }
        }
        char target = val == 1 ? 9 : val - 1;
        std::vector<char>::const_iterator iter;
        while ((iter = std::find(cups.cbegin(), cups.cend(), target)) == cups.cend())
            target = target == 1 ? 9 : target - 1;
        ++iter;
        for (int i = 2; i >= 0; --i)
            iter = cups.insert(iter, pickedUp[i]);
        
        current = (std::distance(cups.cbegin(), std::find(cups.cbegin(), cups.cend(), val)) + 1) % 9;
    }

    auto iter = std::find(cups.cbegin(), cups.cend(), 1);
    
    if (++iter == cups.cend()) iter = cups.cbegin();
    std::cout << "Part 1: ";
    while (*iter != 1)
    {
        std::cout << (char)(*iter + '0');
        if (++iter == cups.cend()) iter = cups.cbegin();
    }
    std::cout << std::endl;

    for (size_t turn = 0; turn < 10000000; ++turn)
    {
        Node* n1 = buffer->next, * n2 = n1->next, * n3 = n2->next;
        std::set<size_t> pickedUp;
        pickedUp.insert(n1->val);
        pickedUp.insert(n2->val);
        pickedUp.insert(n3->val);
        size_t nextVal = buffer->val;
        nextVal = 1 + ((1000000 + nextVal - 2) % 1000000);
        while (pickedUp.find(nextVal) != pickedUp.cend())
            nextVal = 1 + ((1000000 + nextVal - 2) % 1000000);

        buffer->next = n3->next;
        n3->next->prev = buffer;

        Node* destinationCup = nodePtrs[nextVal], * nn = destinationCup->next;
        destinationCup->next = n1;
        n1->prev = destinationCup;
        n3->next = nn;
        nn->prev = n3;
        buffer = buffer->next;
    }

    std::cout << "Part 2: " << (unsigned long long)nodePtrs[1]->next->val * nodePtrs[1]->next->next->val << std::endl;

    std::for_each(nodePtrs.begin(), nodePtrs.end(), [](Node* n) { delete n; });

    return 0;
}