#ifndef EMP_AG2PC_2PC_H__
#define EMP_AG2PC_2PC_H__
#include <emp-tool/emp-tool.h>
#include "preprocessing.h"
#include "helper.h"


#include "mpc/protocols/gc/circuits/bristol_circuit.h"

using namespace std;

namespace emp {
template<typename T>
class C2PC { public:
	const static int SSP = 5;//5*8 in fact...
	const block MASK = makeBlock(0x0ULL, 0xFFFFFULL);
	Fpre<T>* fpre = nullptr;
	block * mac = nullptr;
	block * key = nullptr;

	block * preprocess_mac = nullptr;
	block * preprocess_key = nullptr;

	block * sigma_mac = nullptr;
	block * sigma_key = nullptr;

	block * labels = nullptr;

	bool * mask = nullptr;
	qst::mpc::protocols::gc::BristolCircuit * cf;
	T * io;
	int num_ands = 0;
	int party, total_pre;
	C2PC(T * io, int party, qst::mpc::protocols::gc::BristolCircuit * cf) {
		this->party = party;
		this->io = io;
		this->cf = cf;
		for(int i = 0; i < cf->m_num_gates; ++i) {
			if (cf->m_gates_vector[4*i+3] == AND_GATE)
				++num_ands;
		}
		total_pre = cf->m_input_lhs_size + cf->m_input_rhs_size + num_ands;
		fpre = new Fpre<T>(io, party, num_ands);

		key = new block[cf->m_num_wires];
		mac = new block[cf->m_num_wires];

		preprocess_mac = new block[total_pre];
		preprocess_key = new block[total_pre];

		//sigma values in the paper
		sigma_mac = new block[num_ands];
		sigma_key = new block[num_ands];

		labels = new block[cf->m_num_wires];

		mask = new bool[cf->m_input_lhs_size + cf->m_input_rhs_size];
	}
	~C2PC(){
		delete[] key;
		delete[] mac;
		delete[] mask;
		delete[] GT;
		delete[] GTK;
		delete[] GTM;

		delete[] preprocess_mac;
		delete[] preprocess_key;

		delete[] sigma_mac;
		delete[] sigma_key;

		delete[] labels;
		delete fpre;
	}
	PRG prg;
	PRP prp;
	block (* GT)[4][2] = nullptr;
	block (* GTK)[4] = nullptr;
	block (* GTM)[4] = nullptr;

	//not allocation
	block * ANDS_mac = nullptr;
	block * ANDS_key = nullptr;
	void function_independent() {
		if(party == ALICE)
			prg.random_block(labels, cf->m_num_wires);

		fpre->refill();
		ANDS_mac = fpre->MAC_res;
		ANDS_key = fpre->KEY_res;

		if(fpre->party == ALICE) {
			fpre->abit1[0]->send_dot(preprocess_key, total_pre);
			fpre->abit2[0]->recv_dot(preprocess_mac, total_pre);
		} else {
			fpre->abit1[0]->recv_dot(preprocess_mac,  total_pre);
			fpre->abit2[0]->send_dot(preprocess_key, total_pre);
		}
		memcpy(key, preprocess_key, (cf->m_input_lhs_size+cf->m_input_rhs_size)*sizeof(block));
		memcpy(mac, preprocess_mac, (cf->m_input_lhs_size+cf->m_input_rhs_size)*sizeof(block));
	}

	void function_dependent() {
		int ands = cf->m_input_lhs_size+cf->m_input_rhs_size;
		bool * x1 = new bool[num_ands];
		bool * y1 = new bool[num_ands];
		bool * x2 = new bool[num_ands];
		bool * y2 = new bool[num_ands];

		for(int i = 0; i < cf->m_num_gates; ++i) {
			if (cf->m_gates_vector[4*i+3] == AND_GATE) {
				key[cf->m_gates_vector[4*i+2]] = preprocess_key[ands];
				mac[cf->m_gates_vector[4*i+2]] = preprocess_mac[ands];
				++ands;
			}
		}

		for(int i = 0; i < cf->m_num_gates; ++i) {
			if (cf->m_gates_vector[4*i+3] == XOR_GATE) {
				key[cf->m_gates_vector[4*i+2]] = key[cf->m_gates_vector[4*i]] ^ key[cf->m_gates_vector[4*i+1]];
				mac[cf->m_gates_vector[4*i+2]] = mac[cf->m_gates_vector[4*i]] ^ mac[cf->m_gates_vector[4*i+1]];
				if(party == ALICE)
					labels[cf->m_gates_vector[4*i+2]] = labels[cf->m_gates_vector[4*i]] ^ labels[cf->m_gates_vector[4*i+1]];
			} else if (cf->m_gates_vector[4*i+3] == NOT_GATE) {
				if(party == ALICE)
					labels[cf->m_gates_vector[4*i+2]] = labels[cf->m_gates_vector[4*i]] ^ fpre->Delta;
				key[cf->m_gates_vector[4*i+2]] = key[cf->m_gates_vector[4*i]];
				mac[cf->m_gates_vector[4*i+2]] = mac[cf->m_gates_vector[4*i]];
			}
		}

		ands = 0;
		for(int i = 0; i < cf->m_num_gates; ++i) {
			if (cf->m_gates_vector[4*i+3] == AND_GATE) {
				x1[ands] = getLSB(mac[cf->m_gates_vector[4*i]] ^ANDS_mac[3*ands]);
				y1[ands] = getLSB(mac[cf->m_gates_vector[4*i+1]]^ANDS_mac[3*ands+1]);
				ands++;
			}
		}
		if(party == ALICE) {
			helper_send_bool(io, x1, num_ands);
			helper_send_bool(io, y1, num_ands);
			helper_recv_bool(io, x2, num_ands);
			helper_recv_bool(io, y2, num_ands);
		} else {
			helper_recv_bool(io, x2, num_ands);
			helper_recv_bool(io, y2, num_ands);
			helper_send_bool(io, x1, num_ands);
			helper_send_bool(io, y1, num_ands);
		}
		io->flush();
		for(int i = 0; i < num_ands; ++i) {
			x1[i] = logic_xor(x1[i], x2[i]);
			y1[i] = logic_xor(y1[i], y2[i]);
		}
		ands = 0;
		for(int i = 0; i < cf->m_num_gates; ++i) {
			if (cf->m_gates_vector[4*i+3] == AND_GATE) {
				sigma_mac[ands] = ANDS_mac[3*ands+2];
				sigma_key[ands] = ANDS_key[3*ands+2];

				if(x1[ands]) {
					sigma_mac[ands] = sigma_mac[ands] ^ ANDS_mac[3*ands+1];
					sigma_key[ands] = sigma_key[ands] ^ ANDS_key[3*ands+1];
				}
				if(y1[ands]) {
					sigma_mac[ands] = sigma_mac[ands] ^ ANDS_mac[3*ands];
					sigma_key[ands] = sigma_key[ands] ^ ANDS_key[3*ands];
				}
				if(x1[ands] and y1[ands]) {
					if(party == ALICE)
						sigma_key[ands] = sigma_key[ands] ^ fpre->ZDelta;
					else
						sigma_mac[ands] = sigma_mac[ands] ^ fpre->one;
				}

				ands++;
			}
		}//sigma_[] stores the and of input wires to each AND gates

		delete[] fpre->MAC;
		delete[] fpre->KEY;
		fpre->MAC = nullptr;
		fpre->KEY = nullptr;
		GT = new block[num_ands][4][2];
		GTK = new block[num_ands][4];
		GTM = new block[num_ands][4];

		ands = 0;
		block H[4][2];
		block K[4], M[4];
		for(int i = 0; i < cf->m_num_gates; ++i) {
			if(cf->m_gates_vector[4*i+3] == AND_GATE) {
				M[0] = sigma_mac[ands] ^ mac[cf->m_gates_vector[4*i+2]];
				M[1] = M[0] ^ mac[cf->m_gates_vector[4*i]];
				M[2] = M[0] ^ mac[cf->m_gates_vector[4*i+1]];
				M[3] = M[1] ^ mac[cf->m_gates_vector[4*i+1]];
				if(party == BOB)
					M[3] = M[3] ^ fpre->one;

				K[0] = sigma_key[ands] ^ key[cf->m_gates_vector[4*i+2]];
				K[1] = K[0] ^ key[cf->m_gates_vector[4*i]];
				K[2] = K[0] ^ key[cf->m_gates_vector[4*i+1]];
				K[3] = K[1] ^ key[cf->m_gates_vector[4*i+1]];
				if(party == ALICE)
					K[3] = K[3] ^ fpre->ZDelta;

				if(party == ALICE) {
					Hash(H, labels[cf->m_gates_vector[4*i]], labels[cf->m_gates_vector[4*i+1]], i);
					for(int j = 0; j < 4; ++j) {
						H[j][0] = H[j][0] ^ M[j];
						H[j][1] = H[j][1] ^ K[j] ^ labels[cf->m_gates_vector[4*i+2]];
						if(getLSB(M[j]))
							H[j][1] = H[j][1] ^fpre->Delta;
#ifdef __debug
						check2(M[j], K[j]);
#endif
					}
					for(int j = 0; j < 4; ++j ) {
						send_partial_block<T, SSP>(io, &H[j][0], 1);
						helper_send_block(io,&H[j][1], 1);
					}
				} else {
					memcpy(GTK[ands], K, sizeof(block)*4);
					memcpy(GTM[ands], M, sizeof(block)*4);
#ifdef __debug
					for(int j = 0; j < 4; ++j)
						check2(M[j], K[j]);
#endif
					for(int j = 0; j < 4; ++j ) {
						recv_partial_block<T, SSP>(io, &GT[ands][j][0], 1);
						helper_recv_block(io,&GT[ands][j][1], 1);
					}
				}
				++ands;
			}
		}
		delete[] x1;
		delete[] x2;
		delete[] y1;
		delete[] y2;

		block tmp;
		if(party == ALICE) {
			send_partial_block<T, SSP>(io, mac, cf->m_input_lhs_size);
			for(int i = cf->m_input_lhs_size; i < cf->m_input_lhs_size+cf->m_input_rhs_size; ++i) {
				recv_partial_block<T, SSP>(io, &tmp, 1);
				block ttt = key[i] ^ fpre->Delta;
				ttt =  ttt & MASK;
				block mask_key = key[i] & MASK;
				tmp =  tmp & MASK;
				if(cmpBlock(&tmp, &mask_key, 1))
					mask[i] = false;
				else if(cmpBlock(&tmp, &ttt, 1))
					mask[i] = true;
				else cout <<"no match! ALICE\t"<<i<<endl;
			}
		} else {
			for(int i = 0; i < cf->m_input_lhs_size; ++i) {
				recv_partial_block<T, SSP>(io, &tmp, 1);
				block ttt = key[i] ^ fpre->Delta;
				ttt =  ttt & MASK;
				tmp =  tmp & MASK;
				block mask_key = key[i] & MASK;
				if(cmpBlock(&tmp, &mask_key, 1)) {
					mask[i] = false;
				} else if(cmpBlock(&tmp, &ttt, 1)) {
					mask[i] = true;
				}
				else cout <<"no match! BOB\t"<<i<<endl;
			}

			send_partial_block<T, SSP>(io, mac+cf->m_input_lhs_size, cf->m_input_rhs_size);
		}
		io->flush();
	}

	void online (const bool * input, bool * output, bool alice_output = false) {
		uint8_t * mask_input = new uint8_t[cf->m_num_wires];
		memset(mask_input, 0, cf->m_num_wires);
		block tmp;
#ifdef __debug
		for(int i = 0; i < cf->m_input_lhs_size+cf->m_input_rhs_size; ++i)
			check2(mac[i], key[i]);
#endif
		if(party == ALICE) {
			for(int i = cf->m_input_lhs_size; i < cf->m_input_lhs_size+cf->m_input_rhs_size; ++i) {
				mask_input[i] = logic_xor(input[i], getLSB(mac[i]));
				mask_input[i] = logic_xor(mask_input[i], mask[i]);
			}
			helper_recv_data(io,mask_input, cf->m_input_lhs_size);
			helper_send_data(io,mask_input+cf->m_input_lhs_size, cf->m_input_rhs_size);
			for(int i = 0; i < cf->m_input_lhs_size + cf->m_input_rhs_size; ++i) {
				tmp = labels[i];
				if(mask_input[i]) tmp = tmp ^ fpre->Delta;
				helper_send_block(io,&tmp, 1);
			}
			//send output mask data
			send_partial_block<T, SSP>(io, mac+cf->m_num_wires - cf->m_output_size, cf->m_output_size);
		} else {
			for(int i = 0; i < cf->m_input_lhs_size; ++i) {
				mask_input[i] = logic_xor(input[i], getLSB(mac[i]));
				mask_input[i] = logic_xor(mask_input[i], mask[i]);
			}
			helper_send_data(io,mask_input, cf->m_input_lhs_size);
			helper_recv_data(io,mask_input+cf->m_input_lhs_size, cf->m_input_rhs_size);
			helper_recv_block(io,labels, cf->m_input_lhs_size + cf->m_input_rhs_size);
		}
		int ands = 0;
		if(party == BOB) {
			for(int i = 0; i < cf->m_num_gates; ++i) {
				if (cf->m_gates_vector[4*i+3] == XOR_GATE) {
					labels[cf->m_gates_vector[4*i+2]] = labels[cf->m_gates_vector[4*i]] ^ labels[cf->m_gates_vector[4*i+1]];
					mask_input[cf->m_gates_vector[4*i+2]] = logic_xor(mask_input[cf->m_gates_vector[4*i]], mask_input[cf->m_gates_vector[4*i+1]]);
				} else if (cf->m_gates_vector[4*i+3] == AND_GATE) {
					int index = 2*mask_input[cf->m_gates_vector[4*i]] + mask_input[cf->m_gates_vector[4*i+1]];
					block H[2];
					Hash(H, labels[cf->m_gates_vector[4*i]], labels[cf->m_gates_vector[4*i+1]], i, index);
					GT[ands][index][0] = GT[ands][index][0] ^ H[0];
					GT[ands][index][1] = GT[ands][index][1] ^ H[1];

					block ttt = GTK[ands][index] ^ fpre->Delta;
					ttt =  ttt & MASK;
					GTK[ands][index] =  GTK[ands][index] & MASK;
					GT[ands][index][0] =  GT[ands][index][0] & MASK;

					if(cmpBlock(&GT[ands][index][0], &GTK[ands][index], 1))
						mask_input[cf->m_gates_vector[4*i+2]] = false;
					else if(cmpBlock(&GT[ands][index][0], &ttt, 1))
						mask_input[cf->m_gates_vector[4*i+2]] = true;
					else 	cout <<ands <<" no match GT!"<<endl;
					mask_input[cf->m_gates_vector[4*i+2]] = logic_xor(mask_input[cf->m_gates_vector[4*i+2]], getLSB(GTM[ands][index]));

					labels[cf->m_gates_vector[4*i+2]] = GT[ands][index][1] ^ GTM[ands][index];
					ands++;
				} else {
					mask_input[cf->m_gates_vector[4*i+2]] = not mask_input[cf->m_gates_vector[4*i]];
					labels[cf->m_gates_vector[4*i+2]] = labels[cf->m_gates_vector[4*i]];
				}
			}
		}
		if (party == BOB) {
			bool * o = new bool[cf->m_output_size];
			for(int i = 0; i < cf->m_output_size; ++i) {
				block tmp;
				recv_partial_block<T, SSP>(io, &tmp, 1);
				tmp =  tmp & MASK;

				block ttt = key[cf->m_num_wires - cf->m_output_size + i] ^ fpre->Delta;
				ttt =  ttt & MASK;
				key[cf->m_num_wires - cf->m_output_size + i] = key[cf->m_num_wires - cf->m_output_size + i] & MASK;

				if(cmpBlock(&tmp, &key[cf->m_num_wires - cf->m_output_size + i], 1))
					o[i] = false;
				else if(cmpBlock(&tmp, &ttt, 1))
					o[i] = true;
				else 	cout <<"no match output label!"<<endl;
			}
			for(int i = 0; i < cf->m_output_size; ++i) {
				output[i] = logic_xor(o[i], mask_input[cf->m_num_wires - cf->m_output_size + i]);
				output[i] = logic_xor(output[i], getLSB(mac[cf->m_num_wires - cf->m_output_size + i]));
			}
			delete[] o;
			if(alice_output) {
				send_partial_block<T, SSP>(io, mac+cf->m_num_wires - cf->m_output_size, cf->m_output_size);
				send_partial_block<T, SSP>(io, labels+cf->m_num_wires - cf->m_output_size, cf->m_output_size);
				helper_send_data(io,mask_input + cf->m_num_wires - cf->m_output_size, cf->m_output_size);
				io->flush();
			}
		} else {//ALICE
			if(alice_output) {
				block * tmp_mac = new block[cf->m_output_size];
				block * tmp_label = new block[cf->m_output_size];
				bool * tmp_mask_input = new bool[cf->m_output_size];
				recv_partial_block<T, SSP>(io, tmp_mac, cf->m_output_size);
				recv_partial_block<T, SSP>(io, tmp_label, cf->m_output_size);
				helper_recv_data(io,tmp_mask_input, cf->m_output_size);
				io->flush();
				for(int i = 0; i < cf->m_output_size; ++i) {
					block tmp = tmp_mac[i];
					tmp =  tmp & MASK;

					block ttt = key[cf->m_num_wires - cf->m_output_size + i] ^ fpre->Delta;
					ttt =  ttt & MASK;
					key[cf->m_num_wires - cf->m_output_size + i] = key[cf->m_num_wires - cf->m_output_size + i] & MASK;

					if(cmpBlock(&tmp, &key[cf->m_num_wires - cf->m_output_size + i], 1))
						output[i] = false;
					else if(cmpBlock(&tmp, &ttt, 1))
						output[i] = true;
					else 	cout <<"no match output label!"<<endl;
					block mask_label = tmp_label[i];
					if(tmp_mask_input[i])
						mask_label = mask_label ^ fpre->Delta;
					mask_label = mask_label & MASK;
					block masked_labels = labels[cf->m_num_wires - cf->m_output_size + i] & MASK;
					if(!cmpBlock(&mask_label, &masked_labels, 1))
						cout <<"no match output label2!"<<endl;

					output[i] = logic_xor(output[i], tmp_mask_input[i]);
					output[i] = logic_xor(output[i], getLSB(mac[cf->m_num_wires - cf->m_output_size + i]));
				}
				delete[] tmp_mac;
				delete[] tmp_label;
				delete[] tmp_mask_input;
			}

		}
		delete[] mask_input;
	}

	void check(block * MAC, block * KEY, bool * r, int length = 1) {
		if (party == ALICE) {
			helper_send_data(io,r, length*3);
			helper_send_block(io,&fpre->Delta, 1);
			helper_send_block(io,KEY, length*3);
			block DD;helper_recv_block(io,&DD, 1);

			for(int i = 0; i < length*3; ++i) {
				block tmp;helper_recv_block(io,&tmp, 1);
				if(r[i]) tmp = tmp ^ DD;
				if (!cmpBlock(&tmp, &MAC[i], 1))
					cout <<i<<"\tWRONG ABIT!\n";
			}

		} else {
			bool tmp[3];
			for(int i = 0; i < length; ++i) {
				helper_recv_data(io,tmp, 3);
				bool res = (logic_xor(tmp[0], r[3*i] )) and (logic_xor(tmp[1], r[3*i+1]));
				if(res != logic_xor(tmp[2], r[3*i+2]) ) {
					cout <<i<<"\tWRONG!\n";
				}
			}
			block DD;helper_recv_block(io,&DD, 1);

			for(int i = 0; i < length*3; ++i) {
				block tmp;helper_recv_block(io,&tmp, 1);
				if(r[i]) tmp = tmp ^ DD;
				if (!cmpBlock(&tmp, &MAC[i], 1))
					cout <<i<<"\tWRONG ABIT!\n";
			}
			helper_send_block(io,&fpre->Delta, 1);
			helper_send_block(io,KEY, length*3);
		}
		io->flush();
	}

	void check2(block & MAC, block & KEY) {
		if (party == ALICE) {
			helper_send_block(io,&fpre->Delta, 1);
			helper_send_block(io,&KEY, 1);
			block DD;helper_recv_block(io,&DD, 1);
			for(int i = 0; i < 1; ++i) {
				block tmp;helper_recv_block(io,&tmp, 1);
				if(getLSB(MAC)) tmp = tmp ^ DD;
				if (!cmpBlock(&tmp, &MAC, 1))
					cout <<i<<"\tWRONG ABIT!2\n";
			}
		} else {
			block DD;helper_recv_block(io,&DD, 1);
			for(int i = 0; i < 1; ++i) {
				block tmp;helper_recv_block(io,&tmp, 1);
				if(getLSB(MAC)) tmp = tmp ^ DD;
				if (!cmpBlock(&tmp, &MAC, 1))
					cout <<i<<"\tWRONG ABIT!2\n";
			}
			helper_send_block(io,&fpre->Delta, 1);
			helper_send_block(io,&KEY, 1);
		}
		io->flush();
	}

	void Hash(block H[4][2], const block & a, const block & b, uint64_t i) {
		block A[2], B[2];
		A[0] = a; A[1] = a ^ fpre->Delta;
		B[0] = b; B[1] = b ^ fpre->Delta;
		A[0] = sigma(A[0]);
		A[1] = sigma(A[1]);
		B[0] = sigma(sigma(B[0]));
		B[1] = sigma(sigma(B[1]));

		H[0][1] = H[0][0] = A[0] ^ B[0];
		H[1][1] = H[1][0] = A[0] ^ B[1];
		H[2][1] = H[2][0] = A[1] ^ B[0];
		H[3][1] = H[3][0] = A[1] ^ B[1];
		for(uint64_t j = 0; j < 4; ++j) {
			H[j][0] = H[j][0] ^ makeBlock(4*i+j, 0);
			H[j][1] = H[j][1] ^ makeBlock(4*i+j, 1);
		}
		prp.permute_block((block *)H, 8);
	}

	void Hash(block H[2], block a, block b, uint64_t i, uint64_t row) {
		a = sigma(a);
		b = sigma(sigma(b));
		H[0] = H[1] = a ^ b;
		H[0] = H[0] ^ makeBlock(4*i+row, 0);
		H[1] = H[1] ^ makeBlock(4*i+row, 1);
		prp.permute_block((block *)H, 2);
	}

	bool logic_xor(bool a, bool b) {
		return a!= b;
	}
};
}
#endif// C2PC_H__
