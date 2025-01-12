#ifndef NPOT_H
#define NPOT_H

#include "mpc/protocols/ot/ot.h"
#include "math/group.h"
#include "math/bigint.h"

namespace qst::mpc::protocols {
    template<typename IO_T>
    /**
    * @brief Naor Pinkas OT class.
    */
    class NPOT final : public OT<IO_T> {
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


        NPOT(const IO_T &io, math::algstruct::Group *g, const std::uint64_t data_size)
            : m_io(const_cast<IO_T *>(&io)), m_group(g ? g : new math::algstruct::Group()), m_data_size(data_size) {
        }


        /**
         * @brief Send function for 1-2 OT instance of Naor Pinkas OT
         * @param data1 First data
         * @param data2 Second data
         */
        void send(const types::Data &data1, const types::Data &data2) override {
            math::numbers::BigInt d{};
            m_group->get_rand_bn(d);
            math::algstruct::Point C = m_group->mul_gen(d);

            send_pt(m_io, &C);
            m_io->flush();

            math::numbers::BigInt r{};
            math::numbers::BigInt rc{};
            math::algstruct::Point pk0{}, gr{}, Cr{};

            m_group->get_rand_bn(r);
            gr = m_group->mul_gen(r);
            rc = r.mul(d, m_group->bn_ctx);
            rc = rc.mod(m_group->order, m_group->bn_ctx);
            Cr = m_group->mul_gen(rc);

            recv_pt(m_io, m_group, &pk0);
            send_pt(m_io, &gr);
            m_io->flush();

            types::Data masked_data[2];
            pk0 = pk0.mul(r);
            math::algstruct::Point inv = pk0.inv();
            math::algstruct::Point pk1 = Cr.add(inv);

            masked_data[0] = hash_point_to_data(pk0) ^ data1;
            masked_data[1] = hash_point_to_data(pk1) ^ data2;

            /* Convert Data to array of bytes */
            auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
            masked_data[0].to_bytes(byte_buffer.get());
            masked_data[1].to_bytes(byte_buffer.get() + m_data_size);
            m_io->write(byte_buffer.get(), 2 * m_data_size);
        }


        /**
         * @brief Receive function for 1-2 OT instance of Naor Pinkas OT
         * @param choice Choice bit
         * @return Retrieved data
         */
        types::Data recv(const bool choice) override {
            math::numbers::BigInt k{};
            math::algstruct::Point gr{};
            math::algstruct::Point pk[2];

            math::algstruct::Point C;
            m_group->get_rand_bn(k);

            recv_pt(m_io, m_group, &C);
            m_io->flush();

            if (choice) {
                pk[1] = m_group->mul_gen(k);
                math::algstruct::Point inv = pk[1].inv();
                pk[0] = C.add(inv);
            } else
                pk[0] = m_group->mul_gen(k);

            send_pt(m_io, &pk[0]);
            recv_pt(m_io, m_group, &gr);

            gr = gr.mul(k);

            auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
            m_io->read(byte_buffer.get(), 2 * m_data_size);

            const types::Data masked_data[2] = {
                types::Data{byte_buffer.get(), m_data_size},
                types::Data{byte_buffer.get() + m_data_size, m_data_size}
            };

            const int ind{choice ? 1 : 0};
            return masked_data[ind] ^ hash_point_to_data(gr);
        }

        /**
         * @brief Send function for N 1-2 OT instance of Naor Pinkas OT
         * @param arr_data1 List of first element of the tuples
         * @param arr_data2 List of second element of the tuples
         * @param count Number of tuples
         */
        void send_n(const types::Data *arr_data1, const types::Data *arr_data2, int count) override {
            math::numbers::BigInt d;
            m_group->get_rand_bn(d);
            math::algstruct::Point C = m_group->mul_gen(d);

            send_pt(m_io, &C);

            m_io->flush();

            auto *r = new math::numbers::BigInt[count];
            auto *rc = new math::numbers::BigInt[count];
            auto *pk0 = new math::algstruct::Point[count],
                    *gr = new math::algstruct::Point[count],
                    *Cr = new math::algstruct::Point[count];

            for (int64_t i = 0; i < count; ++i) {
                m_group->get_rand_bn(r[i]);
                gr[i] = m_group->mul_gen(r[i]);
                rc[i] = r[i].mul(d, m_group->bn_ctx);
                rc[i] = rc[i].mod(m_group->order, m_group->bn_ctx);
                Cr[i] = m_group->mul_gen(rc[i]);
            }

            for (int64_t i = 0; i < count; ++i)
                recv_pt(m_io, m_group, &pk0[i]);


            for (int64_t i = 0; i < count; ++i)
                send_pt(m_io, &gr[i]);


            m_io->flush();

            for (int64_t i = 0; i < count; ++i) {
                pk0[i] = pk0[i].mul(r[i]);
                math::algstruct::Point inv = pk0[i].inv();
                math::algstruct::Point pk1 = Cr[i].add(inv);


                types::Data masked_data[2]{};
                masked_data[0] = hash_point_to_data(pk0[i]) ^ arr_data1[i];
                masked_data[1] = hash_point_to_data(pk1) ^ arr_data2[i];

                /* Convert Data to array of bytes */
                auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
                masked_data[0].to_bytes(byte_buffer.get());
                masked_data[1].to_bytes(byte_buffer.get() + m_data_size);
                m_io->write(byte_buffer.get(), 2 * m_data_size);
            }

            delete[] r;
            delete[] gr;
            delete[] Cr;
            delete[] rc;
            delete[] pk0;
        }

        /**
         * @brief Receive function for N 1-2 OT instance of Naor Pinkas OT
         * @param arr_data Pointer to the list of data to be written into
         * @param choices List of choice bits
         * @param count Number of tuples
         */
        void recv_n(types::Data *arr_data, const bool *choices, const int count) override {
            auto *k = new math::numbers::BigInt[count];
            auto *gr = new math::algstruct::Point[count];
            math::algstruct::Point pk[2];
            math::algstruct::Point C;

            for (int64_t i = 0; i < count; ++i)
                m_group->get_rand_bn(k[i]);
            recv_pt(m_io, m_group, &C);

            m_io->flush();

            for (int64_t i = 0; i < count; ++i) {
                if (choices[i]) {
                    pk[1] = m_group->mul_gen(k[i]);
                    math::algstruct::Point inv = pk[1].inv();
                    pk[0] = C.add(inv);
                } else {
                    pk[0] = m_group->mul_gen(k[i]);
                }
                send_pt(m_io, &pk[0]);
            }

            for (int64_t i = 0; i < count; ++i) {
                recv_pt(m_io, m_group, &gr[i]);
                gr[i] = gr[i].mul(k[i]);
            }
            m_io->flush();

            for (int64_t i = 0; i < count; ++i) {
                // types::Data masked_data[2]{};

                const int ind{choices[i] ? 1 : 0};

                auto byte_buffer = std::make_unique<char[]>(2 * m_data_size);
                m_io->read(byte_buffer.get(), 2 * m_data_size);

                const types::Data masked_data[2] = {
                    types::Data{byte_buffer.get(), m_data_size},
                    types::Data{byte_buffer.get() + m_data_size, m_data_size}
                };

                arr_data[i] = masked_data[ind] ^ hash_point_to_data(gr[i]);
            }
            delete[] k;
            delete[] gr;
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
