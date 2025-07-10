# cmake_minimum_required(VERSION 3.14)
# project(Athena LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # CLI11 (version 2.5.0)
# include(FetchContent)
# FetchContent_Declare(
#     CLI11
#     GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
#     GIT_TAG        v2.5.0
# )
# FetchContent_MakeAvailable(CLI11)

# # Enable LTO (Link Time Optimization) in Release builds
# include(CheckIPOSupported)
# check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
# if(ipo_supported)
#     set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
# else()
#     message(WARNING "IPO/LTO is not supported: ${ipo_output}")
# endif()

# # Source files
# file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# # Main executable
# add_executable(athena athena.cxx ${SRC_FILES})
# target_include_directories(athena PRIVATE ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(athena PRIVATE CLI11::CLI11)
# target_compile_options(athena PRIVATE
#     $<$<CONFIG:Release>:-O3 -march=native>
#     $<$<CONFIG:Debug>:-O0 -g>
# )

# # Enable testing
# enable_testing()

# include(FetchContent)

# # Try to find system-installed GTest first
# find_package(GTest QUIET)

# if (NOT GTest_FOUND)
#     message(STATUS "GTest not found, fetching with FetchContent...")

#     FetchContent_Declare(
#         googletest
#         GIT_REPOSITORY https://github.com/google/googletest.git
#         GIT_TAG        v1.14.0
#     )
#     FetchContent_MakeAvailable(googletest)

# else()
#     message(STATUS "Found system-installed GTest")
# endif()

# # Test executable
# add_executable(test_movegen tests/test_movegen.cc ${SRC_FILES})
# target_include_directories(test_movegen PRIVATE ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(test_movegen PRIVATE GTest::GTest GTest::Main)
# target_compile_options(test_movegen PRIVATE
#     $<$<CONFIG:Release>:-O3 -march=native>
#     $<$<CONFIG:Debug>:-O0 -g>
# )

# add_test(NAME test_movegen COMMAND test_movegen)
# target_link_libraries(test_movegen PRIVATE GTest::GTest GTest::Main CLI11::CLI11)

# cmake_minimum_required(VERSION 3.14)
# project(Athena LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # ========= AVX2 detection and toggle =========
# option(ENABLE_AVX2 "Enable AVX2 instructions if supported" ON)
# include(CheckCXXCompilerFlag)

# if (ENABLE_AVX2)
#     check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
#     if (COMPILER_SUPPORTS_AVX2)
#         message(STATUS "AVX2 is supported by compiler. Enabling it.")
#         set(AVX2_FLAGS "-mavx2")
#         add_definitions(-DUSE_AVX2)
#     else()
#         message(WARNING "Compiler does not support -mavx2")
#     endif()
# endif()
# # =============================================

# # CLI11 (version 2.5.0)
# include(FetchContent)
# FetchContent_Declare(
#     CLI11
#     GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
#     GIT_TAG        v2.5.0
# )
# FetchContent_MakeAvailable(CLI11)

# # Enable LTO (Link Time Optimization) in Release builds
# include(CheckIPOSupported)
# check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
# if(ipo_supported)
#     set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
# else()
#     message(WARNING "IPO/LTO is not supported: ${ipo_output}")
# endif()

# # Source files
# file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# # Main executable
# add_executable(athena athena.cxx ${SRC_FILES})
# target_include_directories(athena PRIVATE ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(athena PRIVATE CLI11::CLI11)
# target_compile_options(athena PRIVATE
#     $<$<CONFIG:Release>:-O3 -march=native ${AVX2_FLAGS}>
#     $<$<CONFIG:Debug>:-O0 -g>
# )

# # Enable testing
# enable_testing()

# # Try to find system-installed GTest first
# find_package(GTest QUIET)

# if (NOT GTest_FOUND)
#     message(STATUS "GTest not found, fetching with FetchContent...")

#     FetchContent_Declare(
#         googletest
#         GIT_REPOSITORY https://github.com/google/googletest.git
#         GIT_TAG        v1.14.0
#     )
#     FetchContent_MakeAvailable(googletest)

# else()
#     message(STATUS "Found system-installed GTest")
# endif()

# # Test executable
# add_executable(test_movegen tests/test_movegen.cc ${SRC_FILES})
# target_include_directories(test_movegen PRIVATE ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(test_movegen PRIVATE GTest::GTest GTest::Main CLI11::CLI11)
# target_compile_options(test_movegen PRIVATE
#     $<$<CONFIG:Release>:-O3 -march=native ${AVX2_FLAGS}>
#     $<$<CONFIG:Debug>:-O0 -g>
# )

# add_test(NAME test_movegen COMMAND test_movegen)

# # NNUE test executable
# add_executable(test_nnue tests/test_nnue.cc ${SRC_FILES})
# target_include_directories(test_nnue PRIVATE ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(test_nnue PRIVATE GTest::GTest GTest::Main CLI11::CLI11)
# target_compile_options(test_nnue PRIVATE
#     $<$<CONFIG:Release>:-O3 -march=native ${AVX2_FLAGS}>
#     $<$<CONFIG:Debug>:-O0 -g>
# )

# add_test(NAME test_nnue COMMAND test_nnue)

cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(CLI11 REQUIRED)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)