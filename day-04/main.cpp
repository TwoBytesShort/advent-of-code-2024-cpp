#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <numeric>

constexpr std::string_view testData{
    R"()"
};

const std::vector<std::pair<int,int>> tests = {{0, -1}, {1, -1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}};

const auto readActualDataFn = [](auto &&filename) {
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


}

int answerPart2(std::string_view data) {

}


int main() {
    const auto actualData = readActualDataFn("data.txt");

    std::cout << "Answer 1 to test data is: " << answerPart1(testData) << std::endl;
    //std::cout << "Answer 1 to actual data is: " << answerPart1(actualData) << std::endl;

    //std::cout << "Answer 2 to test data is: " << answerPart2(testData) << std::endl;
    //std::cout << "Answer 2 to actual data is: " << answerPart2(actualData) << std::endl;

    return 0;
}
