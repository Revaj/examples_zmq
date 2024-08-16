#include <zmq.hpp>
#include <iostream>
#include <sstream>

int main(void) {

    zmq::context_t context;

    std::cout << "Collecting updates from weather server...\n\n";
    zmq::socket_t subscriber (context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556");

    const char *filter = "10001 ";
    subscriber.set(zmq::sockopt::subscribe, filter);

    int update_nbr = 0;
    long total_temp = 0;
    for(;update_nbr < 100; update_nbr++) {
        zmq::message_t update;
        int zipcode, temperature, relhumidity;

        auto result = subscriber.recv(update, zmq::recv_flags::none);

        std::istringstream iss(static_cast<char*>(update.data()));
        iss >> zipcode >> temperature >> relhumidity;

        total_temp += temperature;
    }

    std::cout << "Average temperature for zipcode '" << filter
              << "' was " << static_cast<int>(total_temp / update_nbr) << "F\n";
    return 0;
}