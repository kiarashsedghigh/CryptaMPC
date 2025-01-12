#include <thread>
#include "io/net_io.h"

void server_function(const char *address, const int port) {
    /* Create NetIO with the address and port */
    qst::io::NetIO server{address, port, true};

    /* Random buffer to send */
    char buff[50];
    server.write(buff, 50);
}


void client_function(const char *address, const int port) {
    /* Create NetIO with the address and port */
    qst::io::NetIO client{address, port, false};

    char buff[50];
    client.write(buff, 50);
}


int main() {
    /* Defining the IP address and port number for server*/
    auto server_listen_address = "127.0.0.1";
    int server_listen_port = 9500;

    /* Defining the IP address and port number for client*/
    auto client_dest_address = "127.0.0.1";
    int client_dest_port = 9500;


    /* Crate two threads */
    std::thread server_thread(server_function, server_listen_address, server_listen_port);
    std::thread client_thread(client_function, client_dest_address, client_dest_port);

    server_thread.join();
    client_thread.join();

    return 0;
}
