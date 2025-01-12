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
        const auto* ptr = reinterpret_cast<const std::int8_t*>(&data);
        for (size_t i = 0; i <16; ++i)
            m_bytes.push_back(ptr[i]);
    }

    Data::Data(const int8_t * input, const std::size_t len){
        for (size_t i = 0; i < len; ++i)
            m_bytes.push_back(input[i]);
    }


    std::uint64_t Data::get_size() const {
      return m_bytes.size();
    }

    std::vector<std::int8_t> Data::get_data_as_vector() const {
        return m_bytes;
    }

    const std::int8_t* Data::get_ptr_ref() const {
        return m_bytes.data();
    }

    Data Data::operator^(const Data& other) const {
        if (m_bytes.size() != other.m_bytes.size())
            throw std::runtime_error("[TYPES]: Data::operator^: Size mismatch");
        Data result;

        const std::size_t num_of_bytes {m_bytes.size()};

        for (std::size_t i = 0; i < num_of_bytes; ++i)
            result.m_bytes.push_back(m_bytes[i] ^ other.m_bytes[i]);

        return result;
    }
}
