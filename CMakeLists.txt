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
        src/include/mpc/protocols/ot/npot.h
        src/include/math/bigint.h
)

add_executable(sha2
        src/include/crypto/sha2.h
        main.cpp
        src/lib/types.cpp
        src/lib/crypto/sha2.cpp
        src/include/math/group.h
)


add_executable(ot
        src/include/mpc/protocols/ot/ot.h
        src/lib/mpc/protocols/ot/ot.cpp
        src/include/crypto/sha2.h
        src/lib/crypto/sha2.cpp
        src/include/math/group.h
        src/include/math/bigint.h
        src/include/types.h
        src/lib/types.cpp
        src/lib/io/net_io.cpp

)

# Check if Doxygen is installed
find_package(Doxygen REQUIRED)

# Set the Doxyfile
# Set the Doxygen paths
set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/src) # Source directory
set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs) # Output directory in project root
set(DOXYGEN_CONFIG_FILE ${CMAKE_BINARY_DIR}/Doxyfile) # Doxyfile in build directory

# Configure a custom Doxyfile
configure_file(${CMAKE_SOURCE_DIR}/Doxyfile ${DOXYGEN_CONFIG_FILE} @ONLY)

# Add a custom target for Doxygen
add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} # Ensure Doxygen runs in project root
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
)