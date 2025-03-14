#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <numeric>
#include <vector>
#include <istream>
#include <sstream>

constexpr std::string_view testData{
    R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9)"
};

const auto signFn = [](auto &&value) {
    if (value > 0) {
        return 1;
    }

    if (value < 0) {
        return -1;
    }

    return 0;
};

const auto numbersFn = [](const auto& line) {
    std::basic_stringstream lineStream {line};

    std::vector<int> numbers;

    int num;
    while (lineStream >> num) 
    {
        numbers.push_back(num);
    }
    return numbers;
};

const auto changeFn = [](auto &&pair) {
    return std::get<1>(pair) - std::get<0>(pair);
};

int answerPart1(std::basic_istream<char>&& data) {
    std::string line;
    auto safeReports = 0;

    while (std::getline(data, line)) {
        auto numbers = numbersFn(line);

        auto differences =  numbers | std::ranges::views::adjacent<2> | std::ranges::views::transform(changeFn);

        auto bIncreases = false;
        auto bDecreases = false;

        if (std::ranges::all_of(differences, [&bIncreases, &bDecreases](auto &&diff) {
            
            if (diff > 0)
            {
                bIncreases = true;
            }
            else if (diff < 0)
            {
                bDecreases = true;
            }

            auto change = abs(diff);

            return change > 0 && change < 4;
        }) && bIncreases != bDecreases) {
            safeReports++;
        }
    }

    return safeReports;
}

int answerPart2(std::basic_istream<char>&& data) {
    auto safeReports = 0;
    
    auto safeFn = [](auto values) {
        auto bIncreases = false;
        auto bDecreases = false; 

        auto differences = values | std::ranges::views::adjacent<2> | std::ranges::views::transform(changeFn);

        return std::ranges::all_of(differences, [&bIncreases, &bDecreases](auto &&diff) {
            
            if (diff > 0)
            {
                bIncreases = true;
            }
            else if (diff < 0)
            {
                bDecreases = true;
            }

            auto change = abs(diff);

            return change > 0 && change < 4;
        }) && bIncreases != bDecreases;
    };

    std::string line;

    while (std::getline(data, line)) {
        auto numbers = numbersFn(line);

        if (safeFn(numbers))
        {
            safeReports++;
            continue;;
        }

        auto size = static_cast<int>(std::ranges::distance(numbers));
        
        for (auto n : std::views::iota(0, size))
        {
            auto before = numbers | std::ranges::views::take(n);
            auto after = numbers | std::ranges::views::drop(n+1);
            
            // Would have liked to use std::ranges::views::concat;
            std::vector<int> dampened;
            std::ranges::copy(before, std::back_inserter(dampened));
            std::ranges::copy(after, std::back_inserter(dampened));

            if (safeFn(dampened))
            {
                safeReports++;
                break;
            }
        }
    }

    return safeReports;
}

int main() {
    std::cout << "Answer 1 to test data is: " << answerPart1(std::basic_stringstream{std::string{testData}}) << std::endl;
    std::cout << "Answer 1 to actual data is: " << answerPart1(std::ifstream{"data.txt"}) << std::endl;

    std::cout << "Answer 2 to test data is: " << answerPart2(std::basic_stringstream{std::string{testData}}) << std::endl;
    std::cout << "Answer 2 to actual data is: " << answerPart2(std::ifstream{"data.txt"}) << std::endl;

    return 0;
}