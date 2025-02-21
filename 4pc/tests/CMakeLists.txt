find_package(GTest REQUIRED)

add_executable(tests
    test_parser.cc
)
#test_utility.cc
target_include_directories(tests PUBLIC ../utility ../chess)

target_link_libraries(tests PRIVATE parser GTest::GTest GTest::Main) #  utility

include(GoogleTest)
gtest_discover_tests(tests)