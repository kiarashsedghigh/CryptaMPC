#include "mpc/protocols/gc/circuits/bristol_circuit.h"

#include <fstream>

namespace qst::mpc::protocols::gc {
    BristolCircuit::BristolCircuit(const char* circuit_file) {
        FILE *fh = fopen(circuit_file, "r");

        fscanf(fh, "%d%d\n", &m_num_gates, &m_num_wires);
        fscanf(fh, "%d%d%d\n", &m_input_lhs_size, &m_input_rhs_size, &m_output_size);
        fscanf(fh, "\n");

        /* Gate's vector is num_gates * tuple_of_four_elements -> (input1_wire, input2_wire, output_wire, gate_type) */
        m_gates.resize(m_num_gates * 4);
        m_wires.resize(m_num_wires);

        for (int i = 0; i < m_num_gates; ++i) {
            char gate_name[10];
            int gate_input_size{};

            fscanf(fh, "%d", &gate_input_size);
            if (gate_input_size == 2) {
                fscanf(fh, "%d%d%d%d%s", &gate_input_size, &m_gates[4 * i], &m_gates[4 * i + 1], &m_gates[4 * i + 2], gate_name);
                if (gate_name[0] == 'A') m_gates[4 * i + 3] = AND_GATE;
                else if (gate_name[0] == 'X') m_gates[4 * i + 3] = XOR_GATE;
            } else if (gate_input_size == 1) {
                fscanf(fh, "%d%d%d%s", &gate_input_size, &m_gates[4 * i], &m_gates[4 * i + 2], gate_name);
                m_gates[4 * i + 3] = NOT_GATE;
            }
        }
    }


    void BristolCircuit::evaluate() {
        std::cout << m_num_gates << std::endl;
    }
}
