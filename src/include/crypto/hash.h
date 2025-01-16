#ifndef HASH_H
#define HASH_H

namespace qst::crypto::hash {
    /**
     * @brief Implementing the base class for hash operations
     */
    class Hash {
    public:
        virtual ~Hash() = default;

        /**
         * @brief Adding more data to the internal buffer before hashing
         *
         * @param input Pointer to the input buffer
         * @param input_length Number of bytes of the input
         */
        virtual void put(const void *input, std::uint32_t input_length) = 0;

        /**
         * @brief Performing the actual hash operation
         *
         * @param output Pointer to the output buffer to which we write the hash
         */
        virtual void digest(void *output) = 0;

        /**
         * @brief Resets the internal status of the hash instance. This is required due to the put() functionality
         */
        virtual void reset() = 0;

    private:
    };
}


#endif //HASH_H
