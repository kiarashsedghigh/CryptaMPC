#ifndef OT_H
#define OT_H

#include <types.h>

namespace qst::mpc::protocols {
    template<typename IO_T>
    /**
     * @brief Parent class for various OT protocols.
     */
    class OT {
    public:
        virtual ~OT() = default;

        /**
         * @brief Sender posses two data to transmit in the OT
         * @param data1 First data
         * @param data2 Second Data
         */
        virtual void send(const types::Data &data1, const types::Data &data2) = 0;

        /**
         * @brief Sender posses list of data tuples ([(data11, data12), (data21,data22) ,,,]) to transmit in the OT
         * @param arr_data1 List of first element of tuples
         * @param arr_data2 List of second element of tuples
         * @param count Number of data tuples
         */
        virtual void send_n(const types::Data* arr_data1, const types::Data* arr_data2, int count) = 0;

        /**
         * Receiver retrieves the data indexed by the choice bit
         * @param choice Choice bit
         * @return Retrieved Data
         */
        virtual types::Data recv(bool choice) = 0;


        /**
         * Receiver retrieves multiple data from list of data tuples ([(data11, data12), (data21,data22) ,,,])
         * indexed by the choice bits
         * @param arr_data Pointer to the list of data objects to write the data back
         * @param choices Choice bits
         * @param count Number of choices
         * @return Retrieved Data
         */
        virtual void recv_n(types::Data* arr_data, const bool* choices,  int count) = 0;

    };
}


#endif
