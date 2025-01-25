#include "mpc/protocols/gc/circuits/bristol_circuit.h"
#include <fstream>

namespace qst::mpc::protocols::gc {
    BristolCircuit::BristolCircuit(const char *circuit_file) {
        FILE *fh = fopen(circuit_file, "r");
        if (!fh)
            throw std::runtime_error("[BRISTOL CIRCUIT] Could not open circuit file");

        /* Modern Bristol Format has number of inputs/outputs at the beginning of the second and third line. We need
         * to skip this with a variable.
         */
        int bd_temp{};

        fscanf(fh, "%d%d\n%d%d%d\n%d%d\n", &m_num_gates, &m_num_wires, &bd_temp, &m_input_lhs_size,
               &m_input_rhs_size, &bd_temp, &m_output_size);

        /* Gate's vector is num_gates * tuple_of_four_elements -> (input1_wire, input2_wire, output_wire, gate_type) */
        m_gates_vector.resize(m_num_gates * 4);
        m_wires_vector.resize(m_num_wires);

        for (int i = 0; i < m_num_gates; ++i) {
            char gate_name[10];
            int gate_input_size{};

            fscanf(fh, "%d", &gate_input_size);
            if (gate_input_size == 2) {
                fscanf(fh, "%d%d%d%d%s", &gate_input_size, &m_gates_vector[4 * i], &m_gates_vector[4 * i + 1], &m_gates_vector[4 * i + 2],
                       gate_name);
                if (gate_name[0] == 'A') m_gates_vector[4 * i + 3] = AND_GATE;
                else if (gate_name[0] == 'X') m_gates_vector[4 * i + 3] = XOR_GATE;
            } else if (gate_input_size == 1) {
                fscanf(fh, "%d%d%d%s", &gate_input_size, &m_gates_vector[4 * i], &m_gates_vector[4 * i + 2], gate_name);
                m_gates_vector[4 * i + 3] = NOT_GATE;
            }
        }
        fclose(fh);
    }
}
