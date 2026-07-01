include(FetchContent)

# -------------------------
# GoogleTest
# -------------------------
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    ON      # ← only clones the tagged commit, not full history
    GIT_PROGRESS   ON      # ← prints download progress so you know it's alive
    SYSTEM
    FIND_PACKAGE_ARGS NAMES GTest
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# -------------------------
# Google Benchmark
# -------------------------
FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.1
    GIT_SHALLOW    ON
    GIT_PROGRESS   ON
    SYSTEM
)
set(BENCHMARK_ENABLE_TESTING   OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL   OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

# -------------------------
# Test Data Path
# -------------------------
set(PERFT_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

# -------------------------
# Unit Tests
# -------------------------
add_executable(perft_test perft_test.cc)
target_include_directories(perft_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(perft_test PRIVATE 
    chess_lib
    GTest::gtest_main
    athena_options)
target_compile_definitions(perft_test PRIVATE
    PERFT_DATA_DIR="${PERFT_DATA_DIR}")

include(GoogleTest)
gtest_discover_tests(perft_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES TIMEOUT 120          # per-test timeout in seconds
)

# -------------------------
# Benchmarks
# -------------------------
add_executable(perft_bench perft_bench.cc)
target_include_directories(perft_bench PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(perft_bench PRIVATE 
    chess_lib
    benchmark::benchmark_main
    athena_options)