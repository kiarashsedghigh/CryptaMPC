#include <iostream>

#include "crypto/prng_aes.h"

int main() {

    qst::crypto::prng::PrngAes prng {};

    block t;

    prng.random_block(&t , 1);

    for (int i = 0; i < 16; i++) {
        std::cout << std::hex << ((char *)(&t))[i];
    }
    std::cout << std::endl;

}