file(GLOB_RECURSE ALL_SRC_FILES *.cpp)

# Exclude athena.cxx (main executable)
list(FILTER ALL_SRC_FILES EXCLUDE REGEX "athena\\.cxx$")

add_library(athena_lib STATIC ${ALL_SRC_FILES})

target_include_directories(athena_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_compile_options(athena_lib PRIVATE
    $<$<CONFIG:Release>:-O3 -march=native>
    $<$<CONFIG:Debug>:-O0 -g>
)

target_link_libraries(athena_lib PRIVATE CLI11::CLI11)

add_executable(athena athena.cxx)
target_link_libraries(athena PRIVATE athena_lib)

target_compile_options(athena PRIVATE
    $<$<CONFIG:Release>:-O3 -march=native>
    $<$<CONFIG:Debug>:-O0 -g>
)
