#include "qst.h"

#include <thread>


static void reverse_bool_array(bool *in, size_t i, size_t j) {
    while (i < j) {
        std::swap(in[i], in[j]);
        ++i;
        --j;
    }
}

static std::string bool_array_to_hex(const bool* boolArray, size_t size) {
    std::stringstream hexStream;
    int bitCount = 0;
    unsigned char currentByte = 0;

    // Iterate over the bool array using the pointer
    for (size_t i = 0; i < size; ++i) {
        // Add the current bit to the byte
        currentByte = (currentByte << 1) | boolArray[i];
        bitCount++;

        // If we've processed 8 bits, convert to hex and reset
        if (bitCount == 8) {
            hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(currentByte);
            bitCount = 0;
            currentByte = 0;
        }
    }

    // Handle remaining bits (if array size is not a multiple of 8)
    if (bitCount > 0) {
        currentByte <<= (8 - bitCount); // Shift remaining bits to the left
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(currentByte);
    }

    return hexStream.str();
}


void alice(const char *address, const int port, qst::mpc::protocols::gc::BristolCircuit *cf, const bool *in) {
    /* Create NetIO with the address and port */
    qst::io::NetIO server{address, port, true};
    server.set_no_delay();

    auto *out = new bool[cf->m_output_size];
    memset(out, false, cf->m_output_size);

    /*
     *
     *      Protocol Execution
     *
     */
    emp::C2PC twopc{&server, 1, cf};

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
    reverse_bool_array(out, 0, cf->m_output_size - 1);
    std::cout << bool_array_to_hex(out, cf->m_output_size);

    delete[] out;
}


void bob(const char *address, const int port, qst::mpc::protocols::gc::BristolCircuit *cf, const bool *in) {
    /* Create NetIO with the address and port */
    qst::io::NetIO client{address, port, false};
    client.set_no_delay();

    /*
     *
     *      Input Phase
     *
     */
    auto *out = new bool[cf->m_output_size];
    memset(out, false, cf->m_output_size);

    /*
     *
     *      Protocol Execution
     *
     */
    emp::C2PC twopc{&client, 2, cf};

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
    // for(int i = 0; i < cf->m_output_size; ++i)
    //     res += (out[i]?"1":"0");
    // std::cout << res << std::endl;

    delete[] out;
}


int main() {
    /* Defining the IP address and port number for server*/
    auto server_listen_address = "127.0.0.1";
    int server_listen_port = 9500;

    /* Defining the IP address and port number for client*/
    auto client_dest_address = "127.0.0.1";
    int client_dest_port = 9500;


    /* Input to the GC */
    qst::mpc::protocols::gc::BristolCircuit cf{
        "/home/kiarash/CLionProjects/QuantumSafeThreshold/src/lib/mpc/gc/core_circuits/adder64.txt"
    };
    auto *input = new bool[cf.m_input_lhs_size + cf.m_input_rhs_size];
    memset(input, false, cf.m_input_lhs_size + cf.m_input_rhs_size);

    const qst::types::Data alice_data{"000000000000000a", 16, qst::types::Data::DataInputType::HEX_STRING};
    alice_data.to_bool_array(input + cf.m_input_lhs_size);
    reverse_bool_array(input + cf.m_input_lhs_size, 0, cf.m_input_rhs_size - 1);

    const qst::types::Data bob_data{"000000000000000a", 16, qst::types::Data::DataInputType::HEX_STRING};
    bob_data.to_bool_array(input);
    reverse_bool_array(input, 0, cf.m_input_lhs_size - 1);


    /* Crate two threads */
    std::thread server_thread_12(alice, server_listen_address, server_listen_port, &cf, input);
    std::thread client_thread_12(bob, client_dest_address, client_dest_port, &cf, input);

    server_thread_12.join();
    client_thread_12.join();


    return 0;
}
