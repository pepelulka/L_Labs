#pragma once

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <mutex>

#include <zmq.hpp>

namespace lab5 {

class PortGenerator {
private:

    inline static const size_t lower = 5555, upper = 5600;
    inline static size_t current = 5555;

public:

    PortGenerator() = delete;
    static size_t Get();

};

class PullConnection {
private:

    zmq::socket_t socket;
    size_t port;

public:
    bool isActive = false;

    void Connect(size_t port);
    std::string Pull();

    PullConnection(zmq::context_t& context);

};

class PushConnection {
private:

    zmq::socket_t socket;
    size_t port;
    std::mutex mtx;

public:
    bool isActive = false;

    void Connect(size_t port);
    void Push(const std::string& content);

    PushConnection(zmq::context_t& context);

};

int StartComputer(const std::string& pathToComputerProgram, size_t id, size_t port, size_t parentPort);

}
