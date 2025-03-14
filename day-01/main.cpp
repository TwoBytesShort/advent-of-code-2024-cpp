#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <numeric>

constexpr std::string_view testData{
    R"(3   4
4   3
2   5
1   3
3   9
3   3)"
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

int answerPart1(std::string_view data) {
    auto lines = data | std::ranges::views::split('\n');

    // It is known as a "universal reference" or "forwarding reference" when used in template code.
    // The type deduced by auto&& depends on the value category of the expression it is initialized with:
    // 1. If the expression is an lvalue, auto&& deduces to an lvalue reference(T &)
    // 2. If the expression is an rvalue, auto&& deduces to an rvalue reference(T &&)

    // TODO: what is the difference between an rvalue and an lvalue

    std::vector<int> firstValues;
    std::vector<int> secondValues;

    for (auto &&line: lines) {
        auto values = line | std::ranges::views::split(' ') | std::ranges::views::filter(notEmptyFn);

        if (auto firstValueItr = values.begin(), secondValueItr = ++values.begin(); firstValueItr != values.end()) {
            firstValues.push_back(parseIntFn(*firstValueItr));
            secondValues.push_back(parseIntFn(*secondValueItr));
        }
    }

    std::ranges::sort(firstValues);
    std::ranges::sort(secondValues);

    auto differences = std::ranges::views::zip(firstValues, secondValues) | std::ranges::views::transform(
                           [](auto &&valuePair) {
                               return std::abs(std::get<0>(valuePair) - std::get<1>(valuePair));
                           });

    return std::accumulate(differences.begin(), differences.end(), 0);
}

int answerPart2(std::string_view data) {
    auto lines = data | std::ranges::views::split('\n');

    std::vector<int> firstValues;
    std::map<int, int> secondValues;

    for (auto &&line: lines) {
        auto values = line | std::ranges::views::split(' ') | std::ranges::views::filter(notEmptyFn);

        if (auto firstValueItr = values.begin(), secondValueItr = ++values.begin(); firstValueItr != values.end()) {
            firstValues.push_back(parseIntFn(*firstValueItr));
            ++secondValues[parseIntFn(*secondValueItr)];
        }
    }

    auto occurrences = firstValues | std::ranges::views::transform([&secondValues](auto &&value) {
        return value * secondValues[value];
    });

    return std::accumulate(occurrences.begin(), occurrences.end(), 0);
}


int main() {
    const auto actualData = readActualDataFn("data.txt");

    std::cout << "Answer 1 to test data is: " << answerPart1(testData) << std::endl;
    std::cout << "Answer 1 to actual data is: " << answerPart1(actualData) << std::endl;

    std::cout << "Answer 2 to test data is: " << answerPart2(testData) << std::endl;
    std::cout << "Answer 2 to actual data is: " << answerPart2(actualData) << std::endl;

    return 0;
}
