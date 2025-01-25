#ifndef BRISTOL_CIRCUIT_H
#define BRISTOL_CIRCUIT_H

#include "circuit.h"
#include "types.h"
#include <vector>


namespace qst::mpc::protocols::gc {
    /**
     * @brief Implementing Bristol class for bristol representation of the circuit
     */
    class BristolCircuit final : public Circuit {
    public:
        BristolCircuit() = default;

        /**
         * @brief Loads the circuit from the given file
         * @param circuit_file Pointer to the name of the file
         */
        explicit BristolCircuit(const char *circuit_file);

        /**
         * @brief Vector of gates in the circuit
         */
        std::vector<std::uint32_t> m_gates_vector;

        /**
         * @brief Vector of wires (labels) each of type QST::Data
         */
        std::vector<types::Data> m_wires_vector;
    };
}


#endif
