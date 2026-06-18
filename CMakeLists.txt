cmake_minimum_required(VERSION 3.16)
project(Athena LANGUAGES CXX)

# -------------------------
# Includes
# -------------------------
include(CheckIPOSupported)
include(GoogleTest)

# -------------------------
# IPO/LTO Optimization
# -------------------------
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

if(CMAKE_BUILD_TYPE STREQUAL "Release" AND ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# -------------------------
# C++ Standard
# -------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------
# Add Subdirectories
# -------------------------
add_subdirectory(src)
add_subdirectory(tests)