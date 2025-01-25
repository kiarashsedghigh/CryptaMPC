#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <cstdint>

namespace qst::mpc::protocols::gc {
    /**
     * @brief Enum for gates of a circuit
     */
    enum CircuitGate {
        AND_GATE,
        XOR_GATE,
        NOT_GATE,
    };

    /**
     * @brief Implementing base class for representing binary circuits
     */
    class Circuit {
    public:
        virtual ~Circuit() = default;

        /**
         * @brief Total number of gates in the circuit
         */
        std::uint32_t m_num_gates{};

        /**
         * @brief Total number of wires in the circuit
         */
        std::uint32_t m_num_wires{};

        /**
         * @brief Size of the left hand side operand in bits
         */
        std::uint32_t m_input_lhs_size{};

        /**
         * @brief Size of the left hand side operand in bits
         */
        std::uint32_t m_input_rhs_size{};

        /**
         * @brief Size of the gate's output in bits
         */
        std::uint32_t m_output_size{};
    };
}


#endif //CIRCUIT_H
