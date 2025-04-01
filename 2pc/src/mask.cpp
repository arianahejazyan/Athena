#include "mask.h"

namespace Talia
{

[[nodiscard]] consteval static BitBoard generateMask(const Square sq, const Piece piece, const Color color = NO_COLOR)
{
    BitBoard mask = 0ULL;

    const int r = rankSQ(sq);
    const int f = fileSQ(sq);
    
    const auto applyDelta = [&](const int dr, const int df, const bool slide)
    {
        int tr = r + dr;
        int tf = f + df;

        while (isValidSquare(tr, tf))
        {
            setbb(mask, makeSquare(tr, tf));

            if (!slide) break;

            tr += dr;
            tf += df;
        }
    };

    switch (piece)
    {
        case KNIGHT:
            applyDelta(-2, -1, false); applyDelta(-1, -2, false);
            applyDelta(+1, -2, false); applyDelta(+2, -1, false);
            applyDelta(+2, +1, false); applyDelta(+1, +2, false);
            applyDelta(-1, +2, false); applyDelta(-2, +1, false);
            break;
        
        case KING:
            applyDelta(-1, -1, false); applyDelta(-1, 0, false); applyDelta(-1, +1, false);
            applyDelta( 0, -1, false);                           applyDelta( 0, +1, false);
            applyDelta(+1, -1, false); applyDelta(+1, 0, false); applyDelta(+1, +1, false);
            break;
        
        case BISHOP:
            applyDelta(-1, -1, true); applyDelta(-1, +1, true);
            applyDelta(+1, -1, true); applyDelta(+1, +1, true);
            break;
        
        case ROOK:
            applyDelta(-1, 0, true); applyDelta(+1, 0, true);
            applyDelta(0, -1, true); applyDelta(0, +1, true);
            break;
        
        case QUEEN:
            applyDelta(-1, -1, true); applyDelta(-1, +1, true);
            applyDelta(+1, -1, true); applyDelta(+1, +1, true);
            applyDelta(-1,  0, true); applyDelta(+1, 0,  true);
            applyDelta(0,  -1, true); applyDelta(0, +1,  true);
            break;
        
        case PAWN:
            if (color == WHITE) 
                 applyDelta(+1, +1, false), applyDelta(+1, -1, false);
            else applyDelta(-1, +1, false), applyDelta(-1, -1, false);
            break;
    }
    
    return mask;
}

constexpr StaticArray<BitBoard, SQUARE_NB, PIECE_NB> PIECE_MASK = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB, PIECE_NB> arr{};
    
    for (Square sq = A1; sq <= H8; ++sq)
    {
        arr[sq][KNIGHT] = generateMask(sq, KNIGHT);
        arr[sq][BISHOP] = generateMask(sq, BISHOP);
        arr[sq][ROOK]   = generateMask(sq, ROOK);
        arr[sq][QUEEN]  = generateMask(sq, QUEEN);
        arr[sq][KING]   = generateMask(sq, KING);
    }
    
    return arr;
}();

constexpr StaticArray<BitBoard, SQUARE_NB, COLOR_NB> COLOR_MASK = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB, COLOR_NB> arr{};
    
    for (Square sq = A1; sq <= H8; ++sq)
    {
        arr[sq][WHITE] = generateMask(sq, PAWN, WHITE);
        arr[sq][BLACK] = generateMask(sq, PAWN, BLACK);
    }
    
    return arr;
}();

constexpr StaticArray<BitBoard, SQUARE_NB, SQUARE_NB> LINE = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB, SQUARE_NB> arr{};

    for (Square s1 = A1; s1 <= H8; ++s1)
    for (Square s2 = A1; s2 <= H8; ++s2)
    {
        bool cond1 = PIECE_MASK[s1][BISHOP] & BB(s2);
        bool cond2 = PIECE_MASK[s1][ROOK]   & BB(s2);

        if (!cond1 && !cond2) arr[s1][s2] = EMPTY_BOARD;

        else if (cond1) arr[s1][s2] = (PIECE_MASK[s1][BISHOP] & PIECE_MASK[s2][BISHOP]) | BB(s1) | BB(s2);
        else if (cond2) arr[s1][s2] = (PIECE_MASK[s1][ROOK]   & PIECE_MASK[s2][ROOK])   | BB(s1) | BB(s2);
    }

    return arr;
}();

constexpr StaticArray<BitBoard, SQUARE_NB, SQUARE_NB> BETWEEN = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB, SQUARE_NB> arr{};
    
    for (Square s1 = A1; s1 <= H8; ++s1)
    for (Square s2 = A1; s2 <= H8; ++s2)
    {
        bool cond1 = PIECE_MASK[s1][BISHOP] & BB(s2);
        bool cond2 = PIECE_MASK[s1][ROOK]   & BB(s2);

        if (!cond1 && !cond2) arr[s1][s2] = EMPTY_BOARD;

        else arr[s1][s2] = LINE[s1][s2] & (s1 < s2 ? BB(s2) - BB(s1) + BB(s2) : BB(s1) - BB(s2) + BB(s1)) & ~BB(s1);
    }

    return arr;
}();

} // namespace