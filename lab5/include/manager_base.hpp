#pragma once

#include "utils.hpp"

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <bitset>
#include <chrono>

namespace lab5 {

// Children of i's entry : 2*i + 1 and 2*i + 2
class BinTree {
private:
    std::vector<size_t> data;
    std::map<size_t, size_t> idToInd;
public:

    size_t Size();
    // Возвращает индекс в массиве data.
    size_t Push(size_t id);
    size_t GetInd(size_t id);

};

class ThreadSafeStack {
private:
    std::vector<std::string> data;
    std::mutex mtx;
public:
    void Push(const std::string& msg);
    std::vector<std::string> Get();
};

class Dealer {
private:

    BinTree tree;
    size_t sinkPort;
    PushConnection push;
    std::string pathToComputer;

    size_t makeRouteNum(size_t ind);

public:

    void CreateRoot(size_t id);

    void Create(size_t id);
    void Exec(size_t id, const std::vector<int>& data);
    void Heartbeat(size_t ms);

    Dealer(zmq::context_t &context, size_t _sinkPort, const std::string& _pathToComputer);

};

class HeartBeatManager {
public:
    using Duration = std::chrono::milliseconds;
    using TimePoint = std::chrono::high_resolution_clock::time_point;
private:
    const int64_t MILLISECONDS_TO_SLEEP = 50;
    ThreadSafeStack& output;

    std::mutex mtx;
    std::condition_variable cv;

public:
    bool isActive = false;
    std::atomic<int64_t> threshold;
    std::unordered_set<size_t> unavailable;
    std::unordered_map<size_t, TimePoint> nodes;
    std::mutex nodesMtx;

    HeartBeatManager(ThreadSafeStack& stack);
    void Routine();
    void Run();

};

class Sink {
private:

    size_t port;
    PullConnection pull;
    ThreadSafeStack &stack;
    HeartBeatManager& hbman;

public:

    Sink(zmq::context_t &context, size_t _port, ThreadSafeStack& _stack, HeartBeatManager& _hbman);

    void Routine();

};

class Manager {
private:

    zmq::context_t context;
    size_t sinkPort;
    std::string pathToComputer;

public:
    Dealer dealer;
    HeartBeatManager hbman;
    Sink sink;

    Manager(const std::string& _pathToComputer, ThreadSafeStack& stack);

};

class Frontend {
private:

    ThreadSafeStack results;
    Manager man;

public:

    Frontend(const std::string& pathToComputer);
    void Routine();

};

}
