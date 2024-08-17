#include <zmq.hpp>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#define within(num) (int) ((float) num * random() / (RAND_MAX + 1.0))

int main (int argc, char *argv[]) {
    zmq::context_t context;

    zmq::socket_t sender(context, zmq::socket_type::push);
    sender.bind("tcp://*:5557");

    std::cout << "Press Enter when the workers are ready: \n";
    getchar();
    std::cout << "Sending tasks to workers...\n\n";

    zmq::socket_t sink (context, zmq::socket_type::push);
    sink.connect("tcp://localhost:5558");
    zmq::message_t message(2);
    memcpy(message.data(), "0", 1);
    sink.send(message, zmq::send_flags::none);

    srandom(static_cast<unsigned>(time(NULL)));

    int task_nbr;
    int total_msec = 0;
    for (task_nbr = 0; task_nbr < 100; task_nbr++) {
        int workload;

        workload = within(100) + 1;
        total_msec += workload;

        message.rebuild(10);
        memset(message.data(), '\0', 10);
        snprintf(static_cast<char*>(message.data()), 12, "%d", workload);
        sender.send(message, zmq::send_flags::none);
    }

    std::cout << "Total expected cost: " << total_msec << " msec\n";
    sleep(1);

    return 0;
}