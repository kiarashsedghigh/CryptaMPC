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
)

set(SOURCES_PRNG
        src/lib/crypto/prng_aes.cpp
)

####################################
############ Test Targets
####################################

add_executable(test_netio
        ${HEADERS_NETIO}
        test/io/test_netio.cpp
        ${SOURCES_NETIO}
        ${SOURCES_FILEIO}
)

add_executable(test_npot
        ${HEADERS_OT}
        test/mpc/ot/test_npot.cpp
        ${HEADERS_SHA2}
        ${SOURCES_SHA2}
        ${SOURCES_FILEIO}
        ${SOURCES_NETIO}
)

add_executable(test_coot
        ${HEADERS_OT}
        test/mpc/ot/test_coot.cpp
        ${HEADERS_SHA2}
        ${SOURCES_SHA2}
        ${SOURCES_FILEIO}
        ${SOURCES_NETIO}
)

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

add_executable(test_gc
        ${HEADERS_TEST}
        test/mpc/gc/test_gc.cpp
        ${SOURCES_TEST}
)

add_executable(test_2pc
        ${HEADERS_TEST}
        test/mpc/gc/test_2pc.cpp
        ${SOURCES_TEST}
)

add_executable(test_abit
        ${HEADERS_TEST}
        test/mpc/gc/test_abit.cpp
)

add_executable(test_prng
        ${HEADERS_SHA2}
        test/crypto/test_prng.cpp
        ${SOURCES_PRNG}
)
