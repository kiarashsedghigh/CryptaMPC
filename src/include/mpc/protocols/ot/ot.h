#ifndef OT_H
#define OT_H

#include <types.h>
#include <openssl/evp.h>
#include <cstdint>

namespace qst::mpc::protocols {
    /**
     * @brief
     *  THis
     *
     * @author
     */
    template <typename IO>
    class OT {
    public:
        virtual ~OT() {}

        virtual void send(const types::Data &data1, const types::Data &data2) = 0;

        virtual types::Data recv(bool choice) = 0;
    };
}


#endif
