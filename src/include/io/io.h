#ifndef IO_H
#define IO_H

#include <cstdint>

namespace qst::io {
    /**
     * @brief Parent class for various IO channels like Network or File.
     */
    class IO {
    public:
        virtual ~IO() = default;

    private:
        /**
         * @brief Writes bytes into the IO channel
         * @param input Pointer to the input buffer from which we want to write
         * @param len Number of bytes to be written from the buffer
         */
        virtual void write(const void *input, std::uint64_t len) = 0;

        /**
         * @brief Reads bytes from the IO channel
         * @param buffer Pointer to the output buffer to which we want to write
         * @param len Number of bytes to be read from the buffer
         */
        virtual void read(void *buffer, std::uint64_t len) = 0;
    };
}


#endif
