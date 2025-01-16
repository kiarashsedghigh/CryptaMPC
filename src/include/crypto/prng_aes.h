#ifndef PRG_H
#define PRG_H


#include "types.h"



#ifdef ENABLE_RDSEED
#include <x86intrin.h>
#else
#include <random>
#endif




#include "utils/block.h"
#include "utils/aes.h"
#include "utils/constants.h"
#include <climits>
#include <memory>





using block = __m128i;

namespace qst::crypto::prng {
    class PrngAes {
    public:
        explicit PrngAes(const void *seed = nullptr, int id = 0);
        void reseed(const block *seed, uint64_t id = 0);
        void random_data(void *data, int nbytes);
        void random_bool(bool *data, int length);
        void random_data_unaligned(void *data, int nbytes);
        void random_block(block *data, int nblocks = 1);

        typedef uint64_t result_type;
        // static constexpr result_type min();
        // static constexpr result_type max();
        result_type operator()();

    private:
        uint64_t m_counter {};
        emp::AES_KEY m_aes {};
        block m_key {};
        std::uint64_t m_buffer[32];
        size_t m_ptr {32};

    };

}



#endif