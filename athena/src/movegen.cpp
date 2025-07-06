#include "movegen.h"

namespace athena
{

bool isSquareAttacked(const Position& pos, Square source, Color color) noexcept
{
    auto enemy = pos.board.opponent(color);

    if (enemy & pos.board.occ(Knight) & PIECE_ATTACK[Knight][source])
        return true;

    auto bBB = (pos.board.occ(Queen) | pos.board.occ(Bishop));
    auto rBB = (pos.board.occ(Queen) | pos.board.occ(Rook));

    bBB = (enemy & bBB & PIECE_ATTACK[Bishop][source]);
    rBB = (enemy & rBB & PIECE_ATTACK[Rook][source]);

    for (auto target: bBB)
        if (!(between(source, target, Bishop) & pos.board.everyone()))
            return true;

    for (auto target: rBB)
        if (!(between(source, target, Rook) & pos.board.everyone()))
            return true;

    for (auto opp: OPPONENTS[color])
        if (pos.board.occ(Pawn, opp) & COLOR_ATTACK[ally(opp)][source])
            return true;

    if (enemy & pos.board.occ(King) & PIECE_ATTACK[King][source])
        return true;

    return false;
}

bool isRoyalSafe(const Position& pos, Color color) noexcept {
    return !isSquareAttacked(pos, pos.board.royal(color), color);
}

inline auto genJumperMoves(const Position& pos, Move* moves, auto jumpers, auto allowed, Piece piece, MoveFlag flag)
{
    for (auto source: jumpers)
    {
        auto dest = PIECE_ATTACK[piece][source] & allowed;
        for (auto target: dest)
            *(moves++) = Move(source, target, Jumper, flag);
    }
    return moves;
}

inline auto genSliderMoves(const Position& pos, Move* moves, auto sliders, auto allowed, Piece piece, MoveFlag flag)
{
    for (auto source: sliders)
    {
        auto dest = PIECE_ATTACK[piece][source] & allowed;
        for (auto target: dest)
            if ((between(source, target, piece) & pos.board.everyone()).empty()) 
                *(moves++) = Move(source, target, Slider, flag); 
    }
    return moves;
}

inline auto genPushMoves(const Position& pos, Move* moves)
{
    const GameState& gs = pos.states.back();

    auto sS = PUSH_DELTA[gs.turn];
    auto sD = static_cast<Shift>(sS * 2);

    auto empty = ~pos.board.everyone() & ~BRICK;
    auto pawns = pos.board.occ(Pawn, gs.turn) & ~PROMOTE[gs.turn];
    auto pushS = pawns.shift(sS) & empty;
    auto valid = pushS & HOMERANK[gs.turn];
    auto pushD = valid.shift(sS) & empty;

    for (auto target: pushS) *(moves++) = Move(target - sS, target, Pushed, Quiet);
    for (auto target: pushD) *(moves++) = Move(target - sD, target, Stride, Quiet);

    return moves;
}

inline auto genTakeMoves(const Position& pos, Move* moves)
{
    const GameState& gs = pos.states.back();

    auto sE = TAKE_DELTA[gs.turn][0];
    auto sW = TAKE_DELTA[gs.turn][1];

    auto enemy = pos.board.opponent(gs.turn);
    auto pawns = pos.board.occ(Pawn, gs.turn) & ~PROMOTE[gs.turn];
    auto takeE = pawns.shift(sE) & enemy;
    auto takeW = pawns.shift(sW) & enemy;

    for (auto target: takeE) *(moves++) = Move(target - sE, target, Strike, Noisy);
    for (auto target: takeW) *(moves++) = Move(target - sW, target, Strike, Noisy);

    return moves;
}

inline auto genEvolveMoves(const Position& pos, Move* moves)
{
    const GameState& gs = pos.states.back();

    auto s1 = PUSH_DELTA[gs.turn];
    auto s2 = TAKE_DELTA[gs.turn][0];
    auto s3 = TAKE_DELTA[gs.turn][1];

    auto pawns = pos.board.occ(Pawn, gs.turn) & PROMOTE[gs.turn]; 
    for (auto source: pawns)
    {
        auto dest = BB(); 

        auto empty = ~pos.board.everyone() & ~BRICK;
        auto enemy =  pos.board.opponent(gs.turn);

        auto pushS = static_cast<Square>(source + s1);
        auto takeE = static_cast<Square>(source + s2);
        auto takeW = static_cast<Square>(source + s3);
        
        if (empty.checkSQ(pushS)) dest.setSQ(pushS);
        if (enemy.checkSQ(takeE)) dest.setSQ(takeE);
        if (enemy.checkSQ(takeW)) dest.setSQ(takeW);

        for (auto target: dest)
        {
            *(moves++) = Move(source, target, PieceClass(Queen,  gs.turn));
            *(moves++) = Move(source, target, PieceClass(Rook,   gs.turn));
            *(moves++) = Move(source, target, PieceClass(Bishop, gs.turn));
            *(moves++) = Move(source, target, PieceClass(Knight, gs.turn));
        }
    }

    return moves;
}

inline auto genEnpassMoves(const Position& pos, Move* moves)
{
    const GameState& gs = pos.states.back();
    for (auto opp: OPPONENTS[gs.turn])
    {
        auto epsq  = gs.enpass[opp];
        if (epsq != OFFBOARD) 
        {
            auto pawns = pos.board.occ(Pawn, gs.turn) & COLOR_ATTACK[opp][epsq]; 
            for (auto source: pawns)
                *(moves++) = Move(source, epsq, opp);
        }
    }
    return moves;
}

inline auto genCastleMoves(const Position& pos, Move* moves)
{
    const GameState& gs = pos.states.back();
    for (auto side: SIDES)
    {
        if (!hasCastle(gs.castle, RIGHTS[pos.setup][gs.turn][side]))
            continue;

        if (pos.board.everyone() & PASS[pos.setup][gs.turn][side])
            continue;

        bool isCastle = true;
        auto safe = SAFE[pos.setup][gs.turn][side];
        for (auto sq: safe)
            if (isSquareAttacked(pos, sq, gs.turn))
                isCastle = false;

        if (isCastle) *(moves++) = CASTLE_MOVES[pos.setup][gs.turn][side];
    }
    return moves;
}

template<MoveFlag flag>
inline auto genMoves(const Position& pos, Move* moves)
{
    const GameState& gs = pos.states.back();

    BitBoard allowed;
    if constexpr (flag == Noisy) allowed =  pos.board.opponent(gs.turn);
    if constexpr (flag == Quiet) allowed = ~pos.board.everyone() & ~BRICK;

    auto own = pos.board.occ(gs.turn);
    auto rBB = own & pos.board.occ(Rook,   Queen);
    auto bBB = own & pos.board.occ(Bishop, Queen);
    auto nBB = own & pos.board.occ(Knight);
    auto kBB = own & pos.board.occ(King);

    // Generate piece moves
    moves = genSliderMoves(pos, moves, rBB, allowed, Rook  , flag);
    moves = genSliderMoves(pos, moves, bBB, allowed, Bishop, flag);
    moves = genJumperMoves(pos, moves, nBB, allowed, Knight, flag);
    moves = genJumperMoves(pos, moves, kBB, allowed, King  , flag);
    
    // Generate color moves
    if constexpr (flag == Noisy) moves = genPushMoves(pos, moves);
    if constexpr (flag == Quiet) moves = genTakeMoves(pos, moves);

    // Generate speical moves
    if constexpr (flag == Noisy) moves = genEvolveMoves(pos, moves);
    if constexpr (flag == Noisy) moves = genEnpassMoves(pos, moves);
    if constexpr (flag == Quiet) moves = genCastleMoves(pos, moves);

    return moves;
}

std::size_t genAllNoisyMoves(const Position& pos, Move* moves) { return (genMoves<Noisy>(pos, moves) - moves); }
std::size_t genAllQuietMoves(const Position& pos, Move* moves) { return (genMoves<Quiet>(pos, moves) - moves); }

} // namespace athena