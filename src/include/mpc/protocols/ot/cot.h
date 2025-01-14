#ifndef COT_H
#define COT_H


#include "mpc/protocols/ot/ot.h"


namespace qst::mpc::protocols {
    template<typename IO_T>
    /**
    * @brief Correlated OT class
    *
    */
    class COT : public OT<IO_T> {
    public:


    private:
        IO_T * io {};
        MITCCRH<ot_bsize> mitccrh;
        types::Data Delta {};
        PRG prg {};
    };
}

#endif