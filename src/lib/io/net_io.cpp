#include "io/net_io.h"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <utility>


namespace qst::io {
    NetIO::NetIO(std::string ip, const std::int32_t port, const bool is_server)
        : m_port{port}, m_ip{std::move(ip)}, m_is_server{is_server} {
        if (port < 0 || port > 65535)
            throw std::runtime_error("[NETWORK]: Invalid port number (0-65535)!");

        if (m_is_server) {
            sockaddr_in dest{};
            sockaddr_in serv{};

            socklen_t socket_size{sizeof(struct sockaddr_in)};

            memset(&serv, 0, sizeof(serv));
            serv.sin_family = AF_INET;
            serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
            serv.sin_port = htons(port); /* set the server port number */
            m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
            int reuse{1};
            setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR, (const char *) &reuse, sizeof(reuse));

            if (bind(m_server_socket, (struct sockaddr *) &serv, sizeof(struct sockaddr)) < 0) {
                perror("[NETWORK]: Port binding");
                exit(1);
            }
            if (listen(m_server_socket, 1) < 0) {
                perror("[NETWORK]: Listening issues");
                exit(1);
            }
            m_connection_socket = accept(m_server_socket, (struct sockaddr *) &dest, &socket_size);
            close(m_server_socket);
        } else {
            sockaddr_in dest{};
            memset(&dest, 0, sizeof(dest));
            dest.sin_family = AF_INET;
            dest.sin_addr.s_addr = inet_addr(m_ip.c_str());
            dest.sin_port = htons(port);

            while (1) {
                m_connection_socket = socket(AF_INET, SOCK_STREAM, 0);

                if (connect(m_connection_socket, (struct sockaddr *) &dest, sizeof(struct sockaddr)) == 0) {
                    break;
                }

                close(m_connection_socket);
                usleep(1000);
            }
        }
        set_no_delay();
        m_stream = fdopen(m_connection_socket, "wb+");
        m_buffer = new char[NETWORK_BUFFER_SIZE];
        memset(m_buffer, 0, NETWORK_BUFFER_SIZE);
        setvbuf(m_stream, m_buffer, _IOFBF, NETWORK_BUFFER_SIZE);
    }

    NetIO::~NetIO() {
        flush();
        fclose(m_stream);
        delete[] m_buffer;
    }



    void NetIO::set_delay() const {
        constexpr int zero{0};
        setsockopt(m_connection_socket,IPPROTO_TCP,TCP_NODELAY, &zero, sizeof(zero));
    }



    void NetIO::set_no_delay() const {
        constexpr int one{1};
        setsockopt(m_connection_socket,IPPROTO_TCP,TCP_NODELAY, &one, sizeof(one));
    }

    void NetIO::sync() {
        std::int8_t tmp{};
        if (m_is_server) {
            write(&tmp, 1);
            read(&tmp, 1);
        } else {
            read(&tmp, 1);
            write(&tmp, 1);
            flush();
        }
    }

    void NetIO::flush() const {
        fflush(m_stream);
    }

    void NetIO::write(const void *input, const std::uint64_t len) {
        size_t sent{0};
        while (sent < len) {
            if (size_t res = fwrite(sent + input, 1, len - sent, m_stream); res > 0)
                sent += res;
            else
                throw std::runtime_error("[NETWORK]: Cannot send data");
        }
        m_has_sent = true;
    }

    void NetIO::read(void *buffer, const std::uint64_t len) {
        if (m_has_sent)
            fflush(m_stream);
        m_has_sent = false;
        size_t sent = 0;

        while (sent < len) {
            if (size_t res = fread(sent + buffer, 1, len - sent, m_stream); res > 0)
                sent += res;
            else
                throw std::runtime_error("[NETWORK]: Cannot read data");
        }
    }
}
