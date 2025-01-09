#include "types.h"

namespace qst::types {
    Data::Data(const qst::types::__m128i &data)
    : m_size(16) {
        const auto* ptr = reinterpret_cast<const std::uint8_t*>(&data);
        for (size_t i = 0; i <16; ++i)
            m_bytes.push_back(ptr[i]);
    }
    std::uint64_t Data::get_size() const {
      return m_size;
    }

}