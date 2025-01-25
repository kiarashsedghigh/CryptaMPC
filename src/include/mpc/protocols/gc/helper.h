#ifndef EMP_AG2PC_HELPER_H__
#define EMP_AG2PC_HELPER_H__
#include <emp-tool/emp-tool.h>

using std::future;
using std::cout;
using std::endl;

namespace emp {

    template<typename T>
    void helper_send_data(T* io, const void *data, size_t nbyte) {
        io->write(data, nbyte);
    }

    template<typename T>
    void helper_recv_data(T* io, void *data, size_t nbyte) {
        io->read(data, nbyte);
    }

    template<typename T>
    void helper_send_block(T* io, const block *data, size_t nblock) {
        helper_send_data(io, data, nblock * sizeof(block));
    }

    template<typename T>
    void helper_recv_block(T* io, block *data, size_t nblock) {
        helper_recv_data(io, data, nblock * sizeof(block));
    }

    // void send_pt(Point *A, size_t num_pts = 1) {
    //     for (size_t i = 0; i < num_pts; ++i) {
    //         size_t len = A[i].size();
    //         A[i].group->resize_scratch(len);
    //         unsigned char *tmp = A[i].group->scratch;
    //         send_data(&len, 4);
    //         A[i].to_bin(tmp, len);
    //         send_data(tmp, len);
    //     }
    // }

    // void recv_pt(Group *g, Point *A, size_t num_pts = 1) {
    //     size_t len = 0;
    //     for (size_t i = 0; i < num_pts; ++i) {
    //         recv_data(&len, 4);
    //         assert(len <= 2048);
    //         g->resize_scratch(len);
    //         unsigned char *tmp = g->scratch;
    //         recv_data(tmp, len);
    //         A[i].from_bin(g, tmp, len);
    //     }
    // }



    template<typename T>
    void send_bool_aligned(T * io, const bool *data, size_t length) {
        const bool *data64 = data;
        size_t i = 0;
        unsigned long long unpack;
        for (; i < length / 8; ++i) {
            unsigned long long mask = 0x0101010101010101ULL;
            unsigned long long tmp = 0;
            memcpy(&unpack, data64, sizeof(unpack));
            data64 += sizeof(unpack);
#if defined(__BMI2__)
			tmp = _pext_u64(unpack, mask);
#else
            // https://github.com/Forceflow/libmorton/issues/6
            for (unsigned long long bb = 1; mask != 0; bb += bb) {
                if (unpack & mask & -mask) { tmp |= bb; }
                mask &= (mask - 1);
            }
#endif
            helper_send_data(io, &tmp, 1);
        }
        if (8 * i != length)
            helper_send_data(io, data + 8 * i, length - 8 * i);
    }

    template<typename T>
    void recv_bool_aligned(T* io, bool *data, size_t length) {
        bool *data64 = data;
        size_t i = 0;
        unsigned long long unpack;
        for (; i < length / 8; ++i) {
            unsigned long long mask = 0x0101010101010101ULL;
            unsigned long long tmp = 0;
            helper_recv_data(io, &tmp, 1);
#if defined(__BMI2__)
			unpack = _pdep_u64(tmp, mask);
#else
            unpack = 0;
            for (unsigned long long bb = 1; mask != 0; bb += bb) {
                if (tmp & bb) { unpack |= mask & (-mask); }
                mask &= (mask - 1);
            }
#endif
            memcpy(data64, &unpack, sizeof(unpack));
            data64 += sizeof(unpack);
        }
        if (8 * i != length)
            helper_recv_data(io, data + 8 * i, length - 8 * i);
    }


    template<typename T>
    void helper_send_bool(T * io, bool *data, size_t length) {
        void *ptr = (void *) data;
        size_t space = length;
        const void *aligned = std::align(alignof(uint64_t), sizeof(uint64_t), ptr, space);
        if (aligned == nullptr)
            helper_send_data(io, data, length);
        else {
            size_t diff = length - space;
            helper_send_data(io, data, diff);
            send_bool_aligned(io, (const bool *) aligned, length - diff);
        }
    }

    template<typename T>
    void helper_recv_bool(T* io, bool *data, size_t length) {
        void *ptr = (void *) data;
        size_t space = length;
        void *aligned = std::align(alignof(uint64_t), sizeof(uint64_t), ptr, space);
        if (aligned == nullptr)
            helper_recv_data(io, data, length);
        else {
            size_t diff = length - space;
            helper_recv_data(io, data, diff);
            recv_bool_aligned(io, (bool *) aligned, length - diff);
        }
    }














    template<typename T>
    void joinNclean(vector<future<T> > &res) {
        for (auto &v: res) v.get();
        res.clear();
    }

    template<typename T, int B>
    void send_partial_block(T *io, const block *data, int length) {
        for (int i = 0; i < length; ++i) {
            helper_send_data(io, &(data[i]), B);
        }
    }

    template<typename T, int B>
    void recv_partial_block(T *io, block *data, int length) {
        for (int i = 0; i < length; ++i) {
            helper_recv_data(io, &(data[i]), B);
        }
    }

    template<typename T>
    block coin_tossing(PRG prg, T *io, int party) {
        block S, S2;
        char dgst[Hash::DIGEST_SIZE];
        prg.random_block(&S, 1);
        if (party == ALICE) {
            Hash::hash_once(dgst, &S, sizeof(block));
            helper_send_data(io, dgst, Hash::DIGEST_SIZE);
            helper_recv_block(io, &S2, 1);
            helper_send_block(io, &S, 1);
        } else {
            char dgst2[Hash::DIGEST_SIZE];
            helper_recv_data(io, dgst2, Hash::DIGEST_SIZE);
            helper_send_block(io, &S, 1);
            helper_recv_block(io, &S2, 1);
            Hash::hash_once(dgst, &S2, sizeof(block));
            if (memcmp(dgst, dgst2, Hash::DIGEST_SIZE) != 0)
                error("cheat CT!");
        }
        io->flush();
        return S ^ S2;
    }

}
#endif// __HELPER
