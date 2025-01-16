#ifndef FILE_IO_H
#define FILE_IO_H


#include "io/io.h"
#include <cstdint>


namespace qst::io {
    class FileIO : public IO {
    public:
        FileIO()=default;

        // Overridden methods
        void write(const types::Data &data, std::uint64_t size) override;
        std::uint64_t read(const types::Data &buffer) override;

    private:
    };
}

#endif