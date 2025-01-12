#include <iostream>
#include <vector>
#include <thread>
#include <xmmintrin.h>
#include <boost/format.hpp>
#include <immintrin.h> // For AVX intrinsics



int main() {

    // Define two arrays of 4 64-bit integers (aligned to 32 bytes)
    alignas(32) std::int64_t a[4] = {1, 2, 3, 4};
    alignas(32) std::int64_t b[4] = {10, 20, 30, 40};
    alignas(32) std::int64_t result[4];

    // Load the arrays into AVX2 registers
    __m256i vecA = _mm256_load_si256(reinterpret_cast<const __m256i*>(a));
    __m256i vecB = _mm256_load_si256(reinterpret_cast<const __m256i*>(b));

    // Perform element-wise addition
    __m256i vecResult = _mm256_sub_epi64(vecA, vecB);

    // Store the result back to memory
    _mm256_store_si256(reinterpret_cast<__m256i*>(result), vecResult);

    // Print the result
    std::cout << "Result of vector addition:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
