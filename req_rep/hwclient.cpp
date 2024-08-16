#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>

int main(void) {

    using namespace std::chrono_literals;
    zmq::context_t context;
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    for(int request_nbr = 1; request_nbr <= 10; request_nbr++) {
        zmq::message_t request(5);
        memcpy(static_cast<void*>(request.data()), "Hello", 5);
        std::cout << "Sending Hello " << request_nbr << "...\n";
        socket.send(request, zmq::send_flags::dontwait);


        zmq::message_t reply(5);
        auto rec = socket.recv(reply, zmq::recv_flags::none);
        std::cout << "Received World " << request_nbr << "\n";
        
        std::this_thread::sleep_for(1000ms);
    }

    return 0;
}