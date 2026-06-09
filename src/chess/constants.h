#pragma once

#include <array>
#include <cstdint>

namespace athena
{

constexpr std::size_t 
GAMESETUP_NB=2,
GAMEMODE_NB=2,
SQUARE_DELTA   = 60,
CHUNK_0 = 0,
CHUNK_1 = 1,
CHUNK_2 = 2,
CHUNK_3 = 3,
TEAM_NB        = 2,
ALLIANCE_NB    = 2,
REDUCE_SIZE    = 60,
GUIDES_SIZE    = 2,
GUILD_NB       = 2,
RANK_NB        = 16,
FILE_NB        = 16,
SQUARE_NB      = 256,
PIECE_NB       = 6,
COLOR_NB       = 4,
PIECECLASS_NB  = 64,
SIDE_NB        = 2,
CASTLE_NB      = 8,
MOVETYPE_NB    = 5,
MOVEFLAG_NB    = 2,
MOVE_NB        = 256,
PLAY_NB        = 32,
HOMERANK      = 2,
PROMOTIONRANK = 11,
CRAWL_NB       = 8,
SLIDE_NB       = 4,
PUSH_NB        = 2,
TAKE_NB        = 2,
CHUNK_NB       = 4,
CHUNK_SIZE     = 64,
CACHELINE_SIZE = 64;

enum GameSetup : uint8_t
{
    Modern  = 0,
    Classic = 1,
};

enum GameMode : uint8_t
{
    TEAM = 0,
    FFA  = 1,
};

inline constexpr std::array<GameSetup, SIDE_NB> SETUP_ARRAY =
{
    GameSetup::Modern , 
    GameSetup::Classic, 
};

inline constexpr std::array<GameSetup, SIDE_NB> setup_array() noexcept { 
    return SETUP_ARRAY;
}

inline constexpr std::array<GameMode, SIDE_NB> MODE_ARRAY =
{
    GameMode::TEAM , 
    GameMode::FFA, 
};

inline constexpr std::array<GameMode, SIDE_NB> mode_array() noexcept { 
    return MODE_ARRAY;
}

} // namespace athena