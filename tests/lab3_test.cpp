#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdlib.h>

#include "parent.hpp"

namespace fs = std::filesystem;

void Check(const std::vector<std::string>& input,
           const std::vector<std::string>& expectedOutput) {
    constexpr const char* fileWithInput = "input.txt";
    constexpr const char* fileWithOutput1 = "output1.txt";
    constexpr const char* fileWithOutput2 = "output2.txt";

    std::vector<std::string> inputCopy = input;
    std::vector<std::string> expectedOutputCopy = expectedOutput;
    std::vector<std::string> output;
    sort(inputCopy.begin(), inputCopy.end());
    sort(expectedOutputCopy.begin(), expectedOutputCopy.end());

    {
        auto inFile = std::ofstream(fileWithInput);

        inFile << fileWithOutput1 << '\n';
        inFile << fileWithOutput2 << '\n';

        for(const auto& line : input) {
            inFile << line << '\n';
        }
    }

    std::ifstream inFile(fileWithInput);

    if (getenv("PATH_TO_CHILD") == NULL) {
        printf("ERROR: PATH_TO_CHILD was not specified.\n");
        exit(0);
    }
    ParentRoutine(getenv("PATH_TO_CHILD"), inFile);

    auto outFile1 = std::ifstream(fileWithOutput1);
    auto outFile2 = std::ifstream(fileWithOutput2);

    ASSERT_TRUE(outFile1.good());
    ASSERT_TRUE(outFile2.good());

    std::vector<std::string> output1;
    for (std::string line; std::getline(outFile1, line); ) {
        output.push_back(line);
    }
    for (std::string line; std::getline(outFile2, line); ) {
        output.push_back(line);
    }
    sort(output.begin(), output.end());

    ASSERT_EQ(output.size(), expectedOutputCopy.size());
    for (size_t i = 0; i < output.size(); i++) {
        EXPECT_EQ(output[i], expectedOutputCopy[i]);
    }
    auto removeIfExists = [](const std::string& path) {
        if (fs::exists(path)) {
            fs::remove(path);
        }
    };

    removeIfExists(fileWithInput);
    removeIfExists(fileWithOutput1);
    removeIfExists(fileWithOutput2);
}

TEST(Lab3, Test1) {
    std::vector<std::string> input = {
        "faasdfsa",
        "kokla",
        "foobar"
    };

    std::vector<std::string> expOutput = {
        "asfdsaaf",
        "alkok",
        "raboof"
    };

    Check(input, expOutput);
}

TEST(Lab3, Test2) {
    std::vector<std::string> input = {
        "",
        "'"
    };

    std::vector<std::string> expOutput = {
        "",
        "'"
    };

    Check(input, expOutput);
}

TEST(Lab3, Test3) {
    std::vector<std::string> input = {};

    std::vector<std::string> expOutput = {};

    Check(input, expOutput);
}

TEST(Lab3, Test4) {
    std::vector<std::string> input = {
        "aaaa",
        "bbaabb"
    };

    std::vector<std::string> expOutput = {
        "aaaa",
        "bbaabb"
    };

    Check(input, expOutput);
}