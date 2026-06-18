# -------------------------
# Chess Library
# -------------------------
file(GLOB_RECURSE CHESS_SRC CONFIGURE_DEPENDS chess/*.cpp)
add_library(chess_lib STATIC ${CHESS_SRC})
target_include_directories(chess_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------
# HCE Library
# -------------------------
file(GLOB_RECURSE HCE_SRC CONFIGURE_DEPENDS core/hce/*.cpp)
add_library(hce_lib STATIC ${HCE_SRC})
target_link_libraries(hce_lib PUBLIC chess_lib)
target_include_directories(hce_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------
# NNUE Library
# -------------------------
file(GLOB_RECURSE NNUE_SRC CONFIGURE_DEPENDS core/nnue/*.cpp)
add_library(nnue_lib STATIC ${NNUE_SRC})
target_link_libraries(nnue_lib PUBLIC chess_lib)
target_include_directories(nnue_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------
# Search Library
# -------------------------
file(GLOB_RECURSE SEARCH_SRC CONFIGURE_DEPENDS core/search/*.cpp)
add_library(search_lib STATIC ${SEARCH_SRC})
target_link_libraries(search_lib PUBLIC nnue_lib hce_lib)
target_include_directories(search_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------
# Engine Library
# -------------------------
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS core/engine/*.cpp)
add_library(engine_lib STATIC ${ENGINE_SRC})
target_link_libraries(engine_lib PUBLIC search_lib)
target_include_directories(engine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------
# CLI Library
# -------------------------
file(GLOB_RECURSE CLI_SRC CONFIGURE_DEPENDS cli/*.cpp)
add_library(cli_lib STATIC ${CLI_SRC})
target_link_libraries(cli_lib PUBLIC engine_lib)
target_include_directories(cli_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------
# Main executable
# -------------------------
add_executable(athena athena.cc)
target_link_libraries(athena PRIVATE cli_lib)
target_include_directories(athena PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(athena PRIVATE
    -O3 -march=native -mtune=native -flto -DNDEBUG
)
target_link_options(athena PRIVATE
    -flto
)