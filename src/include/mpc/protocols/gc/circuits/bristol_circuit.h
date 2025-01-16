#ifndef BRISTOL_CIRCUIT_H
#define BRISTOL_CIRCUIT_H

#include <vector>

#include "circuit.h"
#include "types.h"


namespace qst::mpc::protocols::gc {
    /**
     * @brief Implementing Bristol class for bristol representation of the circuit
     */
    class BristolCircuit final : public Circuit {
    public:
        // BristolCircuit(const std::uint32_t num_gates, const std::uint32_t num_wires,
        //                const std::uint32_t input_lhs_size, const std::uint32_t input_rhs_size,
        //                const std::uint32_t output_size){
        //     m_num_gates = num_gates;
        //     m_num_wires = num_wires;
        //     m_input_lhs_size = input_lhs_size;
        //     m_input_rhs_size = input_rhs_size;
        //     m_output_size = output_size;
        // } // todo constructor to parent?
        BristolCircuit() = default;

        explicit BristolCircuit(const char* circuit_file);

        void evaluate() override;

    private:
        /**
         * @brief Vector of gates in the circuit
         */
        std::vector<std::uint32_t> m_gates;

        /**
         * @brief Vector of wires (labels) each of type QST::Data
         */
        std::vector<types::Data> m_wires;
    };
}


#endif
