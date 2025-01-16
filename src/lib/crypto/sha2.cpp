/*
 * Implementation has been adopted from EMP-Toolkit
 *
 *
 */

#include "crypto/sha2.h"
#include <cstring>
#include <stdexcept>
#include <emmintrin.h>

namespace qst::crypto::hash {

    Sha2::Sha2() {
        if ((m_ctx = EVP_MD_CTX_create()) == NULL)
            throw std::runtime_error("Hash function setup error!");
        if (1 != EVP_DigestInit_ex(m_ctx, EVP_sha256(), NULL))
            throw std::runtime_error("Hash function setup error!");
    }

    Sha2::~Sha2() {
        EVP_MD_CTX_destroy(m_ctx);
    }

    void Sha2::put(const void * input_buffer, std::uint32_t input_length) {
        if (input_length >= Hash::hash_buffer_size)
            EVP_DigestUpdate(m_ctx, input_buffer, input_length);
        else if (m_size + input_length < hash_buffer_size) {
            memcpy(m_buffer + m_size, input_buffer, input_length);
            m_size += input_length;
        } else {
            EVP_DigestUpdate(m_ctx, m_buffer, m_size);
            memcpy(m_buffer, input_buffer, input_length);
            m_size = input_length;
        }
    }

    void Sha2::put_block(const types::Data* blk, int nblock) {
        put(blk, sizeof(blk->get_size()) * nblock);
    }

    void Sha2::digest(void * output_buffer) {
        if (m_size > 0) {
            EVP_DigestUpdate(m_ctx, m_buffer, m_size);
            m_size = 0;
        }

        uint32_t len = 0;
        EVP_DigestFinal_ex(m_ctx, (unsigned char *)output_buffer, &len);
        reset();
    }

    void Sha2::reset() {
        EVP_DigestInit_ex(m_ctx, EVP_sha256(), NULL);
        m_size = 0;
    }
}
