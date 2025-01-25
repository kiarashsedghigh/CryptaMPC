#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <iostream>
#include <vector>

namespace qst::types {
    typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));
    using block = __m128i;

    /**
     * @brief Compact data type for 16 byte number. This type in EMP-Toolkit is called block.
     */
    typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));

    /**
     * @brief Generic class for abstracting any data type
     */
    class Data {
    public:

        enum class DataInputType {
            PLAIN_BYTE,
            HEX_STRING,
        };

        Data() = default;

        // explicit Data(const char *input, std::size_t len);

        explicit Data(const char *input, std::size_t input_length, DataInputType type = DataInputType::PLAIN_BYTE);

        explicit Data(const __m128i &data);

        Data(Data &&other) noexcept;

        Data& operator=(const Data&) = default; //todo this has a note

        /**
         * @brief Returns number of bytes of the current data
         * @return Number of bytes of the current data
         */
        [[nodiscard]] std::size_t get_size() const;

        /**
         * @brief Writes (serialize) the byte of the current data to destination
         * @param dest Pointer to the destination to which we write the bytes
         */
        void to_bytes(void *dest) const;

        /**
         * @brief Writes the hex string representation of the bytes into the buffer
         * @param dest Pointer to the destination buffer to which we want to write the string
         */
        void to_hex(void *dest) const;


        /**
         * @brief Converts the internal byte array to bool array
         * @param output Pointer to the bool array
         */
        void to_bool_array(bool * output) const;

        /**
         * @brief Returns the hex string representation of the bytes as a string object
         * @return Hex string representation of the bytes as a string object
         */
        [[nodiscard]] std::string as_hex_string() const;


        /**
         * @brief Returns the string representation of the bytes as a string object
         * @return String representation of the bytes as a string object
         */
        [[nodiscard]] std::string as_string() const;


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


        /**
         * @brief Return the internal data as a single __m128i data type. Only the first 16 Bytes is returned
         * @return __m128i data value of the current data
         */
        [[nodiscard]] __m128i as_m128i() const;

    private:
        std::vector<std::uint8_t> m_bytes{};
    };
}

#endif
