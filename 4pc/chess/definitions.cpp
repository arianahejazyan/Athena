#include "definitions.h"

namespace definitions
{

// used to convert 196 locations to 160 squares
constexpr int bricks[14] = {3,9,15,18,18,18,18,18,18,18,18,21,27,33};

// used to extract rank and file of squares
constexpr int stones[20] = {3,9,15,18,18,18,18,18,18,18,18,18,18,18,18,18,18,21,27,33};

bool valid_square(int row, int col)
{
    if (row < 0)  return false;
    if (col < 0)  return false;
    if (row > 13) return false;
    if (col > 13) return false;
    if (row < 3  && col < 3)  return false;
    if (row < 3  && col > 10) return false;
    if (row > 10 && col < 3)  return false;
    if (row > 10 && col > 10) return false;
    
    return true;
}

bool promotion_square(const Square sq, const Player player)
{
    switch (player)
    {
        case (Red):    return rank(sq) == promotionrank;
        case (Blue):   return file(sq) == promotionrank;
        case (Yellow): return rank(sq) == maxrank - promotionrank;
        case (Green):  return file(sq) == maxrank - promotionrank;

        default: return false;
    }
}

bool homerank_square(const Square sq, const Player player)
{
    switch (player)
    {
        case (Red):    return rank(sq) == homerank || rank(sq) == (homerank - 1); 
        case (Blue):   return file(sq) == homerank || file(sq) == (homerank - 1);
        case (Yellow): return rank(sq) == maxrank - homerank || rank(sq) == (maxrank - homerank + 1);
        case (Green):  return file(sq) == maxrank - homerank || file(sq) == (maxrank - homerank + 1);

        default: return false;
    }
}

Player& operator++(Player& player)
{
    player = static_cast<Player>((player + 1) % 5);

    return player;
}

Player enemy[2][2] = 
{
    {Blue, Green},
    {Red, Yellow},
};

const vector<Offset> patterns[6] =
{
    // King 
    {
        Offset(U, R, Crawl),
        Offset(U, S, Crawl),
        Offset(U, L, Crawl),
        Offset(S, R, Crawl),
        Offset(S, L, Crawl),
        Offset(L, R, Crawl),
        Offset(L, S, Crawl),
        Offset(L, L, Crawl),
    },

    // Queen
    {
        Offset(U, R, Slide),
        Offset(U, S, Slide),
        Offset(U, L, Slide),
        Offset(S, R, Slide),
        Offset(S, L, Slide),
        Offset(D, R, Slide),
        Offset(D, S, Slide),
        Offset(D, L, Slide),
    },

    // Rook
    {
        Offset(U, S, Slide),
        Offset(D, S, Slide),
        Offset(S, R, Slide),
        Offset(S, L, Slide),
    },

    // Bishop
    {
        Offset(U, R, Slide),
        Offset(U, L, Slide),
        Offset(D, R, Slide),
        Offset(D, L, Slide),
    },

    // Knight 
    {
        Offset(U + U, R, Crawl),
        Offset(U + U, L, Crawl),
        Offset(U, R + R, Crawl),
        Offset(U, L + L, Crawl),
        Offset(D, R + R, Crawl),
        Offset(D, L + L, Crawl),
        Offset(D + D, R, Crawl),
        Offset(D + D, L, Crawl),
    },

    // Pawn
    {
        Offset(U, S, Red, Push),
        Offset(U, R, Red, Advance),
        Offset(U, L, Red, Advance),

        Offset(S, R, Blue, Push),
        Offset(U, R, Blue, Advance),
        Offset(D, R, Blue, Advance),

        Offset(D, S, Yellow, Push),
        Offset(D, R, Yellow, Advance),
        Offset(D, L, Yellow, Advance),

        Offset(S, L, Green, Push),
        Offset(U, L, Green, Advance),
        Offset(D, L, Green, Advance),
    }
};

}; // namespace