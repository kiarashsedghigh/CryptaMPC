#ifndef COOT_H
#define COOT_H

#include "mpc/protocols/ot/ot.h"
#include "math/group.h"
#include "math/bigint.h"

namespace qst::mpc::protocols {
    template<typename IO_T>
    /**
    * @brief Chou Orlandi OT class
    *
    * @note Link to the paper: https://eprint.iacr.org/2015/267.pdf
    */
    class COOT final : public OT<IO_T> {
    public:
        //todo
        static void send_pt(IO_T *io, math::algstruct::Point *C) {
            size_t len = C->size();
            C->group->resize_scratch(len);
            unsigned char *tmp = C->group->scratch;
            io->write(&len, 4);
            C->to_bin(tmp, len);
            io->write(tmp, len);
        }

        //todo
        static void recv_pt(IO_T *io, math::algstruct::Group *g, math::algstruct::Point *C) {
            size_t len = 0;
            io->read(&len, 4);
            // assert(len <= 2048);
            g->resize_scratch(len);
            unsigned char *tmp = g->scratch;
            io->read(tmp, len);
            C->from_bin(g, tmp, len);
        }


        COOT(const IO_T &io, math::algstruct::Group *g, const std::uint64_t data_size)
            : m_io(const_cast<IO_T *>(&io)), m_group(g ? g : new math::algstruct::Group()), m_data_size(data_size) {
        }


        /**
         * @brief Send function for 1-2 OT instance of Naor Pinkas OT
         * @param data1 First data
         * @param data2 Second data
         */
        void send(const types::Data &data1, const types::Data &data2) override {
            math::numbers::BigInt a;
            math::algstruct::Point A, AaInv;
            math::algstruct::Point B, BA;

            m_group->get_rand_bn(a);
            A = m_group->mul_gen(a);
            send_pt(m_io, &A);
            AaInv = A.mul(a);
            AaInv = AaInv.inv();

            recv_pt(m_io, m_group, &B);
            B = B.mul(a);
            BA = B.add(AaInv);

            m_io->flush();

            types::Data masked_data[2]{};
            masked_data[0] = hash_point_to_data(B) ^ data1;
            masked_data[1] = hash_point_to_data(BA) ^ data2;

            /* Convert Data to array of bytes */
            auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
            masked_data[0].to_bytes(byte_buffer.get());
            masked_data[1].to_bytes(byte_buffer.get() + m_data_size);
            m_io->write(byte_buffer.get(), 2 * m_data_size);
        }


        /**
         * @brief Receive function for 1-2 OT instance of Chou Orlandi OT
         * @param choice Choice bit
         * @return Retrieved data
         */
        types::Data recv(const bool choice) override {
            math::numbers::BigInt bb;
            math::algstruct::Point B, As, A;
            m_group->get_rand_bn(bb);

            recv_pt(m_io, m_group, &A);

            B = m_group->mul_gen(bb);
            if (choice)
                B = B.add(A);
            send_pt(m_io, &B);


            m_io->flush();

            As = A.mul(bb);


            auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
            m_io->read(byte_buffer.get(), 2 * m_data_size);
            const types::Data masked_data[2] = {
                types::Data{byte_buffer.get(), m_data_size},
                types::Data{byte_buffer.get() + m_data_size, m_data_size}
            };

            types::Data temp_data{hash_point_to_data(As)};

            if (choice)
                temp_data = temp_data ^ masked_data[1];
            else
                temp_data = temp_data ^ masked_data[0];

            return temp_data;
        }

        /**
         * @brief Send function for N 1-2 OT instance of Chou Orlandi OT
         * @param arr_data1 List of first element of the tuples
         * @param arr_data2 List of second element of the tuples
         * @param count Number of tuples
         */
        void send_n(const types::Data *arr_data1, const types::Data *arr_data2, int count) override {
            math::numbers::BigInt a;
            math::algstruct::Point A, AaInv;
            auto *B = new math::algstruct::Point[count];
            auto *BA = new math::algstruct::Point[count];

            m_group->get_rand_bn(a);
            A = m_group->mul_gen(a);
            send_pt(m_io, &A);
            AaInv = A.mul(a);
            AaInv = AaInv.inv();

            for (int64_t i = 0; i < count; ++i) {
                recv_pt(m_io, m_group, &B[i]);
                B[i] = B[i].mul(a);
                BA[i] = B[i].add(AaInv);
            }
            m_io->flush();

            for (int64_t i = 0; i < count; ++i) {
                types::Data masked_data[2]{};
                masked_data[0] = hash_point_to_data(B[i], i) ^ arr_data1[i];
                masked_data[1] = hash_point_to_data(BA[i], i) ^ arr_data2[i];


                /* Convert Data to array of bytes */
                auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
                masked_data[0].to_bytes(byte_buffer.get());
                masked_data[1].to_bytes(byte_buffer.get() + m_data_size);
                m_io->write(byte_buffer.get(), 2 * m_data_size);
            }

            delete[] BA;
            delete[] B;
        }

        /**
         * @brief Receive function for N 1-2 OT instance of Chou Orlandi OT
         * @param arr_data Pointer to the list of data to be written into
         * @param choices List of choice bits
         * @param count Number of tuples
         */
        void recv_n(types::Data *arr_data, const bool *choices, const int count) override {
            auto *bb = new math::numbers::BigInt [count];
            auto *B = new math::algstruct::Point[count],
                    *As = new math::algstruct::Point[count];
            math::algstruct::Point A;

            for (int64_t i = 0; i < count; ++i)
                m_group->get_rand_bn(bb[i]);

            recv_pt(m_io, m_group, &A);

            for (int64_t i = 0; i < count; ++i) {
                B[i] = m_group->mul_gen(bb[i]);
                if (choices[i])
                    B[i] = B[i].add(A);
                send_pt(m_io, &B[i]);
            }
            m_io->flush();

            for (int64_t i = 0; i < count; ++i)
                As[i] = A.mul(bb[i]);

            for (int64_t i = 0; i < count; ++i) {
                auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
                m_io->read(byte_buffer.get(), 2 * m_data_size);

                const types::Data masked_data[2] = {
                    types::Data{byte_buffer.get(), m_data_size},
                    types::Data{byte_buffer.get() + m_data_size, m_data_size}
                };

                arr_data[i] = hash_point_to_data(As[i], i);

                if (choices[i])
                    arr_data[i] = arr_data[i] ^ masked_data[1];
                else
                    arr_data[i] = arr_data[i] ^ masked_data[0];
            }
        }

    private:
        /**
         * Pointer to the base IO for the OT
         */
        IO_T *m_io;

        /**
         * @brief Underlying algebraic group structure
         */
        math::algstruct::Group *m_group;

        /**
         * @brief Size of each data to be sent in the OT in Bytes
         */
        std::uint64_t m_data_size;
    };
}

#endif
