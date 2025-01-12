#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

namespace qst::types {
    /**
     * @brief Compact data type for 16 byte number. This type in EMP-Toolkit is called block.
     */
    typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));

    /**
     * @brief Generic class for abstracting any data type
     */
    class Data {
    public:
        Data() = default;

        explicit Data(const __m128i &data);

        Data(const std::int8_t *input, std::size_t len);

        explicit Data(const std::string &input);

        Data(const char *input, std::size_t len);


        /**
         * @brief Returns number of bytes of the current data
         * @return Number of bytes of the current data
         */
        std::uint64_t get_size() const;

        /**
         * @brief Writes (serialize) the byte of the current data to destination
         * @param dest Pointer to the destination to which we write the bytes
         */
        void to_bytes(void *dest);

        /**
         * @brief Writes the hex string representation of the bytes into the buffer
         * @param dest Pointer to the destination buffer to which we want to write the string
         */
        void to_hex(void *dest) const;

        /**
         * @brief Returns the hex string representation of the bytes as a string object
         * @return Hex string representation of the bytes as a string object
         */
        std::string as_hex_string() const;

        /**
         * @brief Prints the bytes as hex string
         */
        void print_as_hex() const;

        /**
         * @brief Prints the bytes as they are
         */
        void print_string() const;

        /**
         * XOR operator of two data types
         * @param other Other data object
         * @return New Data object containing the XOR of the two inputs
         */
        Data operator^(const Data &other) const;

    private:
        std::vector<std::int8_t> m_bytes{};
    };
}

#endif
