// #include "mpc/protocols/ot/npot.h"
//
//
// namespace qst::mpc::protocols {
//
//
//     // NPOT::NPOT(const IO_T &io, math::algstruct::Group *g = nullptr)
//     //     : io(io), G(_G), delete_G(_G == nullptr) {
//     //     if (_G == nullptr) {
//     //         G = new Group();
//     //     }
//     // }
//
//     // template<typename IO_T>
//     // NPOT<IO_T>::NPOT(const IO_T &io, math::algstruct::Group *g)
//     // : io(const_cast<IO_T *>(&io)), G(g ? g : new math::algstruct::Group()), delete_G(g == nullptr) {
//     //     std::cout << "NPOT Created \n";
//     // }
//     //
//     // template<typename IO_T>
//     // void NPOT<IO_T>::send(const types::Data &data1, const types::Data &data2) {
//     //     // Implementation of send
//     //     // Example: Perform operations using io, G, and data1/data2
//     // }
//     //
//     // template<typename IO_T>
//     // std::uint64_t NPOT<IO_T>::recv(const types::Data &buffer, bool choice) {
//     //     // Implementation of recv
//     //     // Example: Perform operations using io, G, and buffer
//     //     return 0; // Replace with the actual logic
//     // }
//
//     // NPOT::~NPOT() {
//     //     if (delete_G) {
//     //         delete G;
//     //     }
//     // }
//     //
//     // void NPOT::send(const block *data0, const block *data1, int64_t length) {
//     //     BigInt d;
//     //     G->get_rand_bn(d);
//     //     Point C = G->mul_gen(d);
//     //     io->send_pt(&C);
//     //     io->flush();
//     //
//     //     BigInt *r = new BigInt[length];
//     //     BigInt *rc = new BigInt[length];
//     //     Point *pk0 = new Point[length],
//     //           pk1,
//     //           *gr = new Point[length],
//     //           *Cr = new Point[length];
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         G->get_rand_bn(r[i]);
//     //         gr[i] = G->mul_gen(r[i]);
//     //         rc[i] = r[i].mul(d, G->bn_ctx);
//     //         rc[i] = rc[i].mod(G->order, G->bn_ctx);
//     //         Cr[i] = G->mul_gen(rc[i]);
//     //     }
//     //
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         io->recv_pt(G, &pk0[i]);
//     //     }
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         io->send_pt(&gr[i]);
//     //     }
//     //     io->flush();
//     //
//     //     block m[2];
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         pk0[i] = pk0[i].mul(r[i]);
//     //         Point inv = pk0[i].inv();
//     //         pk1 = Cr[i].add(inv);
//     //         m[0] = Hash::KDF(pk0[i]) ^ data0[i];
//     //         m[1] = Hash::KDF(pk1) ^ data1[i];
//     //         io->send_data(m, 2 * sizeof(block));
//     //     }
//     //
//     //     delete[] r;
//     //     delete[] gr;
//     //     delete[] Cr;
//     //     delete[] rc;
//     //     delete[] pk0;
//     // }
//     //
//     // std::uint64_t NPOT::recv(const types::Data &buffer, bool choice) {
//     //     BigInt *k = new BigInt[length];
//     //     Point *gr = new Point[length];
//     //     Point pk[2];
//     //     block m[2];
//     //     Point C;
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         G->get_rand_bn(k[i]);
//     //     }
//     //
//     //     io->recv_pt(G, &C);
//     //     io->flush();
//     //
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         if (b[i]) {
//     //             pk[1] = G->mul_gen(k[i]);
//     //             Point inv = pk[1].inv();
//     //             pk[0] = C.add(inv);
//     //         } else {
//     //             pk[0] = G->mul_gen(k[i]);
//     //         }
//     //         io->send_pt(&pk[0]);
//     //     }
//     //
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         io->recv_pt(G, &gr[i]);
//     //         gr[i] = gr[i].mul(k[i]);
//     //     }
//     //     io->flush();
//     //     for (int64_t i = 0; i < length; ++i) {
//     //         int ind = b[i] ? 1 : 0;
//     //         io->recv_data(m, 2 * sizeof(block));
//     //         data[i] = m[ind] ^ Hash::KDF(gr[i]);
//     //     }
//     //     delete[] k;
//     //     delete[] gr;
//     // }
//
// }
