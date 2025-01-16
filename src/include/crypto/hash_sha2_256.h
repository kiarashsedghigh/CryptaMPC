#ifndef SHA2_H
#define SHA2_H


#include "types.h"
#include "hash.h"
#include <openssl/evp.h>

namespace qst::crypto::hash {
    class Sha2_256 final : public Hash {
    public:
        Sha2_256();

        ~Sha2_256() override;

        void put(const void *input, std::uint32_t input_length) override;

        void digest(void *output) override;

        void reset() override;

        static void hash_once(void *digest, const void *input, int input_length);

        /**
         * @brief SHA2-256 digest size in Bytes
         */
        constexpr static int DIGEST_SIZE{32};

        /**
         * @brief Maximum buffer size for reading inputs before hashing
         */
        constexpr static int HASH_BUFFER_SIZE{1024 * 8};

    private:
        /**
         * @brief Internal buffer for storing the input(s)
         */
        std::uint8_t m_buffer[HASH_BUFFER_SIZE]{};

        /**
         * @brief The length of the input currently in the buffer in Bytes
         */
        std::uint32_t m_in_buffer_data_length{};

        /**
         * @brief EVP-specific context variable
         */
        EVP_MD_CTX *m_evp_ctx{};
    };
}


#endif
