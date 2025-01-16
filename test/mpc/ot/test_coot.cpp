#include <thread>
#include "io/net_io.h"
#include "math/group.h"
#include "mpc/protocols/ot/coot.h"

void npot_one_n_server(const char *address, const int port, qst::math::algstruct::Group *group) {
    /* Create NetIO with the address and port */
    const qst::io::NetIO server{address, port, true};

    /* Create NPOT object */
    qst::mpc::protocols::COOT<qst::io::NetIO> coot{server, group, 16};

    /* Generate 4 data samples (tuples): [('a','A'), ('b','B'), ('c','C'), ('d','D')] */
    const auto lower_case_letters = new qst::types::Data[4]{
        qst::types::Data{"aaaaaaaaaaaaaaaa", 16},
        qst::types::Data{"bbbbbbbbbbbbbbbb", 16},
        qst::types::Data{"cccccccccccccccc", 16},
        qst::types::Data{"dddddddddddddddd", 16},
    };

    const auto upper_case_letters = new qst::types::Data[4]{
        qst::types::Data{"AAAAAAAAAAAAAAAA", 16},
        qst::types::Data{"BBBBBBBBBBBBBBBB", 16},
        qst::types::Data{"CCCCCCCCCCCCCCCC", 16},
        qst::types::Data{"DDDDDDDDDDDDDDDD", 16},
    };

    /* Send */
    coot.send_n(lower_case_letters, upper_case_letters, 4);
}


void npot_one_n_client(const char *address, const int port, qst::math::algstruct::Group *group) {
    /* Create NetIO with the address and port */
    const qst::io::NetIO client{address, port, false};

    qst::mpc::protocols::COOT<qst::io::NetIO> coot{client, group, 16};

    /* List of choices */
    const auto choices = new bool[4];
    choices[0] = true;
    choices[1] = false;
    choices[2] = false;
    choices[3] = true;


    /* List of letters retrieved from the OT */
    const auto letters = new qst::types::Data[4];

    /* Recv */
    coot.recv_n(letters, choices, 4);
    for (int i = 0; i < 4; i++)
        letters[i].print_string();
}


void npot_one_two_server(const char *address, const int port, qst::math::algstruct::Group *group) {
    /* Create NetIO with the address and port */
    const qst::io::NetIO server{address, port, true};

    /* Create NPOT object */
    qst::mpc::protocols::COOT<qst::io::NetIO> coot{server, group, 16};

    /* Generate 2 data samples */
    qst::types::Data data1{"aaaaaaaaaaaaaaaa", 16}, data2{"bbbbbbbbbbbbbbbb", 16};

    /* Send */
    coot.send(data1, data2);
}

void npot_one_two_client(const char *address, const int port, qst::math::algstruct::Group *group) {
    /* Create NetIO with the address and port */
    const qst::io::NetIO server{address, port, false};

    /* Create NPOT object */
    qst::mpc::protocols::COOT<qst::io::NetIO> coot{server, group, 16};

    /* Recv */
    constexpr bool choice{true};
    const qst::types::Data data{coot.recv(choice)};

    data.print_string();
}


int main() {
    /* Defining the IP address and port number for server*/
    auto server_listen_address = "127.0.0.1";
    int server_listen_port = 9500;

    /* Defining the IP address and port number for client*/
    auto client_dest_address = "127.0.0.1";
    int client_dest_port = 9500;


    /* Test 1-2 OT */
    /* Crate two threads */
    std::thread server_thread_12(npot_one_two_server, server_listen_address, server_listen_port, nullptr);
    std::thread client_thread_12(npot_one_two_client, client_dest_address, client_dest_port, nullptr);

    server_thread_12.join();
    client_thread_12.join();


    /* Test N-N OT */
    /* Crate two threads */
    std::thread server_thread_nn(npot_one_n_server, server_listen_address, server_listen_port, nullptr);
    std::thread client_thread_nn(npot_one_n_client, client_dest_address, client_dest_port, nullptr);

    server_thread_nn.join();
    client_thread_nn.join();

    return 0;
}
