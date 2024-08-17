#include <zmq.hpp>
#include <iostream>
#include <string>

void s_sleep (int msecs)
{
    struct timespec t;
    t.tv_sec = msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
}

int main (int argc, char*argv[]) {
    zmq::context_t context;

    zmq::socket_t receiver(context, zmq::socket_type::pull);
    receiver.connect("tcp://localhost:5557");

    zmq::socket_t sender(context, zmq::socket_type::push);
    sender.connect("tcp://localhost:5558");

    while(true) {
        zmq::message_t message;
        int workload;

        auto result = receiver.recv(message,  zmq::recv_flags::none);
        std::string smessage(static_cast<char*>(message.data()), message.size());

        std::istringstream iss(smessage);
        iss >> workload;

        s_sleep(workload);

        message.rebuild();
        sender.send(message, zmq::send_flags::none);
        std::cout << "." << std::flush;
    }
    return 0;
}