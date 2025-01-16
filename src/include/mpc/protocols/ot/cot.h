#ifndef COT_H
#define COT_H

#include "mpc/protocols/ot/ot.h"
#include "utils/mitccrh.h"
#include "utils/prg.h"
#include "crypto/prng_aes.h"

namespace emp {

const static int64_t ot_bsize = 8;
template<typename T>
class COT : public qst::mpc::protocols::OT<T>{ public:
	T * io = nullptr;
	emp::MITCCRH<ot_bsize> mitccrh;
	block Delta;
	qst::crypto::prng::PrngAes prg;
	virtual void send_cot(block* data0, int64_t length) = 0;
	virtual void recv_cot(block* data, const bool* b, int64_t length) = 0;

	void send(const qst::types::Data &data1, const qst::types::Data &data2) {}
	// void send_n(const qst::types::Data* arr_data1, const qst::types::Data* arr_data2, int count){}
	qst::types::Data recv(bool choice){}
	// void recv_n(qst::types::Data* arr_data, const bool* choices,  int count) {}


	void send_n(const qst::types::Data* arr_data1, const qst::types::Data* arr_data2, const std::int64_t count) {

		block * data = new block[count];
		send_cot(data, count);
		block s;prg.random_block(&s, 1);
		// io->send_block(&s,1);
		io->write(&s, 1 * sizeof(block));
		mitccrh.setS(s);
		io->flush();
		block pad[2*ot_bsize];
		for(int64_t i = 0; i < count; i+=ot_bsize) {
			for(int64_t j = i; j < std::min(i+ot_bsize, count); ++j) {
				pad[2*(j-i)] = data[j];
				pad[2*(j-i)+1] = data[j] ^ Delta;
			}
			mitccrh.hash<ot_bsize, 2>(pad);
			for(int64_t j = i; j < std::min(i+ot_bsize, count); ++j) {
				pad[2*(j-i)] = pad[2*(j-i)] ^ arr_data1[j].as_m128i();
				pad[2*(j-i)+1] = pad[2*(j-i)+1] ^ arr_data2[j].as_m128i();
			}
			io->write(pad, 2*sizeof(block)* std::min(ot_bsize,count-i));
		}
		delete[] data;
	}

	void recv_n(qst::types::Data* arr_data, const bool* choices,  const std::int64_t count) {
		auto data = new block[count];
		recv_cot(data, choices, count);
		block s;
		// io->recv_block(&s,1);
		io->read(&s, 1 * sizeof(block));
		mitccrh.setS(s);
		io->flush();

		block res[2*ot_bsize];
		block pad[ot_bsize];
		for(int64_t i = 0; i < count; i+=ot_bsize) {
			memcpy(pad, data+i, std::min(ot_bsize,count-i)*sizeof(block));
			mitccrh.hash<ot_bsize, 1>(pad);
			io->read(res, 2*sizeof(block)* std::min(ot_bsize,count-i));
			for(int64_t j = 0; j < ot_bsize and j < count-i; ++j) {
				arr_data[i+j] = qst::types::Data{res[2*j+choices[i+j]] ^ pad[j]};
			}
		}
		delete[] data;
	}

	void send_rot(block* data0, block* data1, int64_t length) {
		send_cot(data0, length);
		block s; prg.random_block(&s, 1);
		// io->send_block(&s,1);
		io->wite(&s, 1 * sizeof(block));
		mitccrh.setS(s);
		io->flush();

		block pad[ot_bsize*2];
		for(int64_t i = 0; i < length; i+=ot_bsize) {
			for(int64_t j = i; j < std::min(i+ot_bsize, length); ++j) {
				pad[2*(j-i)] = data0[j];
				pad[2*(j-i)+1] = data0[j] ^ Delta;
			}
			mitccrh.hash<ot_bsize, 2>(pad);
			for(int64_t j = i; j < std::min(i+ot_bsize, length); ++j) {
				data0[j] = pad[2*(j-i)];
				data1[j] = pad[2*(j-i)+1];
			}
		}
	}

	void recv_rot(block* data, const bool* r, int64_t length) {
		recv_cot(data, r, length);
		block s;
		// io->recv_block(&s,1);
		io->read(&s, 1 * sizeof(block));
		mitccrh.setS(s);
		io->flush();
		block pad[ot_bsize];
		for(int64_t i = 0; i < length; i+=ot_bsize) {
			memcpy(pad, data+i,  std::min(ot_bsize,length-i)*sizeof(block));
			mitccrh.hash<ot_bsize, 1>(pad);
			memcpy(data+i, pad, std::min(ot_bsize,length-i)*sizeof(block));
		}
	}
};
}





// #include "mpc/protocols/ot/ot.h"
// #include "crypto/prng_aes.h"
// #include "utils/mitccrh.h"

// using block = __m128i;


// namespace qst::mpc::protocols {
//     static constexpr int64_t ot_bsize = 8;
//
//     template<typename IO_T>
//     /**
//     * @brief Correlated OT class
//     *
//     */
//     class COT : public OT<IO_T> {
//     public:
//         virtual void send_cot(block *data0, int64_t length) = 0;
//
//         virtual void recv_cot(block *data, const bool *b, int64_t length) = 0;
//
//         // void send(const block* data0, const block* data1, int64_t length) override;
//         // void recv(block* data, const bool* r, int64_t length) override;
//
//         void send(const types::Data &data1, const types::Data &data2) override {
//         }
//
//         types::Data recv(const bool choice) override {
//         }
//
//         void send_n(const types::Data *arr_data1, const types::Data *arr_data2, int count) override {
//         };
//
//         void recv_n(types::Data *arr_data, const bool *choices, int count) override {
//         }
//
//         // void send_nn(const block *data0, const block *data1, int64_t length) {
//         //     block * data = new block[length];
//         //     send_cot(data, length);
//         //     block s;
//         //     m_prg.random_block(&s, 1);
//         //     // m_io->send_block(&s,1);
//         //
//         //     m_io->write(&s, 1 * sizeof(block));
//         //     m_io->flush();
//         //
//         //     m_mitccrh.setS(s);
//         //     std::cout << "after send\n";
//         //
//         //     block pad[2*ot_bsize];
//         //     for(int64_t i = 0; i < length; i+=ot_bsize) {
//         //         for(int64_t j = i; j < std::min(i+ot_bsize, length); ++j) {
//         //             pad[2*(j-i)] = data[j];
//         //             pad[2*(j-i)+1] = data[j] ^ m_Delta;
//         //         }
//         //         m_mitccrh.hash<ot_bsize, 2>(pad);
//         //         for(int64_t j = i; j < std::min(i+ot_bsize, length); ++j) {
//         //             pad[2*(j-i)] = pad[2*(j-i)] ^ data0[j];
//         //             pad[2*(j-i)+1] = pad[2*(j-i)+1] ^ data1[j];
//         //         }
//         //         // m_io->send_data(pad, 2*sizeof(block)* std::min(ot_bsize,length-i));
//         //         m_io->write(pad, 2*sizeof(block)* std::min(ot_bsize,length-i));
//         //     }
//         //     delete[] data;
//         // }
//         //
//         // void recv_nn(block *data, const bool *r, int64_t length) {
//         //     recv_cot(data, r, length);
//         //     std::cout << "After recv COT\n";
//         //
//         //     block s;
//         //     // m_io->recv_block(&s,1);
//         //
//         //     m_io->read(&s,1 * sizeof(block));
//         //
//         //
//         //     std::cout << "after read \n";
//         //
//         //
//         //     m_mitccrh.setS(s);
//         //     m_io->flush();
//         //
//         //
//         //
//         //     block res[2*ot_bsize];
//         //     block pad[ot_bsize];
//         //     for(int64_t i = 0; i < length; i+=ot_bsize) {
//         //         memcpy(pad, data+i, std::min(ot_bsize,length-i)*sizeof(block));
//         //         m_mitccrh.hash<ot_bsize, 1>(pad);
//         //         // io->recv_data(res, 2*sizeof(block)*std::min(ot_bsize,length-i));
//         //         m_io->read(res, 2*sizeof(block)*std::min(ot_bsize,length-i));
//         //         for(int64_t j = 0; j < ot_bsize and j < length-i; ++j) {
//         //             data[i+j] = res[2*j+r[i+j]] ^ pad[j];
//         //         }
//         //     }
//         // }
//
//
//         // void send_nn(const block* arr_data1, const block* arr_data2, int count) {
//         //     block * data = new block[count];
//         //     send_cot(data, count);
//         //
//         //     block s; m_prg.random_block(&s, 1);
//         //     // io->send_block(&s,1);
//         //
//         //     io->write(&s, 1 * sizeof(block)); //todo
//         //     m_mitccrh.setS(s);
//         //     io->flush();
//         //     block pad[2*ot_bsize];
//         //
//         //     for(int64_t i = 0; i < count; i+=ot_bsize) {
//         //         for(int64_t j = i; j < std::min(i+ot_bsize, static_cast<long>(count)); ++j) {
//         //             pad[2*(j-i)] = data[j];
//         //             pad[2*(j-i)+1] = data[j] ^ m_Delta;
//         //         }
//         //         m_mitccrh.hash<ot_bsize, 2>(pad);
//         //         for(int64_t j = i; j < std::min(i+ot_bsize, static_cast<long>(count)); ++j) {
//         //             pad[2*(j-i)] = pad[2*(j-i)] ^ arr_data1[j];
//         //             pad[2*(j-i)+1] = pad[2*(j-i)+1] ^ arr_data2[j];
//         //         }
//         //         // io->send_data(pad, 2*sizeof(block)* std::min(ot_bsize,count-i));
//         //         io->write(pad, 2*sizeof(block)* std::min(ot_bsize,count-i));
//         //     }
//         //     delete[] data;
//         // }
//         //
//         // void recv_nn(block *arr_data, const bool *choices, const int count) {
//         //     recv_cot(arr_data, choices, count);
//         //     block s;
//         //     // io->recv_block(&s,1 * sizeof(block));
//         //     io->read(&s,1 * sizeof(block));
//         //     m_mitccrh.setS(s);
//         //     io->flush();
//         //
//         //     block res[2*ot_bsize];
//         //     block pad[ot_bsize];
//         //     for(int64_t i = 0; i < count; i+=ot_bsize) {
//         //         memcpy(pad, arr_data+i, std::min(ot_bsize,count-i)*sizeof(block));
//         //         m_mitccrh.hash<ot_bsize, 1>(pad);
//         //         // io->recv_data(res, 2*sizeof(block)*std::min(ot_bsize,count-i));
//         //         io->read(res, 2*sizeof(block)*std::min(ot_bsize,count-i)); //todo
//         //         for(int64_t j = 0; j < ot_bsize and j < count-i; ++j) {
//         //             arr_data[i+j] = res[2*j+choices[i+j]] ^ pad[j];
//         //         }
//         //     }
//         // }
//
//         // private:
//         IO_T *m_io{};
//         emp::MITCCRH<ot_bsize> m_mitccrh{};
//         block m_Delta{};
//         crypto::prng::PrngAes m_prg{}; // define abstract prng and template
//     };
// }

#endif
