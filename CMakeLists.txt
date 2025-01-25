cmake_minimum_required(VERSION 3.26)

project(QuantumSafeThreshold LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-O3 -w -maes -msse4.1 -mavx -mavx2)

include_directories(src/include)

link_libraries(-lssl -lcrypto)

####################################
############ File Classification
####################################

####################### Header files
set(HEADERS_NETIO
        src/include/io/net_io.h
)

set(HEADERS_FILEIO
        src/include/io/file_io.h
        src/include/crypto/hash.h
        src/include/mpc/protocols/ot/npot.h
        src/include/math/bigint.h
        src/include/mpc/protocols/ot/coot.h
        src/include/mpc/protocols/ot/cot.h
)

set(HEADERS_SHA2
        src/include/crypto/hash_sha2_256.h
        src/include/math/group.h
)

set(HEADERS_OT
        src/include/mpc/protocols/ot/ot.h
        src/include/mpc/protocols/ot/iknp.h
)

set(HEADERS_TEST
        src/include/mpc/protocols/gc/abit.h
        src/include/mpc/protocols/gc/2pc.h
        src/include/mpc/protocols/gc/circuits/circuit.h
        src/include/mpc/protocols/gc/circuits/bristol_circuit.h
)


file(GLOB HEADERS_UTILS src/include/utils/*.h)


####################### Source files
set(SOURCES_NETIO
        src/lib/io/net_io.cpp
)

set(SOURCES_FILEIO
        src/lib/io/file_io.cpp
        src/lib/types.cpp
)

set(SOURCES_SHA2
        src/lib/crypto/hash_sha2_256.cpp
)

set(SOURCES_OT
        src/lib/mpc/protocols/ot/ot.cpp
)

set(SOURCES_TEST
        src/lib/mpc/gc/circuits/bristol_circuit.cpp
        Misc/backup_2pc.h
)

set(SOURCES_PRNG
        src/lib/crypto/prng_aes.cpp
)

####################################
############ Test Targets
####################################

# Set the output directory for test executables
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/test)

# Create test executables and set the output directory
add_executable(test_netio
        ${HEADERS_NETIO}
        test/io/test_netio.cpp
        ${SOURCES_NETIO}
        ${SOURCES_FILEIO}
)
set_target_properties(test_netio PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})

add_executable(test_npot
        ${HEADERS_OT}
        test/mpc/ot/test_npot.cpp
        ${HEADERS_SHA2}
        ${SOURCES_SHA2}
        ${SOURCES_FILEIO}
        ${SOURCES_NETIO}
)
set_target_properties(test_npot PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})

add_executable(test_coot
        ${HEADERS_OT}
        test/mpc/ot/test_coot.cpp
        ${HEADERS_SHA2}
        ${SOURCES_SHA2}
        ${SOURCES_FILEIO}
        ${SOURCES_NETIO}
)
set_target_properties(test_coot PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})

add_executable(test_cot
        ${HEADERS_OT}
        test/mpc/ot/test_cot.cpp
        ${HEADERS_SHA2}
        ${SOURCES_SHA2}
        ${SOURCES_FILEIO}
        ${SOURCES_NETIO}
        ${SOURCES_PRNG}
        ${HEADERS_UTILS}
)
set_target_properties(test_cot PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})



file(GLOB_RECURSE SOURCES_LIB "src/lib/*.cpp")


add_executable(test_types
        ./src/include/types.h
        ./src/lib/types.cpp
        test/test_types.cpp
)
set_target_properties(test_types PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})



add_executable(test_2pc
        ${HEADERS_TEST}
        ${SOURCES_LIB}
        src/include/mpc/protocols/gc/preprocessing.h
        src/include/mpc/protocols/gc/leaky_deltaot.h
        src/include/mpc/protocols/gc/helper.h
        src/include/mpc/protocols/gc/feq.h

        test/mpc/test_2pc.cpp
)
set_target_properties(test_2pc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})


add_executable(test_prng
        ${HEADERS_SHA2}
        test/crypto/test_prng.cpp
        ${SOURCES_PRNG}
)
set_target_properties(test_prng PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})

####################################
############ Doxygen Documentation
####################################

# Check if Doxygen is available
find_package(Doxygen REQUIRED)

# Define the Doxygen configuration file path
set(DOXYGEN_CONFIG_FILE ${CMAKE_SOURCE_DIR}/Doxyfile)

# Modify the Doxygen output directory to be the 'doc' folder
set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc)

# Add target for generating the documentation
add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
)

# Ensure the output directory exists
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

