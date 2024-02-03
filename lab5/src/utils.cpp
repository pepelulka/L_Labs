#include "utils.hpp"

#include <unistd.h>

namespace lab5 {

size_t PortGenerator::Get() {
    if (current > upper) {
        throw std::logic_error("There're no availible ports.");
    }
    current++;
    return current - 1;
}

void PullConnection::Connect(size_t port) {
    socket.bind("tcp://*:" + std::to_string(port));
    isActive = true;
}

std::string PullConnection::Pull() {
    if (!isActive) {
        throw std::logic_error("Connection is not active");
    }
    zmq::message_t msg;
    auto ret = socket.recv(msg, zmq::recv_flags::none);
    if (ret.has_value() && (EAGAIN == ret.value())) {
        throw std::logic_error("Something bad... (PULL SOCKET) pid : " + std::to_string(getpid()));
    }
    std::string content((char*)msg.data(), (char*)msg.data() + msg.size());
    return content;
}

PullConnection::PullConnection(zmq::context_t& context) : socket(context, zmq::socket_type::pull) { }

void PushConnection::Connect(size_t port) {
    socket.connect("tcp://localhost:" + std::to_string(port));
    isActive = true;
}

void PushConnection::Push(const std::string& content) {
    if (!isActive) {
        throw std::logic_error("Connection is not active");
    }
    zmq::message_t msg(content);
    {
        std::lock_guard<std::mutex> lk(mtx);
        socket.send(msg, zmq::send_flags::none);
    }
}

PushConnection::PushConnection(zmq::context_t& context) : socket(context, zmq::socket_type::push) { }

int StartComputer(const std::string& pathToComputerProgram, size_t id, size_t port, size_t parentPort) {
    int pid = fork();
    if (pid == -1) {
        std::cout << "Can't fork!";
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        execl(
            pathToComputerProgram.c_str(),
            pathToComputerProgram.c_str(),
            std::to_string(id).c_str(),
            std::to_string(port).c_str(),
            std::to_string(parentPort).c_str(),
            nullptr
        );
    }
    return pid;
}

}


