#include "types.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace qst::types {
    Data::Data(const __m128i &data) {
        const auto *ptr = reinterpret_cast<const std::int8_t *>(&data);
        for (size_t i = 0; i < 16; ++i)
            m_bytes.push_back(ptr[i]);
    }

    Data::Data(const int8_t *input, const std::size_t len) {
        for (size_t i = 0; i < len; ++i)
            m_bytes.push_back(input[i]);
    }

    Data::Data(const std::string &input) {
        for (const auto &c: input)
            m_bytes.push_back(c);
    }

    Data::Data(const char *input, const std::size_t len) {
        for (int i = 0; i < len; ++i)
            m_bytes.push_back(input[i]);
    }

    std::uint64_t Data::get_size() const {
        return m_bytes.size();
    }

    void Data::to_bytes(void *dest) {
        auto *byte_dest = static_cast<std::int8_t *>(dest);
        for (const auto byte: m_bytes)
            *byte_dest++ = byte;
    }

    void Data::to_hex(void *dest) const {
        int i{};
        for (const auto byte: m_bytes) {
            sprintf(static_cast<char *>(dest) + i * 2, "%02x", static_cast<unsigned char>(byte));
            i++;
        }
    }

    std::string Data::as_hex_string() const {
        std::ostringstream hex_stream;

        for (const auto byte: m_bytes) {
            hex_stream << std::setw(2) << static_cast<int>(byte);
        }
        return hex_stream.str();
    }

    void Data::print_as_hex() const {
        for (const auto byte: m_bytes)
            std::cout << std::setw(2) << static_cast<int>(byte);
        std::cout << std::endl;
    }

    void Data::print_string() const {
        for (const auto byte: m_bytes)
            std::cout << byte;
        std::cout << std::endl;
    }

    Data Data::operator^(const Data &other) const {
        if (m_bytes.size() != other.m_bytes.size())
            throw std::runtime_error("[TYPES]: Data::operator^: Size mismatch");
        Data result;

        const std::size_t num_of_bytes{m_bytes.size()};

        for (std::size_t i = 0; i < num_of_bytes; ++i)
            result.m_bytes.push_back(m_bytes[i] ^ other.m_bytes[i]);

        return result;
    }
}
