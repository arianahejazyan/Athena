cmake_minimum_required(VERSION 3.25)
project(Athena LANGUAGES CXX)

# ---------------- Version ----------------
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE ATHENA_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_RESULT
        ERROR_QUIET
    )
    if(NOT GIT_RESULT EQUAL 0 OR ATHENA_VERSION STREQUAL "")
        set(ATHENA_VERSION "unknown")
    endif()
else()
    set(ATHENA_VERSION "unknown")
endif()

set(ATHENA_AUTHOR "Ariana Hejazyan")
message(STATUS "Athena version: ${ATHENA_VERSION}")

configure_file(
    ${CMAKE_SOURCE_DIR}/src/misc.h.in
    ${CMAKE_BINARY_DIR}/misc.h
    @ONLY
)

# ---------------- Compiler options ----------------
add_library(athena_options INTERFACE)
target_compile_features(athena_options INTERFACE cxx_std_23)
target_compile_options(athena_options INTERFACE -Wall -Wextra -Wpedantic -Wshadow)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(athena_options INTERFACE
        $<$<CONFIG:Release>:-O3 -march=native>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(athena_options INTERFACE /W4
        $<$<CONFIG:Release>:/O2 /arch:AVX2>)
endif()

# ---------------- IPO / LTO ----------------
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(NOT ipo_supported)
    message(STATUS "IPO/LTO not supported: ${ipo_error}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

# ---------------- Tests ----------------
enable_testing()
add_subdirectory(tests)