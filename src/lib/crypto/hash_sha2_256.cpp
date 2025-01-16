#include "crypto/hash_sha2_256.h"
#include <cstring>
#include <stdexcept>

namespace qst::crypto::hash {
    Sha2_256::Sha2_256() {
        if ((m_evp_ctx = EVP_MD_CTX_create()) == nullptr)
            throw std::runtime_error("Hash function setup error!");
        if (1 != EVP_DigestInit_ex(m_evp_ctx, EVP_sha256(), nullptr))
            throw std::runtime_error("Hash function setup error!");
    }

    Sha2_256::~Sha2_256() {
        EVP_MD_CTX_destroy(m_evp_ctx);
    }

    void Sha2_256::put(const void *input, std::uint32_t input_length) {
        if (input_length >= HASH_BUFFER_SIZE)
            EVP_DigestUpdate(m_evp_ctx, input, input_length);
        else if (m_in_buffer_data_length + input_length < HASH_BUFFER_SIZE) {
            memcpy(m_buffer + m_in_buffer_data_length, input, input_length);
            m_in_buffer_data_length += input_length;
        } else {
            EVP_DigestUpdate(m_evp_ctx, m_buffer, m_in_buffer_data_length);
            memcpy(m_buffer, input, input_length);
            m_in_buffer_data_length = input_length;
        }
    }

    void Sha2_256::digest(void *output) {
        if (m_in_buffer_data_length > 0) {
            EVP_DigestUpdate(m_evp_ctx, m_buffer, m_in_buffer_data_length);
            m_in_buffer_data_length = 0;
        }

        uint32_t len = 0;
        EVP_DigestFinal_ex(m_evp_ctx, static_cast<unsigned char *>(output), &len);
        reset();
    }

    void Sha2_256::hash_once(void *digest, const void *input, const int input_length) {
        Sha2_256 hash{};
        hash.put(input, input_length);
        hash.digest(digest);
    }

    void Sha2_256::reset() {
        EVP_DigestInit_ex(m_evp_ctx, EVP_sha256(), nullptr);
        m_in_buffer_data_length = 0;
    }
}
