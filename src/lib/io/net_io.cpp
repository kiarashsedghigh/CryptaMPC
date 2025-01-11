#include "io/net_io.h"

namespace qst::io {
    // NetIO::NetIO(std::uint32_t port, const std::string &ip)
    //     : m_port{port}, m_ip{ip} {
    //
    //
    //
    //
    //     // Debug output
    //     std::cout << "NetIO constructor" << std::endl;
    // }

    void NetIO::write(const types::Data &data, std::uint64_t size) {
        // Debug output
        std::cout << "NetIO send" << std::endl;
    }

    std::uint64_t NetIO::read(const types::Data &buffer) {
        // Debug output
        std::cout << "NetIO recv" << std::endl;
        return 0;
    }

}