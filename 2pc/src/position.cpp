#include "attacks.h"
#include "bitboard.h"
#include "position.h"
#include "mask.h"
#include "move.h"

namespace Talia
{

void Position::computePinned()
{
    const Square royal = getlsb(teammate(KING));

    pinned = EMPTY_BOARD;

    u64 candidates = attacks<BISHOP>(royal, opponent()) & (opponent(QUEEN) | opponent(BISHOP))
                   | attacks<ROOK>(royal,   opponent()) & (opponent(QUEEN) | opponent(ROOK)); 

    while (candidates)
    {
        Square sq = poplsb(candidates); 

        u64 between = BETWEEN[royal][sq]; 

        if (countbb(between & teammate()) == 1) setbb(pinned, between);
    }
}

void Position::computeChecks()
{
    const Square royal = getlsb(teammate(KING)); 
    
    auto checkers = EMPTY_BOARD;
                             
    setbb(checkers, attacks<BISHOP>(royal, everyone()) & (opponent(BISHOP) | opponent(QUEEN)));
    setbb(checkers, attacks<ROOK>(royal, everyone())   & (opponent(ROOK)   | opponent(QUEEN)));

    checks = ~EMPTY_BOARD;
    while (checkers) checks &= BETWEEN[royal][poplsb(checkers)];
    
    checks &= ((attacks(royal, turn) & opponent(PAWN)) | (attacks(royal, KNIGHT) & opponent(KNIGHT))) ?: ~EMPTY_BOARD;
}

void Position::computeThreat()
{
    threat = EMPTY_BOARD;

    auto pawns   = opponent(PAWN);
    auto knights = opponent(KNIGHT);
    auto bishops = opponent(BISHOP);
    auto rooks   = opponent(ROOK);
    auto queens  = opponent(QUEEN);
    auto kings   = opponent(KING);

    while (pawns)   setbb(threat, attacks(poplsb(pawns), next(turn)));
    while (knights) setbb(threat, attacks(poplsb(knights), KNIGHT));
    while (bishops) setbb(threat, attacks<BISHOP>(poplsb(bishops), everyone() & ~teammate(KING)));
    while (rooks)   setbb(threat, attacks<ROOK>(poplsb(rooks),     everyone() & ~teammate(KING)));
    while (queens)  setbb(threat, attacks<QUEEN>(poplsb(queens),   everyone() & ~teammate(KING)));
    while (kings)   setbb(threat, attacks(poplsb(kings), KING));
}

constexpr inline PieceType getPromotionPieceType(MoveFlag flag, Color turn)
{
    switch (flag)
    {
        case FLAG_KNIGHT: return (turn == WHITE ? W_KNIGHT : B_KNIGHT);
        case FLAG_BISHOP: return (turn == WHITE ? W_BISHOP : B_BISHOP);
        case FLAG_ROOK:   return (turn == WHITE ? W_ROOK   : B_ROOK);
        case FLAG_QUEEN:  return (turn == WHITE ? W_QUEEN  : B_QUEEN);

        default: return NO_PIECE_TYPE;
    }
}

void Position::makemove(const Move move)
{
    GameState& gs = states[size++];

    auto source = move.source();
    auto target = move.target();
    auto nature = move.nature();

    PieceType type = board.at(source);
    PieceType take = board.at(target);

    if (take != NO_PIECE_TYPE)
    {
        board.popSQ(target);
    }

    board.setSQ(target, type);
    board.popSQ(source);

    state().epsq = OFFBOARD;

    if (nature != SIMPLE)
    {
        
    if (nature == DOUBLE)
        state().epsq = (Square)(turn == WHITE ? target - 8 : target + 8);

    else if (nature == ENPASS)
        board.popSQ((Square)(turn == WHITE ? target - 8 : target + 8));

    else if (nature == CASTLE)
    {
        Square rookSource = (Square)(source < target ? source + 3 : source - 4);
        Square rookTarget = (Square)(source < target ? source + 1 : source - 1);

        board.setSQ(rookTarget, turn == WHITE ? W_ROOK : B_ROOK);
        board.popSQ(rookSource);
    }

    else
    {
        board.popSQ(target);
        board.setSQ(target, getPromotionPieceType(nature, turn));
    }

    }; // end of handle non-simple moves

    // update castling rights
    state().castle = static_cast<Castle>(gs.castle & ~BB(source) & ~BB(target));

    turn = next(turn);
    gs.captured = take;
}

void Position::undomove(const Move move)
{
    const GameState& gs = states[--size];

    auto source = move.source();
    auto target = move.target();
    auto nature = move.nature();
    
    PieceType type = board.at(target);
    PieceType take = gs.captured;

    board.setSQ(source, type);
    board.popSQ(target);

    if (take != NO_PIECE_TYPE) board.setSQ(target, take);


    if (nature != SIMPLE)
    {

    if (nature == ENPASS)
    {
        board.setSQ((Square)(turn == BLACK ? target - 8 : target + 8), (turn == WHITE ? W_PAWN : B_PAWN));
    }

    else if (nature == CASTLE)
    {
        Square rookSource = (Square)(source < target ? source + 3 : source - 4);
        Square rookTarget = (Square)(source < target ? source + 1 : source - 1);

        board.setSQ(rookSource, turn == BLACK ? W_ROOK : B_ROOK);
        board.popSQ(rookTarget);
    }

    else if (nature != DOUBLE)
    {
        board.popSQ(source);
        board.setSQ(source, turn == BLACK ? W_PAWN : B_PAWN);
    }

    }; // end of handle non-simple moves

    turn = next(turn);
}

} // namespace