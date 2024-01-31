#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

#include "utils.hpp"

constexpr int PIPE_READ = 0;
constexpr int PIPE_WRITE = 1;

void ParentRoutine(const char *pathToChild, std::istream &in);