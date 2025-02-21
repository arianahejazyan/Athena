cmake_minimum_required(VERSION 3.12)

project(Talia C CXX)

enable_testing()

add_subdirectory(engine)
add_subdirectory(chess)
add_subdirectory(tests)
add_subdirectory(utility)