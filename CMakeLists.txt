cmake_minimum_required(VERSION 3.25)
project(Athena LANGUAGES CXX)

# ====================== VERSION MANAGEMENT ======================
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(GIT_VERSION "unknown")
endif()

set(ATHENA_VERSION "${GIT_VERSION}")
set(ATHENA_AUTHOR "Ariana Hejazyan")

message(STATUS "Athena version: ${ATHENA_VERSION}")

configure_file(
    ${CMAKE_SOURCE_DIR}/src/misc.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/misc.h
    @ONLY
)
# ================================================================

# ====================== Compiler Settings ======================
add_library(athena_options INTERFACE)

target_compile_features(athena_options INTERFACE cxx_std_23)

target_compile_options(athena_options INTERFACE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion -Wsign-conversion
)

# Compiler specific flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(athena_options INTERFACE
        -fconstexpr-ops-limit=1000000000
        -fconstexpr-depth=10000
        $<$<CONFIG:Release>:-O3 -march=native -mtune=native>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(athena_options INTERFACE
        -fconstexpr-steps=100000000
        -fconstexpr-depth=10000
        $<$<CONFIG:Release>:-O3 -march=native -mtune=native>
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(athena_options INTERFACE
        /constexpr:steps100000000
        /constexpr:depth10000
        /W4
        $<$<CONFIG:Release>:/O2 /arch:AVX2>
    )
endif()

# IPO / LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT _ipo_supported)
if(_ipo_supported)
    set_property(TARGET athena_options PROPERTY INTERFACE_INTERPROCEDURAL_OPTIMIZATION $<$<CONFIG:Release>:TRUE>)
endif()
# ================================================================

# Global settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Subdirectories
enable_testing()
add_subdirectory(src)
add_subdirectory(tests)