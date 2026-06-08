cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES CXX)

include(CheckIPOSupported)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ALL_SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_library(athena_lib STATIC ${ALL_SRC_FILES})

target_include_directories(athena_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(athena_lib PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Release>:-march=native>
    -fconstexpr-ops-limit=1000000000
)

set_target_properties(athena_lib PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)

add_executable(athena src/athena.cc)
target_link_libraries(athena PRIVATE athena_lib)

check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    set_target_properties(athena_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_target_properties(athena PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

enable_testing()

find_package(GTest REQUIRED)

add_executable(perft_tests tests/perft_test.cc)
target_link_libraries(perft_tests PRIVATE athena_lib GTest::gtest_main)

gtest_discover_tests(perft_tests)