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

        Data(const std::int8_t * input, std::size_t len);

        std::uint64_t get_size() const;

        std::vector<std::int8_t> get_data_as_vector() const;

        const std::int8_t *get_ptr_ref() const;

        Data operator^(const Data &other) const;

    private:
        std::vector<std::int8_t> m_bytes {};
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
