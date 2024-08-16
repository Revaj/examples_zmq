#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <string>

#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))

int main(void) {

    zmq::context_t context;
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5556");
    publisher.bind("ipc://weather.ipc");

    srandom(static_cast<unsigned>(time(NULL)));
    while(true) {
        int zipcode, temperature, relhumidity;

        zipcode     = within (100000);
        temperature = within (215) - 80;
        relhumidity = within (50) + 10;

        zmq::message_t message(20);
        snprintf(static_cast<char*>(message.data()), 36, "%05d %d %d",
                 zipcode, temperature, relhumidity);
        publisher.send(message, zmq::send_flags::none);
    }
    return 0;
}