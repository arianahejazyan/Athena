#pragma once

#include <cstdint>

namespace athena::chess {

enum BoardRepresentation {
    Athena,
    ChessCom,
    NoStones,
    Compressed,
    BORAD_REPRESENTATION
};

constexpr std::size_t BOARD_SIZE[BORAD_REPRESENTATION] = {
    256, // 16x16 board
    196, // 14x14 board: first/last row and column removed
    160, // stone squares removed
    216, // first/last 20 squares removed; index = sq - 20
};

constexpr int
MOVETYPE_NB = 5, 
MOVEFLAG_NB = 4, 
MOVE_NB     = 256,
PLAY_NB     = 64,
HOMERANK    = 2, 
PROMOTES    = 11,
CRAWL_NB    = 8,
SLIDE_NB    = 4,
PUSH_NB     = 2, 
TAKE_NB     = 2,
RANK_NB     = 16, 
FILE_NB     = 16, 
STONE_NB    = 40,
VALID_NB    = 216, 
SQUARE_NB   = 256,
CHUNK_NB    = 4,
CHUNK_SIZE  = 64,
PIECE_NB    = 6,
COLOR_NB    = 4,
PIECECOLOR_NB = 64,
SETUP_NB    = 2,
SIDE_NB     = 2,
CASTLE_NB   = 256;

} // namespace athena
