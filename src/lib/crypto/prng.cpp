#include "crypto/prng.h"


namespace qst::crypto::prng {
    PrngAes::PrngAes(const void *seed, int id) {
        if (seed != nullptr) {
            reseed((const block *) seed, id);
        } else {
            block v;
#ifndef ENABLE_RDSEED
            uint32_t *data = (uint32_t *) (&v);
            std::random_device rand_div("/dev/urandom");
            for (size_t i = 0; i < sizeof(block) / sizeof(uint32_t); ++i)
                data[i] = rand_div();
#else
        unsigned long long r0, r1;
        int i = 0;
        for (; i < 10; ++i)
            if ((_rdseed64_step(&r0) == 1) && (r0 != ULLONG_MAX) && (r0 != 0)) break;
        if (i == 10) error("RDSEED FAILURE");

        for (i = 0; i < 10; ++i)
            if ((_rdseed64_step(&r1) == 1) && (r1 != ULLONG_MAX) && (r1 != 0)) break;
        if (i == 10) error("RDSEED FAILURE");

        v = makeBlock(r0, r1);
#endif
            reseed(&v, id);
        }
    }

    void PrngAes::reseed(const block *seed, uint64_t id) {
        block v = _mm_loadu_si128(seed);
        v ^= emp::makeBlock(0LL, id);
        m_key = v;
        emp::AES_set_encrypt_key(v, &m_aes);
        m_counter = 0;
    }

    void PrngAes::random_data(void *data, int nbytes) {
        random_block((block *) data, nbytes / 16);
        if (nbytes % 16 != 0) {
            block extra;
            random_block(&extra, 1);
            memcpy((nbytes / 16 * 16) + (char *) data, &extra, nbytes % 16);
        }
    }

    void PrngAes::random_bool(bool *data, int length) {
        uint8_t *uint_data = (uint8_t *) data;
        random_data_unaligned(uint_data, length);
        for (int i = 0; i < length; ++i)
            data[i] = uint_data[i] & 1;
    }

    void PrngAes::random_data_unaligned(void *data, int nbytes) {
        size_t size = nbytes;
        void *aligned_data = data;
        if (std::align(sizeof(block), sizeof(block), aligned_data, size)) {
            int chopped = nbytes - size;
            random_data(aligned_data, nbytes - chopped);
            block tmp[1];
            random_block(tmp, 1);
            memcpy(data, tmp, chopped);
        } else {
            block tmp[2];
            random_block(tmp, 2);
            memcpy(data, tmp, nbytes);
        }
    }

    void PrngAes::random_block(block *data, int nblocks) {
        block tmp[emp::AES_BATCH_SIZE];
        for (int i = 0; i < nblocks / emp::AES_BATCH_SIZE; ++i) {
            for (int j = 0; j < emp::AES_BATCH_SIZE; ++j)
                tmp[j] = emp::makeBlock(0LL, m_counter++);
            AES_ecb_encrypt_blks<emp::AES_BATCH_SIZE>(tmp, &m_aes);
            memcpy(data + i * emp::AES_BATCH_SIZE, tmp, emp::AES_BATCH_SIZE * sizeof(block));
        }
        int remain = nblocks % emp::AES_BATCH_SIZE;
        for (int j = 0; j < remain; ++j)
            tmp[j] = emp::makeBlock(0LL, m_counter++);
        AES_ecb_encrypt_blks(tmp, remain, &m_aes);
        memcpy(data + (nblocks / emp::AES_BATCH_SIZE) * emp::AES_BATCH_SIZE, tmp, remain * sizeof(block));
    }

    // PRG::result_type PRG::min() {
    //     return 0;
    // }
    //
    // PRG::result_type PRG::max() {
    //     return 0xFFFFFFFFFFFFFFFFULL;
    // }

    PrngAes::result_type PrngAes::operator()() {
        if (m_ptr == 32) {
            random_block((block *) m_buffer, 16);
            m_ptr = 0;
        }
        return m_buffer[m_ptr++];
    }
}
