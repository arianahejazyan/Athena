cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CLI11 (version 2.5.0)
include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.5.0
)
FetchContent_MakeAvailable(CLI11)

# Enable LTO (Link Time Optimization) in Release builds
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "IPO/LTO is not supported: ${ipo_output}")
endif()

# Source files
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Main executable
add_executable(athena athena.cxx ${SRC_FILES})
target_include_directories(athena PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(athena PRIVATE CLI11::CLI11)
target_compile_options(athena PRIVATE
    $<$<CONFIG:Release>:-O3 -march=native>
    $<$<CONFIG:Debug>:-O0 -g>
)

# Enable testing
enable_testing()

include(FetchContent)

# Try to find system-installed GTest first
find_package(GTest QUIET)

if (NOT GTest_FOUND)
    message(STATUS "GTest not found, fetching with FetchContent...")

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

else()
    message(STATUS "Found system-installed GTest")
endif()

# Test executable
add_executable(test_movegen tests/test_movegen.cc ${SRC_FILES})
target_include_directories(test_movegen PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_movegen PRIVATE GTest::GTest GTest::Main)
target_compile_options(test_movegen PRIVATE
    $<$<CONFIG:Release>:-O3 -march=native>
    $<$<CONFIG:Debug>:-O0 -g>
)

add_test(NAME test_movegen COMMAND test_movegen)
