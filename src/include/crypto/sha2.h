#ifndef SHA2_H
#define SHA2_H


#include "types.h"
#include "hash.h"
#include <openssl/evp.h>

namespace qst::crypto::hash {
    class Sha2 : public Hash {
    public:
        Sha2();

        ~Sha2();

        void put(const void *input_buffer, std::uint32_t input_length) override;

        void digest(void *output_buffer) override;

        void put_block(const types::Data *blk, int nblock = 1);

        void reset();

        // static void hash_once(void *dgst, const void *data, int nbyte);


        static void hash_once(void * dgst, const void * data, int nbyte) {
            Sha2 hash;
            hash.put(data, nbyte);
            hash.digest(dgst);
        }

        // static types::Data hash_for_block(const void *data, int nbyte);
        //
        // static types::Data KDF(math::algstruct::Point &in, uint64_t id = 1);

        constexpr static int digest_size = 32;

    private:
        std::uint8_t m_buffer[Hash::hash_buffer_size];
        std::uint32_t m_size{};
        EVP_MD_CTX *m_ctx{};
    };
}


#endif
