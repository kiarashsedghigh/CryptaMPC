#include "qst.h"

#include <thread>


static void reverse_bool_array(bool* in, size_t i, size_t j) {
    while (i < j) {
        std::swap(in[i], in[j]);
        ++i;
        --j;
    }
}


void alice(const char *address, const int port, qst::math::algstruct::Group *group) {
    /* Create NetIO with the address and port */
    qst::io::NetIO server{address, port, true};
    server.set_no_delay();

    /*
     *
     *      Input Phase
     *
     */
    qst::mpc::protocols::gc::BristolCircuit cf {"/home/kiarash/CLionProjects/QuantumSafeThreshold/src/lib/mpc/gc/core_circuits/adder64.txt"};
    auto *in = new bool[cf.m_input_lhs_size + cf.m_input_rhs_size];
    auto *out = new bool[cf.m_output_size];
    memset(in, false, cf.m_input_lhs_size + cf.m_input_rhs_size);
    memset(out, false, cf.m_output_size);

    in[0] = true;
    //
    // const qst::types::Data input{"0000000000000001", 16, qst::types::Data::DataInputType::HEX_STRING};
    // input.to_bool_array(in);
    // reverse_bool_array(in, 0,cf.m_input_lhs_size-1);
    //
    // // std::cout << ">> ";
    // for (int i = 0; i < cf.m_input_lhs_size; i++)
    //     std::cout << in[i];
    // //
    // std::cout << std::endl;

    /*
     *
     *      Protocol Execution
     *
     */
    emp::C2PC twopc {&server, 1, &cf};

    twopc.function_independent();
    server.flush();

    twopc.function_dependent();
    server.flush();

    twopc.online(in, out, true);


    /*
     *
     *      Output Phase
     *
     */
    std::string res {};
    for(int i = 0; i < cf.m_output_size; ++i)
        res += (out[i]?"1":"0");
    std::cout << res << std::endl << "\n";

    delete[] in;
    delete[] out;
}


void bob(const char *address, const int port, qst::math::algstruct::Group *group) {
    /* Create NetIO with the address and port */
    qst::io::NetIO client{address, port, false};
    client.set_no_delay();

    /*
     *
     *      Input Phase
     *
     */
    qst::mpc::protocols::gc::BristolCircuit cf {"/home/kiarash/CLionProjects/QuantumSafeThreshold/src/lib/mpc/gc/core_circuits/adder64.txt"};
    auto *in = new bool[cf.m_input_lhs_size + cf.m_input_rhs_size];
    auto *out = new bool[cf.m_output_size];
    memset(in, false, cf.m_input_lhs_size + cf.m_input_rhs_size);
    memset(out, false, cf.m_output_size);

    in[1] = true;
    in[2] = true;
    // const qst::types::Data input{"0000000000000006", 16, qst::types::Data::DataInputType::HEX_STRING};
    // input.to_bool_array(in + cf.m_input_lhs_size);
    // reverse_bool_array(in, cf.m_input_lhs_size,cf.m_input_lhs_size + cf.m_input_rhs_size-1);
    //
    // for (int i = 0; i < cf.m_input_rhs_size ; i++) {
    //     std::cout << (in + cf.m_input_lhs_size)[i];
    // }
    // std::cout << std::endl;

    /*
     *
     *      Protocol Execution
     *
     */
    emp::C2PC twopc {&client, 2, &cf};

    twopc.function_independent();
    client.flush();

    twopc.function_dependent();
    client.flush();

    twopc.online(in, out, true);



    /*
     *
     *      Output Phase
     *
     */
    // std::string res {};
    // for(int i = 0; i < cf.m_output_size; ++i)
    //     res += (out[i]?"1":"0");
    // std::cout << res << std::endl;

    delete[] in;
    delete[] out;



}




int main() {
    /* Defining the IP address and port number for server*/
    auto server_listen_address = "127.0.0.1";
    int server_listen_port = 9500;

    /* Defining the IP address and port number for client*/
    auto client_dest_address = "127.0.0.1";
    int client_dest_port = 9500;


    /* Crate two threads */
    std::thread server_thread_12(alice, server_listen_address, server_listen_port, nullptr);
    std::thread client_thread_12(bob, client_dest_address, client_dest_port, nullptr);

    server_thread_12.join();
    client_thread_12.join();




    return 0;
}
