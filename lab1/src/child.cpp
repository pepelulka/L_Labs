#include <iostream>
#include <fstream>
#include <algorithm>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("No input file.\n");
        exit(EXIT_FAILURE);
    }

    std::string filename(argv[1]);
    std::ofstream file(filename);

    for (std::string line; std::getline(std::cin, line); ) {
        std::reverse(line.begin(), line.end());
        line.push_back('\n');
        file << line;
    }

    return 0;
}