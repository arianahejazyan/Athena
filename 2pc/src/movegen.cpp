#include "attacks.h"
#include "movegen.h"
#include "mask.h"

namespace Talia
{

template<Piece piece>
static inline auto addSlideMoves(const Position& pos, Move* moves, u64 slider, const u64 legal)
{
    const u64 everyone = pos.everyone();

    while (slider) 
    {
        auto source = poplsb(slider);
        auto target = attacks<piece>(source, everyone) & legal;

        if (pos.pinned & BB(source)) 
            target &= LINE[getlsb(pos.teammate(KING))][source];

        while (target) *(moves++) = Move(source, poplsb(target), SIMPLE);
    }

    return moves;
}

template<MoveType movetype, Color turn>
static inline auto genSlideMoves(const Position& pos, Move* moves)
{
    auto queens  = pos.teammate(QUEEN);
    auto rooks   = pos.teammate(ROOK);
    auto bishops = pos.teammate(BISHOP);

    u64 legal;
    if constexpr (movetype == NOISY) legal =  pos.opponent() & pos.checks;
    if constexpr (movetype == QUIET) legal = ~pos.everyone() & pos.checks;

    moves = addSlideMoves<QUEEN>(pos, moves, queens, legal);
    moves = addSlideMoves<ROOK>(pos, moves, rooks, legal);
    moves = addSlideMoves<BISHOP>(pos, moves, bishops, legal);

    return moves;
}

template<Piece piece>
static inline auto addCrawlMoves(const Position& pos, Move* moves, u64 crawler, const u64 legal)
{
    while (crawler) 
    {
        auto source = poplsb(crawler);
        auto target = attacks(source, piece) & legal;

        if (pos.pinned & BB(source)) 
            target &= LINE[getlsb(pos.teammate(KING))][source];

        while (target) *(moves++) = Move(source, poplsb(target), SIMPLE);
    }

    return moves;
}

template<MoveType movetype, Color turn>
static inline auto genCrawlMoves(const Position& pos, Move* moves)
{
    auto knights = pos.teammate(KNIGHT);

    u64 legal;
    if constexpr (movetype == NOISY) legal =  pos.opponent() & pos.checks; 
    if constexpr (movetype == QUIET) legal = ~pos.everyone() & pos.checks;

    moves = addCrawlMoves<KNIGHT>(pos, moves, knights, legal);

    return moves;
}

template<bool isPromotion, Offset offset>
static inline auto addMarchMoves(Move* moves, u64 mask, const MoveFlag flag)
{
    while (mask)
    {
        const auto target = poplsb(mask);
        const auto source = (Square)((int)target - (int)offset);

        if constexpr (isPromotion)
        {
            *(moves++) = Move(source, target, FLAG_QUEEN);
            *(moves++) = Move(source, target, FLAG_ROOK);
            *(moves++) = Move(source, target, FLAG_BISHOP);
            *(moves++) = Move(source, target, FLAG_KNIGHT);
        }

        else *(moves++) = Move(source, target, flag);
    }

    return moves;
}

template<Color turn, bool isPromotion>
static inline auto genPushMoves(const Position& pos, Move* moves, const u64 pawns)
{
    constexpr auto UP = (turn == WHITE ? NORTH : SOUTH); 
    constexpr auto UU = (turn == WHITE ? NN    : SS);

    auto nonpinned = pawns & ~pos.pinned;

    auto mask1 = shift<UP>(nonpinned)              & ~pos.everyone();
    auto mask2 = shift<UP>(mask1 & homerank(turn)) & ~pos.everyone();

    mask1 &= pos.checks;
    mask2 &= pos.checks;

    moves = addMarchMoves<isPromotion, UP>(moves, mask1, SIMPLE);
    moves = addMarchMoves<isPromotion, UU>(moves, mask2, DOUBLE);

    auto pinned = pawns & pos.pinned;

    while (pinned)
    {
        auto source = poplsb(pinned);

        auto mask1 = ~pos.everyone() & shift<UP>(BB(source));
        auto mask2 = ~pos.everyone() & shift<UP>(mask1 & homerank(turn));

        mask1 &= pos.checks & LINE[getlsb(pos.teammate(KING))][source];
        mask2 &= pos.checks & LINE[getlsb(pos.teammate(KING))][source];

        moves = addMarchMoves<isPromotion, UP>(moves, mask1, SIMPLE);
        moves = addMarchMoves<isPromotion, UU>(moves, mask2, DOUBLE);
    }

    return moves;
}

template<Color turn, bool isPromotion>
static inline auto genTakeMoves(const Position& pos, Move* moves, const u64 pawns)
{
    constexpr auto R = (turn == WHITE ? NE : SW);
    constexpr auto L = (turn == WHITE ? NW : SE);

    auto nonpinned = pawns & ~pos.pinned;

    auto maskR = shift<R>(nonpinned) & pos.opponent() & pos.checks;
    auto maskL = shift<L>(nonpinned) & pos.opponent() & pos.checks;

    moves = addMarchMoves<isPromotion, R>(moves, maskR, SIMPLE);
    moves = addMarchMoves<isPromotion, L>(moves, maskL, SIMPLE);

    auto pinned = pawns & pos.pinned; 

    while (pinned)
    {
        auto source = poplsb(pinned);

        auto maskR = shift<R>(BB(source)) & pos.opponent() & pos.checks & LINE[getlsb(pos.teammate(KING))][source]; 
        auto maskL = shift<L>(BB(source)) & pos.opponent() & pos.checks & LINE[getlsb(pos.teammate(KING))][source];
    
        moves = addMarchMoves<isPromotion, R>(moves, maskR, SIMPLE);
        moves = addMarchMoves<isPromotion, L>(moves, maskL, SIMPLE);
    }

    return moves;
}

template<MoveType movetype, Color turn>
static inline auto genMarchMoves(const Position& pos, Move* moves)
{
    auto simple = pos.teammate(PAWN) & ~promotionrank(turn);
    auto evolve = pos.teammate(PAWN) &  promotionrank(turn);

    if constexpr (movetype == NOISY)
    {
        moves = genTakeMoves<turn, true>(pos, moves, evolve);
        moves = genPushMoves<turn, true>(pos, moves, evolve);
        moves = genTakeMoves<turn, false>(pos, moves, simple);
    }

    else moves = genPushMoves<turn, false>(pos, moves, simple);

    return moves;
}

template<MoveType movetype, Color turn>
static inline auto genRoyalMoves(const Position& pos, Move* moves)
{
    auto source = getlsb(pos.teammate(KING)); 
    auto target = attacks(source, KING);

    if constexpr (movetype == NOISY) target &=  pos.opponent() & ~pos.threat;
    if constexpr (movetype == QUIET) target &= ~pos.everyone() & ~pos.threat;

    while (target) *(moves++) = Move(source, poplsb(target), SIMPLE);

    return moves;
}

template<Color turn>
static inline auto genEnpassMoves(const Position& pos, Move* moves)
{
    auto target = pos.state().epsq;

    if (target != OFFBOARD)
    {
        BitBoard mask = attacks(target, next(turn)) & pos.teammate(PAWN); 
 
        while (mask)
        {
            Square source = poplsb(mask);
            Square royal  = getlsb(pos.teammate(KING));

            BitBoard filter = ~BB(source) & ~BB(static_cast<Square>((int)target + (turn == WHITE ? (int)SOUTH : (int)NORTH)));

            bool cond1 = rankbb(royal) & (pos.opponent(ROOK) | pos.opponent(QUEEN));
            bool cond2 = attacks<ROOK>(royal, pos.everyone() & filter) & (pos.opponent(ROOK) | pos.opponent(QUEEN));

            if (cond1 && cond2) continue; 

            bool cond3 = BB(source) & pos.pinned;
            bool cond4 = BB(target) & LINE[royal][source];

            if (cond3 && !cond4) continue;

            *(moves++) = Move(source, target, ENPASS);
        }
    }

    return moves;
}

template<Color turn>
static inline auto genCastleMoves(const Position& pos, Move* moves)
{
    constexpr auto SIDE =  (turn == WHITE ? WHITESIDE : BLACKSIDE);

    constexpr auto S = (turn == WHITE ? E1 : E8);
    constexpr auto R = (turn == WHITE ? G1 : G8);
    constexpr auto L = (turn == WHITE ? C1 : C8);

    constexpr auto sideR = RIGHT & SIDE & KINGSIDE;
    constexpr auto sideL = RIGHT & SIDE & QUEENSIDE;
    constexpr auto safeR = SAFE  & SIDE & KINGSIDE;
    constexpr auto safeL = SAFE  & SIDE & QUEENSIDE;
    constexpr auto passR = PASS  & SIDE & KINGSIDE;
    constexpr auto passL = PASS  & SIDE & QUEENSIDE;

    bool condR = ((pos.state().castle & sideR) == sideR) && !(pos.threat & safeR) && !(pos.everyone() & passR);
    bool condL = ((pos.state().castle & sideL) == sideL) && !(pos.threat & safeL) && !(pos.everyone() & passL);

    if (condR) *(moves++) = Move(S, R, CASTLE);
    if (condL) *(moves++) = Move(S, L, CASTLE);

    return moves;
}

template<MoveType movetype, Color turn>
static auto genLegalMoves(const Position& pos, Move* moves)
{
    moves = genSlideMoves<movetype, turn>(pos, moves);
    moves = genCrawlMoves<movetype, turn>(pos, moves);
    moves = genMarchMoves<movetype, turn>(pos, moves);
    moves = genRoyalMoves<movetype, turn>(pos, moves);

    if constexpr (movetype == NOISY) moves = genEnpassMoves<turn>(pos, moves);
    if constexpr (movetype == QUIET) moves = genCastleMoves<turn>(pos, moves);

    return moves;
}

[[nodiscard]] int genAllNoisyMoves(Position& pos, Move* moves)
{
    pos.computePinned();
    pos.computeChecks();
    pos.computeThreat();

    const Move* start = moves;

    if (pos.turn == WHITE)
         moves = genLegalMoves<NOISY, WHITE>(pos, moves);
    else moves = genLegalMoves<NOISY, BLACK>(pos, moves);
    
    return moves - start;
}

[[nodiscard]] int genAllQuietMoves(Position& pos, Move* moves)
{
    pos.computePinned();
    pos.computeChecks();
    pos.computeThreat();

    const Move* start = moves;

    if (pos.turn == WHITE)
         moves = genLegalMoves<QUIET, WHITE>(pos, moves);
    else moves = genLegalMoves<QUIET, BLACK>(pos, moves);

    return moves - start;
}

[[nodiscard]] int genAllLegalMoves(Position& pos, Move* moves)
{
    pos.computePinned();
    pos.computeChecks();
    pos.computeThreat();

    const Move* start = moves;

    if (pos.turn == WHITE)
         moves = genLegalMoves<NOISY, WHITE>(pos, moves), moves = genLegalMoves<QUIET, WHITE>(pos, moves); 
    else moves = genLegalMoves<NOISY, BLACK>(pos, moves), moves = genLegalMoves<QUIET, BLACK>(pos, moves); 

    return moves - start;
}

} // namespace