add_library(parser STATIC parser.cpp)
add_library(definitions STATIC definitions.cpp)

target_link_libraries(parser definitions)

#target_include_directories(parser PUBLIC ${CMAKE_SOURCE_DIR}/chess)

#add_library(utility STATIC utility.cpp)