#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <set>
#include <regex>

constexpr std::string_view testData{
    R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5)))"
};

constexpr std::string_view testData2{
    R"(xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5)))"
};

constexpr auto readActualDataFn = [](auto &&filename) {
    std::ifstream file{filename};

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + std::string{filename});
    }

    return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
};

int answerPart1(const std::string& data) {
    std::regex pattern(R"(mul\(([0-9]+),([0-9]+)\))");
    
    auto matchesBegin = std::sregex_iterator(data.begin(), data.end(), pattern);
    auto matchesEnd = std::sregex_iterator();

    auto sum = 0;

    for (auto it = matchesBegin; it != matchesEnd; ++it) {
        std::smatch match = *it;
        
        auto first = std::stoi(match[1].str());
        auto second = std::stoi(match[2].str());
        
        sum += first * second;
    }

    return sum;
}

int answerPart2(const std::string& data) {
    std::regex pattern(R"(mul\(([0-9]+),([0-9]+)\)|do\(\)|don't\(\))");
    
    auto matchesBegin = std::sregex_iterator(data.begin(), data.end(), pattern);
    auto matchesEnd = std::sregex_iterator();

    auto sum = 0;
    auto doMul = true;

    for (auto it = matchesBegin; it != matchesEnd; ++it) {
        std::smatch match = *it;

        if (match[0].str() == "do()") {
            doMul = true;
        }
        else if (match[0].str() == "don't()")
        {
            doMul = false;
        }
        else if (doMul)
        {            
            auto first = std::stoi(match[1].str());
            auto second = std::stoi(match[2].str());
            
            sum += first * second;
        }
    }

    return sum;
}

int main() {
    const auto actualData = readActualDataFn("data.txt");

    std::cout << "Answer 1 to test data is: " << answerPart1(std::string(testData)) << std::endl;
    std::cout << "Answer 1 to actual data is: " << answerPart1(actualData) << std::endl;

    std::cout << "Answer 2 to test data is: " << answerPart2(std::string(testData2)) << std::endl;
    std::cout << "Answer 2 to actual data is: " << answerPart2(actualData) << std::endl;

    return 0;
}

