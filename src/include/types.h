#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <vector>

namespace qst::types {

    typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));

    class Data {

    public:
        Data() = default;
        explicit Data(const __m128i &data);
        std::uint64_t get_size() const;
    private:
        std::uint64_t m_size {};
        std::vector<std::uint8_t> m_bytes {};
    };

    // class Block : public Data {
    // public:
    //     // using block = __m128i;
    //     Block() = default;
    //     __m128i get_block() const {
    //         return m_blk;
    //     }
    // private:
    //     __m128i m_blk {};
    // };


}

#endif //TYPES_H
