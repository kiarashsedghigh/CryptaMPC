#include <flint/fmpz.h>
#include <flint/fmpz_poly.h>
#include <iostream>
#include <vector>
#include <openssl/rand.h>
#include <gmp.h>

#include "src/crypto/include/random.h"



int main() {

    cryptampc::crypto::Random random {32};


    std::cout << random.as_hex_string();

    // mpz_t mpz_num;
    //
    //
    // fmpz_t num;
    // // fmpz_set_str(num, "18273981241669124", 10);
    // // fmpz_print(num);
    //
    // std::vector<unsigned char> bytes(32);
    // RAND_bytes(bytes.data(), 32);
    //
    //
    // fmpz_set_str(num, static_cast<char *>(bytes.data()), 10);
    //
    //
    //
    // for(int i {}; i < 32; i++) {
    //     std::cout << bytes[i];
    // }

    // Initialize polynomial object
    // fmpz_poly_t poly;
    // fmpz_poly_init(poly);  // Initialize the polynomial
    //
    // // Define the degree of the polynomial
    // long degree = 3;
    //
    // // Set the coefficients (using fmpz_t)
    // fmpz_t coeff;
    // fmpz_init(coeff);
    //
    // // Assign values to coefficients (coefficients are of type fmpz_t)
    // fmpz_set_ui(coeff, 5);  // Set coefficient for x^0 (constant term)
    // fmpz_poly_set_coeff_fmpz(poly, 0, coeff);  // Set poly[0] = 5
    //
    // fmpz_set_ui(coeff, 3);  // Set coefficient for x^1
    // fmpz_poly_set_coeff_fmpz(poly, 1, coeff);  // Set poly[1] = 3
    //
    // fmpz_set_ui(coeff, 7);  // Set coefficient for x^2
    // fmpz_poly_set_coeff_fmpz(poly, 2, coeff);  // Set poly[2] = 7
    //
    // fmpz_set_ui(coeff, 1);  // Set coefficient for x^3
    // fmpz_poly_set_coeff_fmpz(poly, 3, coeff);  // Set poly[3] = 1
    //
    // // Print the polynomial
    // std::cout << "Polynomial: ";
    // fmpz_poly_print_pretty(poly, "x");
    // std::cout << std::endl;
    //
    // fmpz_t res;
    // fmpz_t at {1};
    // fmpz_poly_evaluate_fmpz(res, poly, at);
    //
    // fmpz_t mod {8};
    // fmpz_mod(res, res, mod);
    //
    // fmpz_print(res);
    //


    // std::cout << "----------------------------\n";

    // // Example vector of unsigned char (byte data)
    // unsigned char byte_vector[32]; // Buffer to store random bytes (32 bytes for example)
    // // if (RAND_bytes(byte_vector, sizeof(byte_vector)) != 1)
    // RAND_bytes(byte_vector, sizeof(byte_vector));
    // // std::vector<unsigned char> byte_vector = {1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // 4 bytes
    //
    //
    //
    // // Initialize an fmpz_t variable
    // fmpz_t num;
    // fmpz_init(num);
    //
    // // Loop through the byte vector and set the value in fmpz_t
    // fmpz_set_ui(num, 0);  // Initialize num to 0
    //
    // for (size_t i = 0; i < 32; ++i) {
    //     // Shift the existing number left by 8 bits (1 byte)
    //     fmpz_mul_2exp(num, num, 8);
    //
    //     // Add the current byte
    //     fmpz_add_ui(num, num, byte_vector[i]);
    // }
    // fmpz_print(num);
    //
    //
    // // Clear allocated memory
    // fmpz_clear(coeff);
    // fmpz_poly_clear(poly);

    return 0;
}
