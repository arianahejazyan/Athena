#pragma once
#include "definitions.hpp"

using namespace definitions;

namespace pseudo
{

//
struct Collection
{
    /*
        Shift: 2880
        Crawl: 2116
        Slide: 5960
        March: 1664
        Total: 9740
    */

    //
    array<Square, 9740> moves = {};

    //
    array<int, 2880> heads = {};

    //
    array<int, 2880> tails = {};
};

//
#define CrawlShift(sq, piece) (piece == King ? 0 : 1) * board_size + sq

//
#define SlideShift(sq, ray) (2 + ray) * board_size + sq

//
#define PushShift(sq, player) (10 + player) * board_size + sq

//
#define AdvanceShift(sq, player) (14 + player) * board_size + sq

//
#define AttackShift(sq, player) (14 + opposite(player)) * board_size + sq

//
constexpr array<Offset, 8> crawl_patterns[2] = 
{
    // King
    {
        Offset{U, R},
        Offset{U, S},
        Offset{U, L},
        Offset{S, R},
        Offset{S, L},
        Offset{L, R},
        Offset{L, S},
        Offset{L, L},
    }, 
    // Knight
    {
        Offset{U + U, R},
        Offset{U + U, L},
        Offset{U, R + R},
        Offset{U, L + L},
        Offset{D, R + R},
        Offset{D, L + L},
        Offset{D + D, R},
        Offset{D + D, L},
    } 
};

//
constexpr array<Offset, 8> slide_patterns =
{
    Offset{U, R}, // Bishop & Queen 
    Offset{U, S}, // Rook   & Queen 
    Offset{U, L}, // Bishop & Queen
    Offset{S, R}, // Rook   & Queen 
    Offset{S, L}, // Rook   & Queen 
    Offset{D, R}, // Bishop & Queen
    Offset{D, S}, // Rook   & Queen 
    Offset{D, L}, // Bishop & Queen
};

//
constexpr array<Offset, 1> push_patterns[player_size] =
{
    Offset{U, S}, // Red
    Offset{S, R}, // Blue
    Offset{D, S}, // Yellow
    Offset{S, L}, // Green
};

//
constexpr array<Offset, 2> advance_patterns[player_size] =
{
    { Offset{U, R}, Offset{U, L} }, // Red
    { Offset{U, R}, Offset{D, R} }, // Blue
    { Offset{D, R}, Offset{D, L} }, // Yellow
    { Offset{U, L}, Offset{D, L} }, // Green
};

constexpr void crawl(Collection& arrays, int& idx, Piece piece)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        //
        arrays.heads[CrawlShift(sq, piece)] = idx;

        for (auto offset: crawl_patterns[piece == King ? 0 : 1])
        {
            int r = rank(sq) + offset.first;
            int f = file(sq) + offset.second;

            if (valid_square(r, f))
            {
                arrays.moves[idx++] = spot(r, f);
            }
        }

        //
        arrays.tails[CrawlShift(sq, piece)] = idx;
    }
}

constexpr void slide(Collection& arrays, int& idx, int ray)
{
    Offset offset = slide_patterns[ray];

    for (Square sq = 0; sq < board_size; ++sq)
    {
        //
        arrays.heads[SlideShift(sq, ray)] = idx;

        int r = rank(sq) + offset.first;
        int f = file(sq) + offset.second;

        while (valid_square(r, f))
        {
            arrays.moves[idx++] = spot(r, f);
            
            r += offset.first;
            f += offset.second;
        }

        //
        arrays.tails[SlideShift(sq, ray)] = idx;
    }
}

constexpr void push(Collection& arrays, int& idx, Player player)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        //
        arrays.heads[PushShift(sq, player)] = idx;

        for (Offset offset: push_patterns[player])
        {
            int r = rank(sq) + offset.first;
            int f = file(sq) + offset.second;        

            if (valid_square(r, f))
            {
                arrays.moves[idx++] = spot(r, f);
            } 
        }
        
        //
        arrays.tails[PushShift(sq, player)] = idx;
    }
}

constexpr void advance(Collection& arrays, int& idx, Player player)
{
    for (Square sq = 0; sq < board_size; ++sq)
    {
        //
        arrays.heads[AdvanceShift(sq, player)] = idx;

        for (Offset offset: advance_patterns[player])
        {
            int r = rank(sq) + offset.first;
            int f = file(sq) + offset.second;        

            if (valid_square(r, f))
            {
                arrays.moves[idx++] = spot(r, f);
            } 
        }
        
        //
        arrays.tails[AdvanceShift(sq, player)] = idx;
    }
}

constexpr Collection initialize()
{
    Collection arrays;

    // index that iterate over moves array
    int idx = 0;

    //
    for (Piece piece: {King, Knight})
    {
        crawl(arrays, idx, piece);
    }
    
    //
    int ray = 0;
    for (Offset offset: slide_patterns)
    {
        slide(arrays, idx, ray++);
    }

    // push
    for (Player player: {Red, Blue, Yellow, Green})
    {
        push(arrays, idx, player); 
    }
    
    // advance
    for (Player player: {Red, Blue, Yellow, Green})
    {
        advance(arrays, idx, player);
    }        

    return arrays;
}

//
inline constexpr auto arrays = initialize();

inline constexpr auto& heads = arrays.heads;
inline constexpr auto& tails = arrays.tails;
inline constexpr auto& moves = arrays.moves;

};