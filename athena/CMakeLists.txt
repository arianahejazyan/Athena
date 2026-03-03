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

option(ATHENA_ENABLE_NATIVE_ARCH "Enable native CPU tuning in Release builds" ON)
option(ATHENA_ENABLE_IPO "Enable IPO/LTO in Release builds when supported" ON)
option(ATHENA_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ATHENA_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ATHENA_BUILD_SDL3_GUI "Build SDL3 GUI application" ON)

file(GLOB_RECURSE ALL_SRC_FILES src/*.cpp)

add_library(athena_lib STATIC 
    ${ALL_SRC_FILES}
)

if(ATHENA_ENABLE_NATIVE_ARCH)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(athena_lib PRIVATE $<$<CONFIG:Release>:-march=native>)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(athena_lib PRIVATE -fconstexpr-ops-limit=1000000000)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(athena_lib PRIVATE -fconstexpr-steps=1000000000)
endif()

target_include_directories(athena_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(athenacli 
    athena.cpp
)

target_link_libraries(athenacli PRIVATE athena_lib)

if(ATHENA_BUILD_SDL3_GUI)
    find_package(SDL3 QUIET)
    if(SDL3_FOUND)
        add_executable(athena athena_gui.cpp)
        target_link_libraries(athena PRIVATE athena_lib SDL3::SDL3)
        target_compile_features(athena PRIVATE cxx_std_23)
    else()
        message(STATUS "SDL3 not found: athena target (GUI) will not be built")
    endif()
endif()

if(ATHENA_ENABLE_NATIVE_ARCH)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(athenacli PRIVATE $<$<CONFIG:Release>:-march=native>)
        if(TARGET athena)
            target_compile_options(athena PRIVATE $<$<CONFIG:Release>:-march=native>)
        endif()
    endif()
endif()

if(ATHENA_ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ATHENA_IPO_SUPPORTED OUTPUT ATHENA_IPO_ERROR)
    if(ATHENA_IPO_SUPPORTED)
        set_property(TARGET athena_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set_property(TARGET athenacli PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        if(TARGET athena)
            set_property(TARGET athena PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        endif()
    else()
        message(STATUS "IPO/LTO not enabled: ${ATHENA_IPO_ERROR}")
    endif()
endif()

if(ATHENA_ENABLE_ASAN OR ATHENA_ENABLE_UBSAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        set(ATHENA_SAN_FLAGS -fno-omit-frame-pointer)
        if(ATHENA_ENABLE_ASAN)
            list(APPEND ATHENA_SAN_FLAGS -fsanitize=address)
        endif()
        if(ATHENA_ENABLE_UBSAN)
            list(APPEND ATHENA_SAN_FLAGS -fsanitize=undefined)
        endif()
        target_compile_options(athena_lib PRIVATE ${ATHENA_SAN_FLAGS})
        target_compile_options(athenacli PRIVATE ${ATHENA_SAN_FLAGS})
        target_link_options(athenacli PRIVATE ${ATHENA_SAN_FLAGS})
        if(TARGET athena)
            target_compile_options(athena PRIVATE ${ATHENA_SAN_FLAGS})
            target_link_options(athena PRIVATE ${ATHENA_SAN_FLAGS})
        endif()
    else()
        message(WARNING "Sanitizers requested but unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()

# Print compiler information
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler path: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

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
