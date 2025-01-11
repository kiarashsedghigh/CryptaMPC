#ifndef NPOT_H
#define NPOT_H

#include "mpc/protocols/ot/ot.h"
#include "math/group.h"
#include "math/bigint.h"

namespace qst::mpc::protocols {


    template<typename IO_T>
    class NPOT : public OT<IO_T> {
    public:
        IO_T *io;
        math::algstruct::Group *G;
        bool delete_G;
        int m_length;


        void send_pt(IO_T* io, math::algstruct::Point* C) {
            size_t len = C->size();
            C->group->resize_scratch(len);
            unsigned char * tmp = C->group->scratch;
            io->send_data_internal(&len, 4);
            C->to_bin(tmp, len);
            io->send_data_internal(tmp, len);
        }

        void recv_pt(IO_T* io, math::algstruct::Group * g, math::algstruct::Point* C) {
            size_t len = 0;
            io->recv_data_internal(&len, 4);
            // assert(len <= 2048);
            g->resize_scratch(len);
            unsigned char * tmp = g->scratch;
            io->recv_data_internal(tmp, len);
            C->from_bin(g, tmp, len);
        }









        NPOT(const int length, const IO_T &io, math::algstruct::Group *g)
        : io(const_cast<IO_T *>(&io)), G(g ? g : new math::algstruct::Group()), delete_G(g == nullptr), m_length {length} {}


        void send_onetwo(const types::Data &data1, const types::Data &data2) {
            std::cout << "J\n";

            math::numbers::BigInt d;
            G->get_rand_bn(d);
            math::algstruct::Point C = G->mul_gen(d);


            // io->send_pt(&C);
            // -----------------------------------
            send_pt(io, &C);
















            io->flush();
            m_length = 1;

            auto * r = new math::numbers::BigInt[m_length];
            auto * rc = new math::numbers::BigInt[m_length];
            auto * pk0 = new math::algstruct::Point[m_length],
                    *gr = new math::algstruct::Point[m_length],
                    *Cr = new math::algstruct::Point[m_length];
            for(int64_t i = 0; i < m_length; ++i) {
                G->get_rand_bn(r[i]);
                gr[i] = G->mul_gen(r[i]);
                rc[i] = r[i].mul(d, G->bn_ctx);
                rc[i] = rc[i].mod(G->order, G->bn_ctx);
                Cr[i] = G->mul_gen(rc[i]);
            }

            for(int64_t i = 0; i < m_length; ++i) {
                std::cout << "recv points\n";

                // io->recv_pt(G, &pk0[i]);
                // --------------------------------
                recv_pt(io, G, &pk0[i]);
                std::cout << "Done\n";

            }
            for(int64_t i = 0; i < m_length; ++i) {
                std::cout << "sending points\n";

                // io->send_pt(&gr[i]);
                // ---------------------------------------------------
                send_pt(io, &gr[i]);



            }

            io->flush();

            types::Data m[2];
            for(int64_t i = 0 ; i < m_length; ++i) {
                pk0[i] = pk0[i].mul(r[i]);
                math::algstruct::Point inv = pk0[i].inv();
                math::algstruct::Point pk1 = Cr[i].add(inv);
                m[0] = qst::math::algstruct::hash_point_to_data(pk0[i]) ^ data1;
                m[0] = qst::math::algstruct::hash_point_to_data(pk1) ^ data2;

                // m[0] = Hash::KDF(pk0[i]) ^ data1[i];
                // m[1] = Hash::KDF(pk1) ^ data2[i];
                // io->send_data(m, 2*sizeof(block));
            }

            delete[] r;
            delete[] gr;
            delete[] Cr;
            delete[] rc;
            delete[] pk0;
            std::cout << "send\n";
        }

        types::Data recv_onetwo(bool choice) {
            m_length = 1;
            auto * k = new math::numbers::BigInt[m_length];
            auto * gr = new math::algstruct::Point[m_length];
            math::algstruct::Point pk[2];
            types::Data m[2];
            math::algstruct::Point C;
            for(int64_t i = 0; i < m_length; ++i)
                G->get_rand_bn(k[i]);

            // io->recv_pt(G, &C);
            // --------------------------------------
            recv_pt(io, G, &C);










            io->flush();

            for(int64_t i = 0; i< m_length; ++i) {
                if(choice) {
                    pk[1] = G->mul_gen(k[i]);
                    math::algstruct::Point inv = pk[1].inv();
                    pk[0] = C.add(inv);
                } else {
                    pk[0] = G->mul_gen(k[i]);
                }
                // io->send_pt(&pk[0]);
                // ------------------------
                send_pt(io, &pk[0]);
                //
                // size_t len = pk[0].size();
                // pk[0].group->resize_scratch(len);
                // unsigned char * tmp = pk[0].group->scratch;
                // io->send_data_internal(&len, 4);
                // pk[0].to_bin(tmp, len);
                // io->send_data_internal(tmp, len);













            }

            for(int64_t i = 0; i < m_length; ++i) {
                // io->recv_pt(G, &gr[i]);
                recv_pt(io, G, &gr[i]);















                gr[i] = gr[i].mul(k[i]);
            }
            io->flush();
            for(int64_t i = 0; i < m_length; ++i) {
                int ind = choice ? 1 : 0;
                io->recv_data_internal(m, 50); // todo


                // data[i] = m[ind] ^ Hash::KDF(gr[i]);
                // data
                return (m[ind] ^ math::algstruct::hash_point_to_data(gr[i]));
            }
            delete[] k;
            delete[] gr;

        }




        void send(const types::Data &data1, const types::Data &data2) override {
            // math::numbers::BigInt d;
            // G->get_rand_bn(d);
            // math::algstruct::Point C = G->mul_gen(d);
            //
            // io->send_pt(&C);
            // io->flush();
            //
            // auto * r = new math::numbers::BigInt[m_length];
            // auto * rc = new math::numbers::BigInt[m_length];
            // auto * pk0 = new math::algstruct::Point[m_length],
            //         *gr = new math::algstruct::Point[m_length],
            //         *Cr = new math::algstruct::Point[m_length];
            // for(int64_t i = 0; i < m_length; ++i) {
            //     G->get_rand_bn(r[i]);
            //     gr[i] = G->mul_gen(r[i]);
            //     rc[i] = r[i].mul(d, G->bn_ctx);
            //     rc[i] = rc[i].mod(G->order, G->bn_ctx);
            //     Cr[i] = G->mul_gen(rc[i]);
            // }
            //
            // for(int64_t i = 0; i < m_length; ++i) {
            //     std::cout << "recv points";
            //     io->recv_pt(G, &pk0[i]);
            // }
            // for(int64_t i = 0; i < m_length; ++i) {
            //     std::cout << "sending points";
            //     io->send_pt(&gr[i]);
            // }
            //
            // io->flush();
            //
            // types::Data m[2];
            // for(int64_t i = 0 ; i < m_length; ++i) {
            //     pk0[i] = pk0[i].mul(r[i]);
            //     math::algstruct::Point inv = pk0[i].inv();
            //     math::algstruct::Point pk1 = Cr[i].add(inv);
            //     m[0] = qst::math::algstruct::hash_point_to_data(pk0[i]) ^ data1[i];
            //     m[0] = qst::math::algstruct::hash_point_to_data(pk1) ^ data2[i];
            //
            //     // m[0] = Hash::KDF(pk0[i]) ^ data1[i];
            //     // m[1] = Hash::KDF(pk1) ^ data2[i];
            //     // io->send_data(m, 2*sizeof(block));
            // }
            //
            // delete[] r;
            // delete[] gr;
            // delete[] Cr;
            // delete[] rc;
            // delete[] pk0;
            std::cout << "send";
        }

        types::Data recv(bool choice) override {
            // auto * k = new math::numbers::BigInt[m_length];
            // auto * gr = new math::algstruct::Point[m_length];
            // math::algstruct::Point pk[2];
            // types::Data m[2];
            // math::algstruct::Point C;
            // for(int64_t i = 0; i < m_length; ++i)
            //     G->get_rand_bn(k[i]);
            //
            // io->recv_pt(G, &C);
            // io->flush();
            //
            // // for(int64_t i = 0; i< m_length; ++i) {
            // //     if(b[i]) {
            // //         pk[1] = G->mul_gen(k[i]);
            // //         math::algstruct::Point inv = pk[1].inv();
            // //         pk[0] = C.add(inv);
            // //     } else {
            // //         pk[0] = G->mul_gen(k[i]);
            // //     }
            // //     io->send_pt(&pk[0]);
            // // }
            //
            // for(int64_t i = 0; i < m_length; ++i) {
            //     io->recv_pt(G, &gr[i]);
            //     gr[i] = gr[i].mul(k[i]);
            // }
            // io->flush();
            // for(int64_t i = 0; i < m_length; ++i) {
            //     int ind = b[i] ? 1 : 0;
            //     io->recv_data(m, 2*sizeof(block));
            //     // data[i] = m[ind] ^ Hash::KDF(gr[i]);
            //     // data
            //     return (m[ind] ^ math::algstruct::hash_point_to_data(gr[i]));
            // }
            // delete[] k;
            // delete[] gr;
            return types::Data {};
        }
    };
}


#endif
