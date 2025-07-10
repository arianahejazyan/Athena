file(GLOB_RECURSE TEST_SOURCES "*.cc")

foreach(test_src IN LISTS TEST_SOURCES)
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE athena_lib GTest::GTest GTest::Main)
    target_compile_options(${test_name} PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native>
        $<$<CONFIG:Debug>:-O0 -g>
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
