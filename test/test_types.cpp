#include "types.h"

int main() {

    auto data = qst::types::Data("12ff",4, qst::types::Data::DataInputType::HEX_STRING);
    data.print_as_hex();

}
