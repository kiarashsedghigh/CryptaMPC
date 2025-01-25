#include "types.h"
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace qst::types {
    Data::Data(const __m128i &data) {
        m_bytes.assign(reinterpret_cast<const std::uint8_t *>(&data),
                       reinterpret_cast<const std::uint8_t *>(&data) + 16);
    }

    Data::Data(Data &&other) noexcept
        : m_bytes(std::move(other.m_bytes)) {
    }

    // Data::Data(const char *input, const std::size_t len) {
    //     m_bytes.resize(len);
    //     std::memcpy(m_bytes.data(), input, len);
    // }

    Data::Data(const char *input, const std::size_t input_length,  const DataInputType type){
        if (type == DataInputType::HEX_STRING) {
            const std::string hex_str {input};

            if (input_length % 2 != 0)
                throw std::invalid_argument("HEX string must have an even length.");

            m_bytes.reserve(input_length / 2);

            for (size_t i = 0; i < input_length; i += 2) {
                unsigned int byte {};
                if (sscanf(hex_str.c_str() + i, "%2x", &byte) != 1)
                    throw std::invalid_argument("Invalid HEX string.");
                m_bytes.push_back(static_cast<std::uint8_t>(byte));
            }
        }else {
            m_bytes.resize(input_length);
            std::memcpy(m_bytes.data(), input, input_length);
        }
    }

    std::size_t Data::get_size() const {
        return m_bytes.size();
    }

    void Data::to_bytes(void *dest) const {
        std::memcpy(dest, m_bytes.data(), m_bytes.size());
    }


    void Data::to_bool_array(bool * output) const {
        const std::size_t size {m_bytes.size()};
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < 8; ++j)
                output[i * 8 + j] = ((m_bytes[i] >> (7 - j)) & 1);
        }
    }

    void Data::to_hex(void *dest) const {
        char *dest_ptr = static_cast<char *>(dest);
        for (std::size_t i = 0; i < m_bytes.size(); ++i) {
            std::sprintf(dest_ptr + i * 2, "%02x", static_cast<unsigned int>(m_bytes[i]));
        }
    }

    std::string Data::as_hex_string() const {
        std::ostringstream hex_stream;
        hex_stream.fill('0');
        hex_stream << std::hex;
        for (const auto byte: m_bytes) {
            hex_stream << std::setw(2) << static_cast<unsigned int>(byte);
        }
        return hex_stream.str();
    }

    std::string Data::as_string() const {
        return {m_bytes.begin(), m_bytes.end()};
    }

    void Data::print_as_hex() const {
        for (const auto byte: m_bytes)
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(byte);
    }

    void Data::print_string() const {
        for (const auto byte: m_bytes)
            std::cout << byte;
    }

    Data Data::operator^(const Data &other) const {
        if (m_bytes.size() != other.m_bytes.size())
            throw std::runtime_error("[TYPES]: Data::operator^: Size mismatch");

        Data result{};
        const std::size_t num_of_bytes{m_bytes.size()};
        result.m_bytes.resize(num_of_bytes);

        for (std::size_t i = 0; i < num_of_bytes; ++i)
            result.m_bytes[i] = m_bytes[i] ^ other.m_bytes[i];

        return result;
    }

    __m128i Data::as_m128i() const {
        return *reinterpret_cast<const __m128i *>(m_bytes.data());
    }
}
