#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <numeric>
#include <vector>

constexpr std::string_view testData{
    R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9)"
};

constexpr auto readActualDataFn = [](auto &&filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + std::string{filename});
    }

    return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
};

constexpr auto parseIntFn = [](auto &&value) {
    return std::stoi(std::string(value.begin(), value.end()));
};

constexpr auto notEmptyFn = [](auto &&value) {
    return !value.empty();
};

constexpr auto signFn = [](auto &&value) {
    if (value > 0) {
        return 1;
    }

    if (value < 0) {
        return -1;
    }

    return 0;
};

constexpr auto changeFn = [](auto &&pair) {
    return std::get<1>(pair) - std::get<0>(pair);
};   

int answerPart1(std::string_view data) {
    auto lines = data | std::ranges::views::split('\n');

    auto safeReports = 0;

    for (auto &&line: lines) {
        auto differences = line | std::ranges::views::split(' ') | std::ranges::views::filter(notEmptyFn) | std::ranges::views::transform(parseIntFn) | std::ranges::views::adjacent<2> |
        std::ranges::views::transform(changeFn);

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

int answerPart2(std::string_view data) {
    auto lines = data | std::ranges::views::split('\n');

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

    for (auto &&line: lines) {
        auto valuesView = line | std::ranges::views::split(' ') | std::ranges::views::filter(notEmptyFn) | std::ranges::views::transform(parseIntFn);
        auto values = std::vector<int>(valuesView.begin(), valuesView.end());

        if (safeFn(values))
        {
            safeReports++;
            continue;;
        }

        auto size = static_cast<int>(std::ranges::distance(values));
        
        for (auto n : std::views::iota(0, size))
        {
            auto before = values | std::ranges::views::take(n);
            auto after = values | std::ranges::views::drop(n+1);
            
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
    const auto actualData = readActualDataFn("data.txt");

    std::cout << "Answer 1 to test data is: " << answerPart1(testData) << std::endl;
    std::cout << "Answer 1 to actual data is: " << answerPart1(actualData) << std::endl;

    std::cout << "Answer 2 to test data is: " << answerPart2(testData) << std::endl;
    std::cout << "Answer 2 to actual data is: " << answerPart2(actualData) << std::endl;

    return 0;
}