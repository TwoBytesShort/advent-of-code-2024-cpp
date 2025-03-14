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

constexpr std::string_view actualDataFilename = "data.txt";

class Row : public std::vector<int> {
    public:
        Row() = default;

        Row(const std::string& line)
        {
            std::stringstream lineStream {line};

            int num;
            while (lineStream >> num) 
            {
                push_back(num);
            }
        }
    
};

class InputData {
    public:        
        std::optional<Row> getRow()
        {
            std::string line;
            return std::getline(getStream(), line) ? std::optional<Row>(Row(line)) : std::nullopt;
        }

    private:
        virtual std::istream& getStream() = 0;

};


class StringInputData : public InputData {
    public:
        StringInputData(std::string inputData)
            : InputData{}
            , stream(inputData)
        {
        };

    private:
        std::stringstream stream;

        std::istream& getStream() override {
            return stream;
        }
};

class FileInputData : public InputData {
    public:
        FileInputData(std::string filename)
            : InputData{}
            , stream(filename)
        {
        };    

    private:
        std::ifstream stream;

        std::istream& getStream() override {
            return stream;
        }
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

const auto changeFn = [](auto &&pair) {
    return std::get<1>(pair) - std::get<0>(pair);
};

int answerPart1(InputData&& data) {
    auto safeReports = 0;

    while (auto row = data.getRow()) {
        auto differences =  *row | std::ranges::views::adjacent<2> | std::ranges::views::transform(changeFn);

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

int answerPart2(InputData&& data) {
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

    while (auto row = data.getRow()) {
        if (safeFn(*row))
        {
            safeReports++;
            continue;;
        }

        auto size = static_cast<int>(std::ranges::distance(*row));
        
        for (auto n : std::views::iota(0, size))
        {
            auto before = *row | std::ranges::views::take(n);
            auto after = *row | std::ranges::views::drop(n+1);
            
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

    std::cout << "Answer 1 to test data is: " << answerPart1(StringInputData(std::string(testData))) << std::endl;
    std::cout << "Answer 1 to actual data is: " << answerPart1(FileInputData(std::string(actualDataFilename))) << std::endl;

    std::cout << "Answer 2 to test data is: " << answerPart2(StringInputData(std::string(testData))) << std::endl;
    std::cout << "Answer 2 to actual data is: " << answerPart2(FileInputData(std::string(actualDataFilename))) << std::endl;

    return 0;
}