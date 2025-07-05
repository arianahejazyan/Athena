#include "position.h"

namespace athena
{

void Position::makemove(Move move)
{
    const GameState& gs = states.back();

    auto source = move.source();
    auto target = move.target();
    auto nature = move.nature();

    auto type = board[source];
    auto take = board[target];
    auto hash = gs.hash;

    // Update clock
    int clock = 0;
    if (take.piece() != Empty && type.piece() != Pawn && nature != Castle)
        clock = gs.clock + 1;

    // Update castle
    auto castle = gs.castle;
    castle.popSQ(source);
    castle.popSQ(target);

    // 
    auto enpass = gs.enpass;
    enpass[gs.turn] = OFFBOARD;
 
    if (nature == Stride)
    {
        board.popSQ(source);
        board.setSQ(target, type);
        enpass[gs.turn] = target - PUSH_DELTA[gs.turn];
    }

    else if (nature == Enpass)
    {
        board.popSQ(target + PUSH_DELTA[move.enpass()]);
        board.popSQ(source);
        board.setSQ(target, type);
    }
        
    else if (nature == Castle)
    {
        // Update King location
        board.popSQ(source);
        board.setSQ(target, PieceClass(King, gs.turn));

        // Update Rook location
        board.popSQ(SOURCE_CASTLE[setup][gs.turn][move.castle()]);
        board.setSQ(TARGET_CASTLE[setup][gs.turn][move.castle()], PieceClass(Rook, gs.turn));
    }

    else if (nature == Evolve)
    {
        board.popSQ(source);
        board.popSQ(target);
        board.setSQ(target, move.evolve());
    }

    else // Jumper, Slider, Pushed, Strike
    {
        board.popSQ(target);
        board.popSQ(source);
        board.setSQ(target, type);
    }

    states.emplace_back(clock, next(gs.turn), hash, castle, take, enpass);
}

void Position::undomove(Move move)
{
    const GameState& gs = states.back();

    auto source = move.source();
    auto target = move.target();
    auto nature = move.nature();
    
    auto type = board[target];
    auto take = gs.captured;

    if (nature == Enpass)
    {
        board.popSQ(target);
        board.setSQ(source, type);
        board.setSQ(target + PUSH_DELTA[move.enpass()], PieceClass(Pawn, move.enpass()));
    }

    else if (nature == Castle)
    {
        // Update King location
        board.popSQ(target);
        board.setSQ(source, type);

        // Update Rook location
        board.popSQ(TARGET_CASTLE[setup][gs.turn][move.castle()]);
        board.setSQ(SOURCE_CASTLE[setup][gs.turn][move.castle()], PieceClass(Rook, type.color()));
    }

    else if (nature == Evolve)
    {
        board.popSQ(target);
        board.setSQ(source, PieceClass(Pawn, type.color()));
    }

    else // Jumper, Slider, Pushed, Stride, Strike
    {
        board.popSQ(target);
        board.setSQ(target, take);
        board.setSQ(source, type);
    }

    states.pop_back();
}

} // namespace athena