file(GLOB_RECURSE ATHENA_SRC CONFIGURE_DEPENDS
    chess/*.cpp
    nnue/*.cpp
    uci/*.cpp
    *.cpp
)
list(REMOVE_ITEM ATHENA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/athena.cc)

add_library(athena_lib STATIC ${ATHENA_SRC})
target_include_directories(athena_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}   # for generated misc.h
)
target_link_libraries(athena_lib PUBLIC athena_options)

if(ipo_supported)
    set_target_properties(athena_lib PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION $<$<CONFIG:Release>:TRUE>)
endif()

add_executable(athena athena.cc)
target_link_libraries(athena PRIVATE athena_lib)

if(ipo_supported)
    set_target_properties(athena PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION $<$<CONFIG:Release>:TRUE>)
endif()