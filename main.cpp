#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <iostream>

int main() {
    // Create the curve group for NID_X9_62_prime256v1
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    if (!group) {
        std::cerr << "Failed to create EC_GROUP for NID_X9_62_prime256v1.\n";
        return 1;
    }

    // Get the curve parameters (a and b)
    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* c = BN_new();
    if (!a || !b) {
        std::cerr << "Failed to allocate BIGNUMs.\n";
        EC_GROUP_free(group);
        return 1;
    }

    // Retrieve the curve's a and b parameters
    EC_GROUP_get_curve(group,c, a, b, nullptr);

    // Print a and b in hexadecimal format
    std::cout << "Curve parameters a: " << std::endl;
    BN_print_fp(stdout, a);  // Print 'a' in hex format
    std::cout << std::endl;

    std::cout << "Curve parameters b: " << std::endl;
    BN_print_fp(stdout, b);  // Print 'b' in hex format
    std::cout << std::endl;

    // Clean up
    BN_free(a);
    BN_free(b);
    EC_GROUP_free(group);

    return 0;
}

// #include <openssl/ec.h>
// #include <openssl/obj_mac.h>
// #include <iostream>
// #include <vector>
//
// void print_hex(const std::vector<unsigned char>& data) {
//     for (unsigned char byte : data) {
//         printf("%02x", byte);
//     }
//     printf("\n");
// }
//
// int main() {
//     // Create the curve group for NID_X9_62_prime256v1
//     EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
//     if (!group) {
//         std::cerr << "Failed to create EC_GROUP for NID_X9_62_prime256v1.\n";
//         return 1;
//     }
//
//     // Get the generator point of the curve
//     const EC_POINT* generator = EC_GROUP_get0_generator(group);
//     if (!generator) {
//         std::cerr << "Failed to get generator point.\n";
//         EC_GROUP_free(group);
//         return 1;
//     }
//
//     // Uncompressed format
//     std::vector<unsigned char> uncompressed(65); // 1 + 32 + 32 bytes
//     size_t uncompressed_size = EC_POINT_point2oct(
//         group, generator, POINT_CONVERSION_UNCOMPRESSED,
//         uncompressed.data(), uncompressed.size(), nullptr
//     );
//
//     if (uncompressed_size == 0) {
//         std::cerr << "Failed to encode generator in uncompressed format.\n";
//         EC_GROUP_free(group);
//         return 1;
//     }
//
//     // Compressed format
//     std::vector<unsigned char> compressed(33); // 1 + 32 bytes
//     size_t compressed_size = EC_POINT_point2oct(
//         group, generator, POINT_CONVERSION_COMPRESSED,
//         compressed.data(), compressed.size(), nullptr
//     );
//
//     if (compressed_size == 0) {
//         std::cerr << "Failed to encode generator in compressed format.\n";
//         EC_GROUP_free(group);
//         return 1;
//     }
//
//     // Print the results
//     std::cout << "Generator Point (Uncompressed, 65 bytes):\n";
//     print_hex(uncompressed);
//
//     std::cout << "\nGenerator Point (Compressed, 33 bytes):\n";
//     print_hex(compressed);
//
//     // Clean up
//     EC_GROUP_free(group);
//     return 0;
// }
