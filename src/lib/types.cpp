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

    Data::Data(const char *input) {
        const std::size_t length{strlen(input)};
        m_bytes.resize(length);
        std::memcpy(m_bytes.data(), input, length);
    }

    Data::Data(const char *input, const std::size_t len) {
        m_bytes.resize(len);
        std::memcpy(m_bytes.data(), input, len);
    }

    std::size_t Data::get_size() const {
        return m_bytes.size();
    }

    void Data::to_bytes(void *dest) const {
        std::memcpy(dest, m_bytes.data(), m_bytes.size());
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
