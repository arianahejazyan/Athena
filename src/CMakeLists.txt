# -------------------------
# Chess Library
# -------------------------
file(GLOB_RECURSE CHESS_SRC CONFIGURE_DEPENDS chess/*.cpp)
add_library(chess_lib STATIC ${CHESS_SRC})
target_include_directories(chess_lib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(chess_lib      PRIVATE athena_options)

# # -------------------------
# # HCE Library
# # -------------------------
# file(GLOB_RECURSE HCE_SRC CONFIGURE_DEPENDS core/hce/*.cpp)
# add_library(hce_lib STATIC ${HCE_SRC})
# target_include_directories(hce_lib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(hce_lib      PUBLIC  chess_lib
#                                    PRIVATE athena_options)

# -------------------------
# NNUE Library
# -------------------------
file(GLOB_RECURSE NNUE_SRC CONFIGURE_DEPENDS core/nnue/*.cpp)
add_library(nnue_lib STATIC ${NNUE_SRC})
target_include_directories(nnue_lib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(nnue_lib      PUBLIC  chess_lib
                                    PRIVATE athena_options)

# -------------------------
# Search Library
# -------------------------
file(GLOB_RECURSE SEARCH_SRC CONFIGURE_DEPENDS core/search/*.cpp)
add_library(search_lib STATIC ${SEARCH_SRC})
target_include_directories(search_lib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(search_lib      PUBLIC  nnue_lib
                                      PRIVATE athena_options)

# -------------------------
# Engine Library
# -------------------------
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS core/engine/*.cpp)
add_library(engine_lib STATIC ${ENGINE_SRC})
target_include_directories(engine_lib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(engine_lib      PUBLIC  search_lib
                                      PRIVATE athena_options)

# -------------------------
# CLI Library
# -------------------------
file(GLOB_RECURSE CLI_SRC CONFIGURE_DEPENDS cli/*.cpp)
add_library(cli_lib STATIC ${CLI_SRC})
target_include_directories(cli_lib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR} )
target_link_libraries(cli_lib      PUBLIC  engine_lib
                                   PRIVATE athena_options)

# -------------------------
# Main Executable
# -------------------------
add_executable(athena athena.cc)
target_link_libraries(athena PRIVATE cli_lib athena_options)
target_include_directories(athena PRIVATE ${CMAKE_BINARY_DIR})