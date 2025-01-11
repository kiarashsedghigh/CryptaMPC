#include <iostream>
#include <vector>
#include <thread>
#include "io/net_io.h"

// Assuming there are functions to run server and client I/O operations
void server_function(const char* address, int port) {
    qst::io::NetIO server{nullptr, 5000};

    // Add server handling code here
    std::cout << "Server running on port 5000" << std::endl;
    while (true) {
        // Server loop
    }
}

void client_function(const char* address, int port) {
    qst::io::NetIO client{address, port};

    // Add client handling code here
    std::cout << "Client connecting to 127.0.0.1 on port 4000" << std::endl;
    while (true) {
        // Client loop
    }
}


int main() {

    const char * server_address = "127.0.0.1";
    int server_port = 5000;

    const char * client_address = "127.0.0.1";
    // int client_port = 4000;

    // Launch server and client in separate threads
    std::thread server_thread(server_function, server_address, server_port);
    // std::thread client_thread(client_function, client_address, server_port);
    qst::io::NetIO client{client_address, server_port};

    server_thread.join();
    // client_thread.join();

    return 0;
}
