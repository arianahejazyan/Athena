cmake_minimum_required(VERSION 3.14)

project(Talia-2PC LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure Release flags are set
# set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/attacks)

# Collect source files
file(GLOB_RECURSE SRC_FILES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/attacks/*.cpp
)

# Main executable
add_executable(main main.cc ${SRC_FILES})

enable_testing()
find_package(GTest REQUIRED)

# Create test executable
add_executable(test_movegen tests/test_movegen.cc ${SRC_FILES})
target_link_libraries(test_movegen GTest::GTest GTest::Main)

add_executable(test_perft tests/test_perft.cc ${SRC_FILES})
target_link_libraries(test_perft GTest::GTest GTest::Main)

# Register test
add_test(NAME test_movegen COMMAND test_movegen)
add_test(NAME test_perft COMMAND test_perft)