# -------------------------
# Perft executable
# -------------------------
add_executable(perft 
    ${CMAKE_CURRENT_SOURCE_DIR}/perft.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/perft.h)
target_link_libraries(perft PRIVATE chess_lib)
target_include_directories(perft PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_options(perft PRIVATE
    -O3 -march=native -mtune=native -flto -DNDEBUG
)
target_link_options(perft PRIVATE
    -flto
)

# -------------------------
# Tests (GoogleTest)
# -------------------------
enable_testing()
find_package(GTest REQUIRED)

add_executable(perft_tests perft_test.cc)
target_link_libraries(perft_tests PRIVATE
    chess_lib
    GTest::gtest_main
)
target_include_directories(perft_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

gtest_discover_tests(perft_tests)