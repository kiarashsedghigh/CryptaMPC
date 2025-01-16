#ifndef HASH_H
#define HASH_H

namespace qst::crypto::hash {
    class Hash {
    public:
        virtual ~Hash() = default;

        virtual void put(const void * input_buffer, std::uint32_t input_length) = 0;
        virtual void digest(void * output_buffer) = 0;

        static constexpr int hash_buffer_size {1024*8};

    private:
    };
}




#endif //HASH_H
