# Minimum CMake version required
cmake_minimum_required(VERSION 3.26)

# Project name and language
project(QuantumSafeThreshold LANGUAGES CXX)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src/include)

link_libraries(-lssl -lcrypto)

add_executable(netio
        src/include/io/net_io.h
        main.cpp
        src/lib/io/net_io.cpp
        src/lib/types.cpp
)

add_executable(fileio
        src/include/io/file_io.h
        main.cpp
        src/lib/io/file_io.cpp
        src/lib/types.cpp
        src/include/crypto/hash.h
)

add_executable(sha2
        src/include/crypto/sha2.h
        main.cpp
        src/lib/types.cpp
        src/lib/crypto/sha2.cpp
        src/include/math/group.h
)

