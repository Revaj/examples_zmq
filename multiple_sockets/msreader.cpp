#include <zmq.hpp>

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

    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556");
    subscriber.set(zmq::sockopt::subscribe, "10001 ");

    while(true) {
        bool rc;
        do {
            zmq::message_t task;
            if (receiver.recv(task, zmq::recv_flags::dontwait)) {
                //process task
            }
        } while(rc == true);

        do {
            zmq::message_t update;
            if (subscriber.recv(update, zmq::recv_flags::dontwait)) {
                //process weather update
            }
        } while(rc == true);

        s_sleep(1);
    }

    return 0;

}