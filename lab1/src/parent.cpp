#include "parent.hpp"

void ParentRoutine(const char *pathToChild, std::istream &in) {
    std::string filename1, filename2;
    std::getline(in, filename1);
    std::getline(in, filename2);

    int pipe1_fd[2];

    pipe(pipe1_fd);

    std::string childPath(pathToChild);

    int pid1 = fork();

    if (pid1 < 0){
        perror("fork failed");
        return;
    } else if (pid1 > 0) {
        close(pipe1_fd[PIPE_READ]);
        int pipe2_fd[2];
        pipe(pipe2_fd);
        int pid2 = fork();

        if (pid2 < 0) {
            perror("fork failed");
            return;
        } else if (pid2 > 0) {
            close(pipe1_fd[PIPE_READ]);
            close(pipe2_fd[PIPE_READ]);
            for (std::string line; std::getline(in, line); ) {
                if (Probability(80)) {
                    line.push_back('\n');
                    write(pipe1_fd[PIPE_WRITE], line.c_str(), line.size());
                } else {
                    line.push_back('\n');
                    write(pipe2_fd[PIPE_WRITE], line.c_str(), line.size());
                }
            }

            close(pipe1_fd[PIPE_WRITE]);
            close(pipe2_fd[PIPE_WRITE]);
        } else {
            close(pipe2_fd[PIPE_WRITE]);

            if (dup2(pipe2_fd[0], STDIN_FILENO) == -1) {
                perror("dup2 error");
                return;
            }

            close(pipe2_fd[0]);
            execl(childPath.c_str(), childPath.c_str(), filename2.c_str(), nullptr);
        }
    } else {
        close(pipe1_fd[1]);

        if (dup2(pipe1_fd[0], STDIN_FILENO) == -1) {
            perror("dup2 error");
            return;
        }

        close(pipe1_fd[0]);
        execl(childPath.c_str(), childPath.c_str(), filename1.c_str(), nullptr);
    }

    wait(nullptr);
    wait(nullptr);
}