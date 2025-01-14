cmake_minimum_required(VERSION 3.26)

project(QuantumSafeThreshold LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3)

include_directories(src/include)

link_libraries(-lssl -lcrypto)


####################################
############ Build Targets
####################################

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
        mainpage.cpp
        src/include/mpc/protocols/ot/coot.h
        src/include/mpc/protocols/ot/cot.h
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


####################################
############ Test Targets
####################################

add_executable(test_netio
        src/include/io/net_io.h
        test/io/test_netio.cpp
        src/lib/io/net_io.cpp
        src/lib/types.cpp
)

add_executable(test_npot
        src/include/mpc/protocols/ot/ot.h
        test/mpc/ot/test_npot.cpp
        src/include/crypto/sha2.h
        src/lib/crypto/sha2.cpp
        src/include/math/group.h
        src/include/math/bigint.h
        src/include/types.h
        src/lib/types.cpp
        src/lib/io/net_io.cpp

)

add_executable(test_coot
        src/include/mpc/protocols/ot/ot.h
        test/mpc/ot/test_coot.cpp
        src/include/crypto/sha2.h
        src/lib/crypto/sha2.cpp
        src/include/math/group.h
        src/include/math/bigint.h
        src/include/types.h
        src/lib/types.cpp
        src/lib/io/net_io.cpp

)

add_executable(test_gc
        src/include/mpc/protocols/gc/circuits/circuit.h
        src/include/mpc/protocols/gc/circuits/bristol_circuit.h
        src/lib/mpc/gc/circuits/bristol_circuit.cpp
        test/mpc/gc/test_gc.cpp

        #                src/include/crypto/sha2.h
        #                src/lib/crypto/sha2.cpp
        #                src/include/math/group.h
        #                src/include/math/bigint.h
#                        src/include/types.h
#                        src/lib/types.cpp
        #                src/lib/io/net_io.cpp

)

add_executable(test_2pc
        src/include/mpc/protocols/gc/circuits/circuit.h
        src/include/mpc/protocols/gc/circuits/bristol_circuit.h
        src/lib/mpc/gc/circuits/bristol_circuit.cpp
        test/mpc/gc/test_2pc.cpp
        src/include/mpc/protocols/gc/2pc.h

        #                src/include/crypto/sha2.h
        #                src/lib/crypto/sha2.cpp
        #                src/include/math/group.h
        #                src/include/math/bigint.h
        #                        src/include/types.h
        #                        src/lib/types.cpp
        #                src/lib/io/net_io.cpp

)

add_executable(test_abit
        src/include/mpc/protocols/gc/abit.h
        test/mpc/gc/test_abit.cpp

        #                src/include/crypto/sha2.h
        #                src/lib/crypto/sha2.cpp
        #                src/include/math/group.h
        #                src/include/math/bigint.h
        #                        src/include/types.h
        #                        src/lib/types.cpp
        #                src/lib/io/net_io.cpp

)

add_executable(test_prng
        src/include/crypto/prng.h
        test/crypto/test_prng.cpp
        src/lib/crypto/prng.cpp

        #                src/include/crypto/sha2.h
        #                src/lib/crypto/sha2.cpp
        #                src/include/math/group.h
        #                src/include/math/bigint.h
        #                        src/include/types.h
        #                        src/lib/types.cpp
        #                src/lib/io/net_io.cpp

)