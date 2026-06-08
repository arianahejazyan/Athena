cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -flto")

file(GLOB_RECURSE ALL_SRC_FILES src/*.cpp)

add_library(athena_lib STATIC 
    ${ALL_SRC_FILES}
)

target_compile_options(athena_lib PRIVATE -fconstexpr-ops-limit=1000000000
    -O3
    -march=native
    -mavx2
    -mbmi2)

target_include_directories(athena_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(athena 
    src/athena.cc
)

target_link_libraries(athena PRIVATE athena_lib)

# Print compiler information
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler path: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

find_package(GTest REQUIRED)

enable_testing()

add_executable(perft_tests tests/perft_test.cc)

target_compile_options(perft_tests PRIVATE
    -fconstexpr-ops-limit=1000000000
    -O3
    -march=native
    -mavx2
    -mbmi2
)

target_link_libraries(perft_tests PRIVATE athena_lib GTest::GTest GTest::Main)

include(GoogleTest)
gtest_discover_tests(perft_tests)