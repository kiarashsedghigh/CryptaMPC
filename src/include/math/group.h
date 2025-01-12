#ifndef GROUP_H
#define GROUP_H


#include "math/bigint.h"
#include <openssl/ec.h>
#include <string>
#include <iostream>
#include <emmintrin.h>
#include "crypto/sha2.h"


namespace qst::math::algstruct {
    class Group;

    class Point {
    public:
        EC_POINT *point = nullptr;
        Group *group = nullptr;

        Point(Group *g = nullptr);

        ~Point();

        Point(const Point &p);

        Point &operator=(Point p);

        void to_bin(unsigned char *buf, size_t buf_len);

        size_t size();

        void from_bin(Group *g, const unsigned char *buf, size_t buf_len);

        Point add(Point &rhs);

        //		Point sub(Point & rhs);
        //		bool is_at_infinity();
        //		bool is_on_curve();
        Point mul(const numbers::BigInt &m);

        Point inv();


        bool operator==(Point &rhs);




    };

    class Group {
    public:
        EC_GROUP *ec_group = nullptr;
        BN_CTX *bn_ctx = nullptr;
        numbers::BigInt order;
        unsigned char *scratch;
        size_t scratch_size = 256;

        Group();

        ~Group();

        void resize_scratch(size_t size);

        void get_rand_bn(numbers::BigInt &n);

        Point get_generator();

        Point mul_gen(const numbers::BigInt &m);



    };


    #ifdef __x86_64__
    __attribute__((target("sse2")))
    #endif
    inline types::Data hash_for_block(const void * data, int nbyte) {
        char digest[32];
        crypto::hash::Sha2::hash_once(digest, data, nbyte);
        return types::Data{_mm_load_si128((types::__m128i*)&digest[0])};
    }

    //todo change design
    inline types::Data hash_point_to_data(math::algstruct::Point &in, uint64_t id = 1) {
        size_t len = in.size();
        in.group->resize_scratch(len + 8);
        unsigned char * tmp = in.group->scratch;
        in.to_bin(tmp, len);
        memcpy(tmp + len, &id, 8);
        return {hash_for_block(tmp, len + 8)};
    }

    //todo not mine
    inline void error(const std::string &message) {

        std::cerr << "Error: " << message << std::endl;
        std::exit(EXIT_FAILURE); // Terminates the program
    }


    inline Point::Point(Group *g) {
        if (g == nullptr) return;
        this->group = g;
        point = EC_POINT_new(group->ec_group);
    }

    inline Point::~Point() {
        if (point != nullptr)
            EC_POINT_free(point);
    }

    inline Point::Point(const Point &p) {
        if (p.group == nullptr) return;
        this->group = p.group;
        point = EC_POINT_new(group->ec_group);
        int ret = EC_POINT_copy(point, p.point);
        if (ret == 0) error("ECC COPY");
    }

    inline Point &Point::operator=(Point p) {
        std::swap(p.point, point);
        std::swap(p.group, group);
        return *this;
    }

    inline void Point::to_bin(unsigned char *buf, size_t buf_len) {
        int ret = EC_POINT_point2oct(group->ec_group, point, POINT_CONVERSION_UNCOMPRESSED, buf, buf_len,
                                     group->bn_ctx);
        if (ret == 0) error("ECC TO_BIN");
    }

    inline size_t Point::size() {
        size_t ret = EC_POINT_point2oct(group->ec_group, point, POINT_CONVERSION_UNCOMPRESSED, NULL, 0, group->bn_ctx);
        if (ret == 0) error("ECC SIZE_BIN");
        return ret;
    }

    inline void Point::from_bin(Group *g, const unsigned char *buf, size_t buf_len) {
        if (point == nullptr) {
            group = g;
            point = EC_POINT_new(group->ec_group);
        }
        int ret = EC_POINT_oct2point(group->ec_group, point, buf, buf_len, group->bn_ctx);
        if (ret == 0) error("ECC FROM_BIN");
    }

    inline Point Point::add(Point &rhs) {
        Point ret(group);
        int res = EC_POINT_add(group->ec_group, ret.point, point, rhs.point, group->bn_ctx);
        if (res == 0) error("ECC ADD");
        return ret;
    }

    inline Point Point::mul(const numbers::BigInt &m) {
        Point ret(group);
        int res = EC_POINT_mul(group->ec_group, ret.point, NULL, point, m.n, group->bn_ctx);
        if (res == 0) error("ECC MUL");
        return ret;
    }

    inline Point Point::inv() {
        Point ret(*this);
        int res = EC_POINT_invert(group->ec_group, ret.point, group->bn_ctx);
        if (res == 0) error("ECC INV");
        return ret;
    }

    inline bool Point::operator==(Point &rhs) {
        int ret = EC_POINT_cmp(group->ec_group, point, rhs.point, group->bn_ctx);
        if (ret == -1) error("ECC CMP");
        return (ret == 0);
    }


    inline Group::Group() {
        ec_group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1); //NIST P-256
        bn_ctx = BN_CTX_new();
        EC_GROUP_get_order(ec_group, order.n, bn_ctx);
        scratch = new unsigned char[scratch_size];
    }

    inline Group::~Group() {
        if (ec_group != nullptr)
            EC_GROUP_free(ec_group);

        if (bn_ctx != nullptr)
            BN_CTX_free(bn_ctx);

        if (scratch != nullptr)
            delete[] scratch;
    }

    inline void Group::resize_scratch(size_t size) {
        if (size > scratch_size) {
            delete[] scratch;
            scratch_size = size;
            scratch = new unsigned char[scratch_size];
        }
    }

    inline void Group::get_rand_bn(numbers::BigInt &n) {
        BN_rand_range(n.n, order.n);
    }

    inline Point Group::get_generator() {
        Point res(this);
        int ret = EC_POINT_copy(res.point, EC_GROUP_get0_generator(ec_group));
        if (ret == 0) error("ECC GEN");
        return res;
    }

    inline Point Group::mul_gen(const numbers::BigInt &m) {
        Point res(this);
        int ret = EC_POINT_mul(ec_group, res.point, m.n,NULL, NULL, bn_ctx);
        if (ret == 0) error("ECC GEN MUL");
        return res;
    }
}


#endif
