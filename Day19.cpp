#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <string_view>
#include <numeric>

class Rule;

using RuleList = std::vector<size_t>;
using RulePair = std::pair<size_t, Rule>;
using RuleSet = std::vector<RulePair>;

class Rule
{
public:
    Rule() = default;
    Rule(std::string& str)
    {
        if (str[0] == '"')
            endChar = str[1];
        else
        {
            RuleList workRule;
            while (true)
            {
                size_t pos = str.find(' ');
                if (pos == std::string::npos)
                {
                    workRule.push_back(stoi(str));
                    subRules.emplace_back(workRule);
                    break;
                }
                if (str[0] == '|')
                {
                    subRules.emplace_back(workRule);
                    workRule.clear();
                }
                else
                    workRule.push_back(stoi(str.substr(0, pos)));
                
                str = str.substr(pos + 1);
            }
        }
    }
    
    bool Evaluate(const RuleSet& set, const std::string_view& str, size_t& start) const
    {
        if (start >= str.size()) return false;
        if (endChar != '\0')
            return str[start++] == endChar;
        size_t backup = start;
        for (const auto& rule : subRules)
        {
            start = backup;
            if (std::all_of(rule.cbegin(), rule.cend(), [&set, &str, &start](size_t i) { return set[i].second.Evaluate(set, str, start); }))
                return true;
        }
        return false;
    }

    size_t MinSize(const RuleSet& set) const
    {
        if (endChar != '\0') return 1;

        size_t minSize = std::numeric_limits<size_t>::max();
        for (const std::vector<size_t>& rules : subRules)
            minSize = std::min(std::accumulate(rules.cbegin(), rules.cend(), 0u, [&set](size_t total, size_t r) { return total + set[r].second.MinSize(set); }), minSize);
        return minSize;
    }

private:
    std::vector<RuleList> subRules;
    char endChar = '\0';
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day19.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::array<char, 256> buffer;
    RuleSet rules;
    bool ruleParsing = true;
    size_t part1 = 0, part2 = 0, rule11MinSize;
    while (in.getline(&buffer[0], 256))
    {
        std::string line(&buffer[0]);
        if (line.empty())
        {
            ruleParsing = false;
            std::sort(rules.begin(), rules.end(), [](const RulePair& l, const RulePair& r) { return l.first < r.first; });
            rule11MinSize = rules[42].second.MinSize(rules) + rules[31].second.MinSize(rules);
            continue;
        }
        if (ruleParsing)
        {
            size_t pos = line.find(':');
            size_t num = stoi(line.substr(0, pos));
            std::string r = line.substr(pos + 2);
            rules.push_back({ num, r });
        }
        else
        {
            std::string_view str(&buffer[0]);
            size_t work = 0;
            if (rules[0].second.Evaluate(rules, str, work) && work == str.length())
                ++part1;
            // Part 2 evaluation
            work = 0;
            std::vector<size_t> startsAfter8;
            while (true)
            {
                if (rules[42].second.Evaluate(rules, str, work) && work < str.size())
                    startsAfter8.push_back(work);
                else break;
            }
            if (startsAfter8.size() >= 2)
            {
                bool found = false;
                startsAfter8.pop_back();
                for (size_t s : startsAfter8)
                {
                    if (found) break;
                    size_t iterations = 0;
                    while (!found && s + (++iterations) * rule11MinSize <= str.size())
                    {
                        bool valid = true;
                        size_t work = s;
                        for (size_t i = 0; valid && i < iterations; ++i)
                            valid = rules[42].second.Evaluate(rules, str, work) && work < str.size();
                        if (valid)
                            for (size_t i = 0; valid && i < iterations; ++i)
                                valid = rules[31].second.Evaluate(rules, str, work) && work <= str.size();
                        if (valid && work == str.size())
                        {
                            ++part2;
                            found = true;
                        }
                    }
                }
            }
        }
    }
    std::cout << "Part 1: " << part1 << std::endl << "Part 2: " << part2 << std::endl;

    return 0;
}