#include "manager_base.hpp"

namespace lab5 {

size_t BinTree::Size() {
    return data.size();
}

size_t BinTree::Push(size_t id) {
    data.push_back(id);
    idToInd[id] = data.size() - 1;
    return data.size() - 1;
}

size_t BinTree::GetInd(size_t id) {
    return idToInd[id];
}

size_t Dealer::makeRouteNum(size_t ind) {
    std::bitset<16> s;
    size_t size = 0;
    size_t result = 0;
    while (ind != 0) {
        s[size] = (ind % 2);
        size++;
        ind = (ind - 1) / 2;
    }
    for (size_t i = size; i > 0; i--) {
        result = result * 2 + 2 - s[i - 1];
    }
    return result;
}

void Dealer::CreateRoot(size_t id) {
    if (tree.Size() != 0) {
        throw std::logic_error("Root already exists.");
    }
    tree.Push(id);
    size_t childPort = PortGenerator::Get();
    int pid = StartComputer(pathToComputer, id, childPort, sinkPort);
    push.Connect(childPort);
    std::cout << "OK: " << pid << std::endl;
}

void Dealer::Create(size_t id) {
    if (tree.Size() == 0) {
        CreateRoot(id);
        return ;
    }
    size_t ind = tree.Push(id);
    size_t routeNum = makeRouteNum(ind);
    push.Push("create " + std::to_string(routeNum) + " " + std::to_string(id) + " " + std::to_string(PortGenerator::Get()));
}

void Dealer::Exec(size_t id, const std::vector<int>& data) {
    size_t routeNum = makeRouteNum(tree.GetInd(id));
    std::string content = "exec " + std::to_string(routeNum) + " " + std::to_string(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        content += " " + std::to_string(data[i]);
    }
    push.Push(content);
}

void Dealer::Heartbeat(size_t ms) {
    push.Push("hb " + std::to_string(ms));
}

Dealer::Dealer(zmq::context_t &context, size_t _sinkPort, const std::string& _pathToComputer) : sinkPort(_sinkPort), push(context), pathToComputer(_pathToComputer) {}

HeartBeatManager::HeartBeatManager(ThreadSafeStack& stack) : output(stack) { }

void HeartBeatManager::Routine() {
    {
        std::unique_lock<std::mutex> lk(mtx);
        if (!isActive) {
            cv.wait(lk);
        }
    }
    while (true) {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        for (auto p : nodes) {
            if (unavailable.find(p.first) != unavailable.end()) {
                continue;
            }
            Duration dif = std::chrono::duration_cast<Duration>(now - p.second);
            if (dif.count() > threshold) {
                unavailable.insert(p.first);
                output.Push("Heartbeat: node " + std::to_string(p.first) + " is unavailable now");
            }
        }
        std::this_thread::sleep_for(Duration(MILLISECONDS_TO_SLEEP));
    }
}

void HeartBeatManager::Run() {
    std::unique_lock<std::mutex> lk(mtx);
    isActive = true;
    cv.notify_all();
}

Sink::Sink(zmq::context_t &context, size_t _port, ThreadSafeStack& _stack, HeartBeatManager& _hbman) : port(_port), pull(context), stack(_stack), hbman(_hbman) {
    pull.Connect(port);
}

void Sink::Routine() {
    while (true) {
        std::string msg = pull.Pull();
        std::stringstream ss(msg);
        std::string temp;
        ss >> temp;
        if (temp == "OK.heartbeat") {
            size_t id;
            ss >> id;
            hbman.nodes[id] = std::chrono::high_resolution_clock::now();
        } else {
            stack.Push(msg);
        }
    }
}

Manager::Manager(const std::string& _pathToComputer, ThreadSafeStack& _stack) : context(1),
                                                                                sinkPort(PortGenerator::Get()),
                                                                                pathToComputer(_pathToComputer),
                                                                                dealer(context, sinkPort, pathToComputer),
                                                                                hbman(_stack),
                                                                                sink(context, sinkPort, _stack, hbman) { }

void ThreadSafeStack::Push(const std::string& msg) {
    std::lock_guard<std::mutex> lk(mtx);
    data.push_back(msg);
}

std::vector<std::string> ThreadSafeStack::Get() {
    std::lock_guard<std::mutex> lk(mtx);
    auto temp = data;
    data.clear();
    return temp;
}

Frontend::Frontend(const std::string& pathToComputer) : man(pathToComputer, results) { }

void Frontend::Routine() {
    std::thread t1(&Sink::Routine, &man.sink);
    std::thread t2(&HeartBeatManager::Routine, &man.hbman);

    std::string line;
    std::cout << "$ ";
    while (std::getline(std::cin, line)) {
        std::vector<std::string> res = results.Get();
        for (auto i : res) {
            std::cout << i << std::endl;
        }
        std::stringstream ss(line);
        std::string q, temp;
        ss >> temp;
        if (temp == "create") {
            size_t id;
            ss >> id;
            man.dealer.Create(id);
            {
                std::lock_guard<std::mutex> lk(man.hbman.nodesMtx);
                man.hbman.nodes.insert({id, std::chrono::high_resolution_clock::now()});
            }
        } else if (temp == "exec") {
            size_t id, n;
            ss >> id >> n;
            std::vector<int> data(n);
            for (size_t i = 0; i < n; i++) {
                ss >> data[i];
            }
            man.dealer.Exec(id, data);
        } else if (temp == "heartbeat") {
            size_t ms;
            ss >> ms;
            {
                std::lock_guard<std::mutex> lk(man.hbman.nodesMtx);
                for (auto& p : man.hbman.nodes) {
                    p.second = std::chrono::high_resolution_clock::now();
                }
            }
            if (!man.hbman.isActive) {
                man.hbman.Run();
            }
            man.hbman.threshold = 4 * ms;
            man.dealer.Heartbeat(ms);
            std::cout << "OK\n";
        } else if (temp != "") {
            std::cout << "Undefined command\n";
        }
        std::cout << "$ ";
    }
}

}
