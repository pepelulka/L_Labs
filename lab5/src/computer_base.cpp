#include "computer_base.hpp"

namespace lab5 {

HeartBeatGenerator::HeartBeatGenerator(PushConnection& _push, size_t _id) : push(_push), id(_id) { }

void HeartBeatGenerator::Routine() {
    {
        std::unique_lock<std::mutex> lk(mtx);
        if (!isActive) {
            cv.wait(lk);
        }
    }
    while (true) {
        push.Push("OK.heartbeat " + std::to_string(id));
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}

void HeartBeatGenerator::Run() {
    std::unique_lock<std::mutex> lk(mtx);
    isActive = true;
    cv.notify_all();
}

Computer::Computer(size_t _id, size_t _port, size_t parentPort, const std::string& _pathToComputer) :
                                        id(_id),
                                        port(_port),
                                        context(1),
                                        pull(context),
                                        up(context),
                                        left(context),
                                        right(context),
                                        pathToComputer(_pathToComputer),
                                        hbgen(up, id) {
    pull.Connect(port);
    up.Connect(parentPort);
}

void Computer::Redirect(const std::string &msg) {
    std::stringstream source(msg);
    std::stringstream result;
    std::string temp;
    source >> temp;
    result << temp << " ";
    size_t routeNum;
    source >> routeNum;
    size_t newRouteNum = (routeNum % 2 == 0 ? routeNum / 2 - 1 : routeNum / 2);
    result << newRouteNum;
    while (source) {
        source >> temp;
        result << " " << temp;
    }
    if (routeNum % 2 == 1) {
        left.Push(result.str());
    } else {
        right.Push(result.str());
    }
}

void Computer::Routine() {
    std::thread t(&HeartBeatGenerator::Routine, &hbgen);
    while (true) {
        std::string msg = pull.Pull();
        std::stringstream ss(msg);
        std::string str;
        ss >> str;
        if (str[0] == 'O' && str[1] == 'K') {
            up.Push(msg);
        } else if (str == "create") {
            size_t routeNum;
            ss >> routeNum;
            if (routeNum <= 2) {
                size_t id, childPort;
                ss >> id >> childPort;
                int pid = StartComputer(pathToComputer, id, childPort, port);
                if (routeNum == 1) {
                    left.Connect(childPort);
                    if (hbgen.isActive) {
                        left.Push("hb " + std::to_string(hbgen.time));
                    }
                } else {
                    right.Connect(childPort);
                    if (hbgen.isActive) {
                        right.Push("hb " + std::to_string(hbgen.time));
                    }
                }
                up.Push("OK: " + std::to_string(pid));
            } else {
                Redirect(msg);
            }
        } else if (str == "exec") {
            size_t routeNum;
            ss >> routeNum;
            if (routeNum == 0) {
                size_t n;
                ss >> n;
                int sum = 0;
                for (size_t i = 0; i < n; i++) {
                    int val;
                    ss >> val;
                    sum += val;
                }
                up.Push("OK:" + std::to_string(id) + ": " + std::to_string(sum));
            } else {
                Redirect(msg);
            }
        } else if (str == "hb") {
            size_t time;
            ss >> time;
            hbgen.time = time;
            hbgen.Run();
            if (left.isActive) {
                left.Push(msg);
            }
            if (right.isActive) {
                right.Push(msg);
            }
        }
    }
    t.join();
}

}
