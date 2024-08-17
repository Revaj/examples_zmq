#include <zmq.hpp>
#include <time.h>
#include <sys/time.h>
#include <iostream>

int main(int argc, char *argv[]) {
    zmq::context_t context;
    zmq::socket_t receiver(context, zmq::socket_type::pull);
    receiver.bind("tcp://*:5558");

    zmq::message_t message;
    auto result = receiver.recv(message, zmq::recv_flags::none);

    struct timeval tstart;
    gettimeofday(&tstart, NULL);

    int task_nbr;
    int total_msec = 0;
    for (task_nbr = 0; task_nbr < 100; task_nbr++) {
        auto tmp = receiver.recv(message, zmq::recv_flags::none);
        if (task_nbr % 10 == 0) {
            std::cout << ":" << std::flush;
        }
        else {
            std::cout << "." << std::flush;
        }
    }

    struct timeval tend, tdiff;
    gettimeofday(&tend, NULL);

    if (tend.tv_usec < tstart.tv_usec) {
        tdiff.tv_sec = tend.tv_sec - tstart.tv_sec - 1;
        tdiff.tv_usec = 1000000 + tend.tv_usec - tstart.tv_usec;
    } else {
        tdiff.tv_sec = tend.tv_sec - tstart.tv_sec;
        tdiff.tv_usec = tend.tv_usec - tstart.tv_usec;
    }

    total_msec = tdiff.tv_sec * 1000 + tdiff.tv_usec / 1000;
    std::cout << "\nTotal elapsed time: " << total_msec << " msec\n\n";
    return 0;
}