#include "types.h"

int main() {
    char buff[10] = {0x88, 0x12};
    qst::types::Data data {buff, 2};
    char str [10];
    data.to_hex(str);
    std::cout << str << std::endl;
    std::cout << data.as_hex_string();
}
