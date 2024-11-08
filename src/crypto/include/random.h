#ifndef RANDOM_H
#define RANDOM_H
#include <cstdint>
#include <ostream>
#include <vector>
#include <string>

namespace cryptampc::crypto {
    class Random {
    public:
        friend std::ostream& operator<<(std::ostream& os, const Random& randObj);

        Random()= default;
        explicit Random(int len);

        const std::vector<uint8_t>& get_bytes() const {return bytes;}
        std::string as_string() const;
        std::string as_hex_string() const;

    private:
        std::vector<uint8_t> bytes;
    };
}



#endif

