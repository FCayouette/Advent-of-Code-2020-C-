#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day21.exe Datafilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;

    std::unordered_map<std::string, size_t> ingredientNumber, allergenNumber;
    std::vector<std::string> numberToIngredient, numberToAllergen;

    auto GetIngredientNumber = [&ingredientNumber, &numberToIngredient](const std::string& ing) -> size_t
    {
        if (auto iter = ingredientNumber.find(ing);
            iter != ingredientNumber.cend())
            return iter->second;
        ingredientNumber[ing] = numberToIngredient.size();
        numberToIngredient.push_back(ing);
        return numberToIngredient.size() - 1;
    };

    auto GetAllergenNumber = [&allergenNumber, &numberToAllergen](const std::string& all) -> size_t
    {
        if (auto iter = allergenNumber.find(all);
            iter != allergenNumber.cend())
            return iter->second;
        allergenNumber[all] = allergenNumber.size();
        numberToAllergen.push_back(all);
        return numberToAllergen.size() - 1;
    };

    std::map<size_t, std::vector<size_t>> allergenPossibilities;
    std::vector<size_t> ingredientCounts, lineIngredients, lineAllergens;
    std::string data;
    bool allergens = false;

    size_t part1 = 0;
    while (in >> data)
    {
        if (data[0] == '(')
            allergens = true;
        else if (!allergens)
        {
            ++part1;

            size_t iNum = GetIngredientNumber(data);
            lineIngredients.push_back(iNum);
            if (iNum == ingredientCounts.size())
                ingredientCounts.push_back(1);
            else
                ++ingredientCounts[iNum];
        }
        else
        {
            lineAllergens.push_back(GetAllergenNumber(data.substr(0, data.size() - 1)));

            allergens = data[data.size() - 1] == ',';
            if (!allergens)
            {
                std::sort(lineIngredients.begin(), lineIngredients.end());
                for (size_t a : lineAllergens)
                {
                    if (auto iter = allergenPossibilities.find(a);
                        iter == allergenPossibilities.cend())
                        allergenPossibilities[a] = lineIngredients;
                    else
                    {
                        std::vector<size_t>& currentPos = iter->second, newPos;
                        std::set_intersection(lineIngredients.cbegin(), lineIngredients.cend(), currentPos.cbegin(), currentPos.cend(), std::back_inserter(newPos));
                        std::swap(newPos, currentPos);
                    }
                }
                lineIngredients.clear();
                lineAllergens.clear();
            }
        }
    }
    std::vector<std::pair<std::string, size_t>> allToIng;

    while (allergenPossibilities.size())
    {
        std::map<std::size_t, std::vector<size_t>>::iterator iter = allergenPossibilities.begin();
        while (iter->second.size() != 1)
            ++iter;
        size_t toDelete = iter->second.front();
        part1 -= ingredientCounts[toDelete];
        allToIng.emplace_back(numberToAllergen[iter->first], toDelete);
        allergenPossibilities.erase(iter);
        for (auto& x : allergenPossibilities)
            if (auto extra = std::find(x.second.begin(), x.second.end(), toDelete);
                extra != x.second.end())
                x.second.erase(extra);
    }

    std::sort(allToIng.begin(), allToIng.end());
    std::cout << "Part 1: " << part1 << "\nPart 2: " 
        << std::accumulate(std::next(allToIng.cbegin()), allToIng.cend(), numberToIngredient[allToIng[0].second], 
                           [&numberToIngredient](const std::string& l, const auto& p) {return l + ',' + numberToIngredient[p.second]; }) << std::endl;
    return 0;
}