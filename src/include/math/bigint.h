#ifndef BIGINT_H
#define BIGINT_H

#include <openssl/ec.h>


namespace qst::math::numbers {
    class BigInt { public:
        BIGNUM *n = nullptr;
        BigInt();
        BigInt(const BigInt &oth);
        BigInt &operator=(BigInt oth);
        ~BigInt();

        int size();
        void to_bin(unsigned char * in);
        void from_bin(const unsigned char * in, int length);

        BigInt add(const BigInt &oth);
        BigInt mul(const BigInt &oth, BN_CTX *ctx);
        BigInt mod(const BigInt &oth, BN_CTX *ctx);
        BigInt add_mod(const BigInt & b, const BigInt& m, BN_CTX *ctx);
        BigInt mul_mod(const BigInt & b, const BigInt& m, BN_CTX *ctx);
    };
}



using BIGINT = qst::math::numbers::BigInt;

inline BIGINT::BigInt() {
    n = BN_new();
}
inline BIGINT::BigInt(const BigInt &oth) {
    n = BN_new();
    BN_copy(n, oth.n);
}
inline BIGINT& BIGINT::operator=(BigInt oth) {
    std::swap(n, oth.n);
    return *this;
}
inline BIGINT::~BigInt() {
    if (n != nullptr)
        BN_free(n);
}

inline int BIGINT::size() {
    return BN_num_bytes(n);
}

inline void BIGINT::to_bin(unsigned char * in) {
    BN_bn2bin(n, in);
}

inline void BIGINT::from_bin(const unsigned char * in, int length) {
    BN_free(n);
    n = BN_bin2bn(in, length, nullptr);
}

inline BIGINT BIGINT::add(const BigInt &oth) {
    BigInt ret;
    BN_add(ret.n, n, oth.n);
    return ret;
}

inline BIGINT BIGINT::mul_mod(const BigInt & b, const BigInt &m,  BN_CTX *ctx) {
    BigInt ret;
    BN_mod_mul(ret.n, n, b.n, m.n, ctx);
    return ret;
}

inline BIGINT BIGINT::add_mod(const BIGINT & b, const BIGINT &m,  BN_CTX *ctx) {
    BigInt ret;
    BN_mod_add(ret.n, n, b.n, m.n, ctx);
    return ret;
}

inline BIGINT BIGINT::mul(const BIGINT &oth, BN_CTX *ctx) {
    BigInt ret;
    BN_mul(ret.n, n, oth.n, ctx);
    return ret;
}

inline BIGINT BIGINT::mod(const BIGINT &oth, BN_CTX *ctx) {
    BIGINT ret;
    BN_mod(ret.n, n, oth.n, ctx);
    return ret;
}


#endif //BIGINT_H
