#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>

int main(void) {

    using namespace std::chrono_literals;
    zmq::context_t context;
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while(true) {
        zmq::message_t request;

        auto result = socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received Hello\n";

        std::this_thread::sleep_for(1000ms);

        zmq::message_t reply(5);
        memcpy(static_cast<void*>(reply.data()), "World", 5);
        socket.send(reply, zmq::send_flags::dontwait);
    }
    return 0;
}