#ifndef NET_IO_H
#define NET_IO_H

#include <cstdint>
#include <string>
#include "io/io.h"


#include <iostream>

namespace qst::io {
  class NetIO : public IO {
  public:
    NetIO(std::uint32_t port, const std::string &ip);

    void write(const types::Data &data, std::uint64_t size) override;
    std::uint64_t read(const types::Data &buffer) override;

  private:
    std::uint32_t m_port;
    std::string m_ip;
  };
}


#endif //NET_IO_H