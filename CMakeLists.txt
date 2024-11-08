cmake_minimum_required(VERSION 3.29)
project(CryptaMPC)

set(CMAKE_CXX_STANDARD 23)


link_libraries(flint ntl gmp givaro ssl crypto)
link_directories("/usr/local/lib")

link_directories(/usr/local/lib)


include_directories(src/crypto/include)

add_executable(CryptaMPC
    main.cpp
        src/math/include/algebraic_structures.h
        src/protocols/src/shamir.cpp
        src/protocols/src/shamir2.cpp
        src/crypto/src/random.cpp
        src/crypto/include/random.h)
