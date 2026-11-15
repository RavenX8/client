# Client tests. Only what links without the game: the packets library.
include(FetchContent)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# The packets the Rust server pins (osIROSE-new, rust/crates/rose-proto).
set(ROSE_SEMANTIC_VECTORS
    "${CMAKE_SOURCE_DIR}/../osIROSE-new/rust/crates/rose-proto/tests/vectors/semantic.txt"
    CACHE FILEPATH "rose-proto's tests/vectors/semantic.txt")
if(NOT EXISTS "${ROSE_SEMANTIC_VECTORS}")
  message(WARNING "ClientPacketTests: no vectors at ${ROSE_SEMANTIC_VECTORS}; set ROSE_SEMANTIC_VECTORS")
endif()

add_executable(ClientPacketTests packets/semantic_vectors_test.cpp)
target_link_libraries(ClientPacketTests PRIVATE osirose_common::osirose_common GTest::gtest_main)
target_compile_definitions(ClientPacketTests PRIVATE ROSE_SEMANTIC_VECTORS="${ROSE_SEMANTIC_VECTORS}")
set_property(TARGET ClientPacketTests PROPERTY FOLDER "Tests")
if(TARGET openssl_runtime)
  add_dependencies(ClientPacketTests openssl_runtime)
endif()

# Listed when the tests run rather than at build time, so building does not
# need the OpenSSL DLLs in place yet.
gtest_discover_tests(ClientPacketTests DISCOVERY_MODE PRE_TEST)
