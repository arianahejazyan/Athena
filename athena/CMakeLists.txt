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


cmake_minimum_required(VERSION 3.14)
project(Athena LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

file(GLOB_RECURSE ALL_SRC_FILES src/*.cpp)

add_library(athena_lib STATIC ${ALL_SRC_FILES})
target_include_directories(athena_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ✅ CPU + Full LTO compile options for Release
target_compile_options(athena_lib PRIVATE
    $<$<CONFIG:Release>:-O3 -flto -ffat-lto-objects -mavx2 -mbmi2>
    $<$<CONFIG:Debug>:-O0 -g>
)

add_executable(athena athena.cxx)
target_link_libraries(athena PRIVATE athena_lib)

# ✅ Full LTO at link time for Release
target_link_options(athena PRIVATE
    $<$<CONFIG:Release>:-flto>
)

# ✅ Full LTO at link time for static lib (just in case it's used directly)
target_link_options(athena_lib PRIVATE
    $<$<CONFIG:Release>:-flto>
)

add_subdirectory(tests)
# add_subdirectory(benchmarks)
