cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES CXX)

include(CheckIPOSupported)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_SRC_FILES CONFIGURE_DEPENDS
    src/chess/*.cpp
    src/cli/*.cpp
    src/core/*.cpp
    src/utility/*.cpp
)

add_library(athena_lib STATIC ${LIB_SRC_FILES})

target_include_directories(athena_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chess
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cli
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility
)

target_compile_options(athena_lib PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Release>:-march=native>
    -fconstexpr-ops-limit=1000000000
)

check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    set_target_properties(athena_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

add_executable(athena src/athena.cc)
target_link_libraries(athena PRIVATE athena_lib)
if(ipo_supported)
    set_target_properties(athena PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(perft_tests tests/perft_test.cc)
target_link_libraries(perft_tests PRIVATE athena_lib GTest::gtest_main)
gtest_discover_tests(perft_tests)