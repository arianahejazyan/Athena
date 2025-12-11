# cmake_minimum_required(VERSION 3.14)
# project(Athena LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable_testing()

# find_package(GTest REQUIRED)
# find_package(benchmark REQUIRED)

# # Enable LTO (Link Time Optimization) in Release builds
# include(CheckIPOSupported)
# check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
# if(ipo_supported)
#     set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
# else()
#     message(WARNING "IPO/LTO is not supported: ${ipo_output}")
# endif()

# file(GLOB_RECURSE ALL_SRC_FILES src/*.cpp)

# add_library(athena_lib STATIC ${ALL_SRC_FILES})
# target_include_directories(athena_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_compile_options(athena_lib PRIVATE -mavx2 -mbmi2)
# target_compile_options(athena_lib PRIVATE
#     $<$<CONFIG:Release>:-O3>
#     $<$<CONFIG:Debug>:-O0 -g>
# )

# add_executable(athena athena.cxx)
# target_link_libraries(athena PRIVATE athena_lib)

# add_subdirectory(tests)
# # add_subdirectory(benchmarks)

# if(ipo_supported)
#     set_target_properties(athena_lib PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
#     set_target_properties(athena PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
# endif()

# set(HOME_RANK 2)
# set(PROMOTION_RANK 11)

# configure_file(
#     ${CMAKE_SOURCE_DIR}/include/athena/cmake/config.hpp.in
#     ${CMAKE_BINARY_DIR}/include/athena/chess/config.hpp
# )

cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -flto")

file(GLOB_RECURSE ALL_SRC_FILES src/*.cpp)

add_library(athena_lib STATIC 
    ${ALL_SRC_FILES}
)

target_include_directories(athena_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(athena 
    athena.cpp
)

target_link_libraries(athena PRIVATE athena_lib)

# add_library(athena-utility STATIC 
#     src/utility/utility.cpp
# )

# target_include_directories(athena-utility PUBLIC 
#     ${CMAKE_SOURCE_DIR}/include/athena/utility
#     ${CMAKE_BINARY_DIR}/include/athena/utility
# )

# add_library(athena-chess STATIC 
#     src/chess/square.cpp
#     src/chess/piece.cpp
#     src/chess/color.cpp
#     src/chess/piececlass.cpp
# )

# target_include_directories(athena-chess PUBLIC 
#     ${CMAKE_SOURCE_DIR}/include/athena/chess
#     ${CMAKE_BINARY_DIR}/include/athena/chess
# )

# add_library(athena-cli STATIC 
#     src/cli/cli.cpp
# )

# target_include_directories(athena-cli PUBLIC 
#     ${CMAKE_SOURCE_DIR}/include/athena/cli
#     ${CMAKE_SOURCE_DIR}/include/athena/utility
# )

# target_link_libraries(athena-cli PRIVATE athena-chess athena-utility)

# add_executable(athena src/athena.cpp)

# target_link_libraries(athena PRIVATE
#     athena-utility
#     athena-chess
#     athena-cli
# )

# set(SETUP "Modern" CACHE STRING "Setup: Modern or Classic")

# if(NOT SETUP STREQUAL "Modern" AND NOT SETUP STREQUAL "Classic")
#   message(FATAL_ERROR "Invalid SETUP: ${SETUP}")
# endif()


# enable_testing()

# find_package(GTest REQUIRED)
# find_package(benchmark REQUIRED)

# file(GLOB_RECURSE ALL_SRC_FILES src/*.cpp)

# add_library(athena_lib STATIC ${ALL_SRC_FILES})
# target_include_directories(athena_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# if(SETUP STREQUAL "Modern")
#   add_compile_definitions(SETUP_MODERN)
# else()
#   add_compile_definitions(SETUP_CLASSIC)
# endif()

# ✅ CPU + Full LTO compile options for Release
# target_compile_options(athena_lib PRIVATE
#     $<$<CONFIG:Release>:-O3 -flto -ffat-lto-objects -mavx2 -mbmi2 -march=native>
#     $<$<CONFIG:Debug>:-O0 -g>
# )
# target_link_libraries(athena PRIVATE athena_lib)



# # ✅ Full LTO at link time for Release
# target_link_options(athena PRIVATE
#     $<$<CONFIG:Release>:-flto>
# )

# # ✅ Full LTO at link time for static lib (just in case it's used directly)
# target_link_options(athena_lib PRIVATE
#     $<$<CONFIG:Release>:-flto>
# )

# add_subdirectory(tests)
# add_subdirectory(benchmarks)
