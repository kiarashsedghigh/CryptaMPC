#ifndef IO_H
#define IO_H

#include <cstdint>
#include "types.h"

namespace qst::io {
    class IO {
    public:
        virtual ~IO() = default;

    private:
        virtual void write(const types::Data &data, std::uint64_t size) = 0;
        virtual std::uint64_t read(const types::Data &buffer) = 0;


    };
}



#endif