#ifndef NET_IO_H
#define NET_IO_H

#include "io/io.h"
#include <string>


namespace qst::io {
    /**
    * @brief Network IO class.
    * \example test_netio.cpp
    *
    */
    class NetIO final : public IO {
    public:
        constexpr static int NETWORK_BUFFER_SIZE{1024 * 1024};

        NetIO(std::string ip, std::int32_t port, bool is_server = false);

        ~NetIO();


        /**
         * @brief Writes bytes into the communication channel
         * @param input Pointer to the input buffer from which we want to write
         * @param len Number of bytes to be written from the buffer
         */
        void write(const void *input, std::uint64_t len) override;


        /**
         * @brief Reads bytes from the communication channel
         * @param buffer Pointer to the output buffer to which we want to write
         * @param len Number of bytes to be read from the buffer
         */
        void read(void *buffer, std::uint64_t len) override;

        /**
         * @brief Synchronize the server and client.
         */
        void sync();

        /**
         * @brief Disabling the Nagle algorithm.
         */
        void set_no_delay() const;

        /**
         * @brief Enabling the Nagle algorithm to minimize the number of small packets sent over the network
         * by buffering data and only sending it once a full packet is available.
         */
        void set_delay() const;

        /**
         * @brief Flushes the communication channel
         * @note The current buffer mode for NetIO is full buffer!
         */
        void flush() const;

    // private: //todo
        std::int32_t m_port{};
        std::string m_ip{};
        bool m_is_server{};


        int m_server_socket{-1};
        int m_connection_socket{-1};
        FILE *m_stream{nullptr};

        char *m_buffer{nullptr};
        bool m_has_sent{false};
    };
}

#endif //NET_IO_H
