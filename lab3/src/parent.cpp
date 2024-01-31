#include "parent.hpp"

void ParentRoutine(const char *pathToChild, std::istream &in) {
    std::string filename1, filename2;
    std::getline(in, filename1);
    std::getline(in, filename2);

    std::string childPath(pathToChild);

    // Dealing with shm

    int shm1_fd = shm_open(SHM1_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm1_fd, SHM_SIZE);
    char* ptr1 = (char*)mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm1_fd, 0);

    int shm2_fd = shm_open(SHM2_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm2_fd, SHM_SIZE);
    char* ptr2 = (char*)mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm2_fd, 0);

    for (std::string line; std::getline(in, line); ) {
        if (Probability(80)) {
            line.push_back('\n');
            size_t bytes = line.size();
            std::memcpy(ptr1, line.c_str(), bytes);
            ptr1 += bytes;
        } else {
            line.push_back('\n');
            size_t bytes = line.size();
            std::memcpy(ptr2, line.c_str(), bytes);
            ptr2 += bytes;
        }
    }
    ptr1[0] = '\0';
    ptr2[0] = '\0';

    //

    int pid1 = fork();
    if (pid1 < 0){
        perror("fork failed");
        return;
    } else if (pid1 > 0) {
        int pid2 = fork();

        if (pid2 < 0) {
            perror("fork failed");
            return;
        } else if (pid2 == 0) {
            execl(childPath.c_str(), childPath.c_str(), filename2.c_str(), SHM2_NAME, nullptr);
        }
    } else {
        execl(childPath.c_str(), childPath.c_str(), filename1.c_str(), SHM1_NAME, nullptr);
    }

    wait(nullptr);
    wait(nullptr);

    shm_unlink(SHM1_NAME);
    shm_unlink(SHM2_NAME);
}