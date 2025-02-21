cmake_minimum_required(VERSION 3.14)

project(ELSA-2PC LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

# include_directories(src)
# include_directories(src/containers)

# add_executable(main main.cc)

# target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)


include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(main main.cc ${SRC_FILES})