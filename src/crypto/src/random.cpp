#include "random.h"
#include <openssl/rand.h>
#include <iostream>
#include <iomanip>

cryptampc::crypto::Random::Random(const int len) {
    bytes.resize(len);
    if (RAND_bytes(bytes.data(), len) != 1) {
        std::cerr << "Error generating random bytes!" << std::endl;
        throw std::runtime_error("Failed to generate random bytes");
    }
}

std::string cryptampc::crypto::Random::as_string() const {
    std::string str(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    return str;
}

std::string cryptampc::crypto::Random::as_hex_string() const {
    std::ostringstream oss;

    for (uint8_t byte : bytes) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }

    return oss.str();
}

namespace cryptampc::crypto {
    std::ostream& operator<<(std::ostream& os, const cryptampc::crypto::Random& obj){
        for (auto byte: obj.get_bytes()) {
            os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        return os;
    }
}