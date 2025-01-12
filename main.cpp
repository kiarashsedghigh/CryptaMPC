#include <iostream>
#include <vector>
#include <thread>
#include <boost/format.hpp>



int main() {

    std::cout << boost::format("Debug value: %1%\n") % 42;


    return 0;
}
