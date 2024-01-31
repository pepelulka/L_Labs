#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "utils.hpp"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Missing arguments.\n");
        exit(EXIT_FAILURE);
    }

    std::string filename(argv[1]);
    std::string shmName(argv[2]);
    std::ofstream file(filename);

    int shm_fd = shm_open(shmName.c_str(), O_RDONLY, 0666);
    char* ptr = (char*)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    std::string content(ptr);
    std::stringstream ss(content);

    for (std::string line; std::getline(ss, line); ) {
        std::reverse(line.begin(), line.end());
        line.push_back('\n');
        file << line;
    }

    return 0;
}