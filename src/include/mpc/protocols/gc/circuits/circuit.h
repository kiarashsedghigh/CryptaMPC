#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <cstdint>

namespace qst::mpc::protocols::gc {
    /**
     * @brief Implementing base class for representing binary circuits
     */
    class Circuit {
    public:
        virtual ~Circuit() = default;

        virtual void evaluate() = 0;

        static constexpr int AND_GATE{0};
        static constexpr int XOR_GATE{1};
        static constexpr int NOT_GATE{2};

    protected:
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
