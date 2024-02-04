#pragma once

#include <iostream>
#include <sstream>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "utils.hpp"

namespace lab5 {

class HeartBeatGenerator {
private:
    PushConnection& push;
    size_t id;
    std::mutex mtx;
    std::condition_variable cv;
public:
    bool isActive = false;
    std::atomic<int64_t> time;

    HeartBeatGenerator(PushConnection& _push, size_t _id);

    void Routine();
    void Run();
};

class Computer {
private:

    size_t id, port;
    zmq::context_t context;
    PullConnection pull;
    PushConnection up, left, right;
    std::string pathToComputer;
    HeartBeatGenerator hbgen;

public:

    Computer(size_t _id, size_t _port, size_t parentPort, const std::string& _pathToComputer);

    void Redirect(const std::string &msg);
    void Routine();

};

}