#include "types.h"

#include <stdexcept>
#include <iostream>

namespace qst::types {
    /**
     * @brief asd
     * SOME FUNCTION
     * @param data
     */
    Data::Data(const qst::types::__m128i &data){
        const auto* ptr = reinterpret_cast<const std::uint8_t*>(&data);
        for (size_t i = 0; i <16; ++i)
            m_bytes.push_back(ptr[i]);
    }
    std::uint64_t Data::get_size() const {
      return m_bytes.size();
    }

    std::vector<std::uint8_t> Data::get_data_as_vector() const {
        return m_bytes;
    }

    Data Data::operator^(const Data& other) const {
        if (m_bytes.size() != other.m_bytes.size())
            throw std::runtime_error("Data::operator^: Size mismatch");
        Data result;

        std::size_t num_of_bytes {m_bytes.size()};

        for (std::size_t i = 0; i < num_of_bytes; ++i)
            result.m_bytes.push_back( m_bytes[i] ^ other.m_bytes[i]); // Perform XOR on each byte

        return result;
    }
}
