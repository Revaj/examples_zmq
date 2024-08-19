#include <zmq.hpp>

int main(int argc, char *argv[]) {
    zmq::context_t context;

    zmq::socket_t receiver(context, zmq::socket_type::pull);
    receiver.connect("tcp://localhost:5557");

    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556");
    subscriber.set(zmq::sockopt::subscribe, "10001 ");

    zmq::pollitem_t items[] = {
        { receiver, 0, ZMQ_POLLIN, 0},
        { subscriber, 0, ZMQ_POLLIN, 0}
    };

    while(true) {
        zmq::message_t message;
        zmq::poll (&items[0], 2);// -1);
        if (items[0].revents & ZMQ_POLLIN) {
            auto result = subscriber.recv(message);
        }
    }
    return 0;

}