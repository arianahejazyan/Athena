#include <iostream>
#include "move.h"
#include "utility.h"

namespace athena
{

const char* Move::uci(bool debug) const
{
    static char uci_move[8];
    char* curr = uci_move;

    const char* src = toString(source(), debug);
    const char* tgt = toString(target(), debug);

    while (*src) *(curr++) = *(src++);
    while (*tgt) *(curr++) = *(tgt++);

    if (type() == Type(Type::Evolve)) curr = toString(curr, evolve().piece(), true);

    *curr = '\0';
    return uci_move;
}

void MoveList::print(bool legal, bool debug) const
{
    int total = 0;
    for (int i = 0; i < size; i++)
    {
        std::cout << moves[i].uci(debug) << "\n", total++;
    }
    std::cout << "total " << (!legal ? "pseudo legal" : "legal") << " moves: " << total << std::endl;
}


// inline const char* encodeString(SQ sq, bool debug)
// {
//     static constexpr char TABLE[SQUARE_NB][4] =
//     {
//         "a1" , "b1" , "c1" , "d1" , "e1" , "f1",  "g1",  "h1" , "i1" , "j1" , "k1" , "l1" , "m1" , "n1" , "o1" , "p1" ,
//         "a2" , "b2" , "c2" , "d2" , "e2" , "f2",  "g2",  "h2" , "i2" , "j2" , "k2" , "l2" , "m2" , "n2" , "o2" , "p2" ,
//         "a3" , "b3" , "c3" , "d3" , "e3" , "f3",  "g3",  "h3" , "i3" , "j3" , "k3" , "l3" , "m3" , "n3" , "o3" , "p3" ,
//         "a4" , "b4" , "c4" , "d4" , "e4" , "f4",  "g4",  "h4" , "i4" , "j4" , "k4" , "l4" , "m4" , "n4" , "o4" , "p4" ,
//         "a5" , "b5" , "c5" , "d5" , "e5" , "f5",  "g5",  "h5" , "i5" , "j5" , "k5" , "l5" , "m5" , "n5" , "o5" , "p5" ,
//         "a6" , "b6" , "c6" , "d6" , "e6" , "f6",  "g6",  "h6" , "i6" , "j6" , "k6" , "l6" , "m6" , "n6" , "o6" , "p6" ,
//         "a7" , "b7" , "c7" , "d7" , "e7" , "f7",  "g7",  "h7" , "i7" , "j7" , "k7" , "l7" , "m7" , "n7" , "o7" , "p7" ,
//         "a8" , "b8" , "c8" , "d8" , "e8" , "f8",  "g8",  "h8" , "i8" , "j8" , "k8" , "l8" , "m8" , "n8" , "o8" , "p8" ,
//         "a9" , "b9" , "c9" , "d9" , "e9" , "f9",  "g9",  "h9" , "i9" , "j9" , "k9" , "l9" , "m9" , "n9" , "o9" , "p9" ,
//         "a10", "b10", "c10", "d10", "e10", "f10", "g10", "h10", "i10", "j10", "k10", "l10", "m10", "n10", "o10", "p10",
//         "a11", "b11", "c11", "d11", "e11", "f11", "g11", "h11", "i11", "j11", "k11", "l11", "m11", "n11", "o11", "p11",
//         "a12", "b12", "c12", "d12", "e12", "f12", "g12", "h12", "i12", "j12", "k12", "l12", "m12", "n12", "o12", "p12",
//         "a13", "b13", "c13", "d13", "e13", "f13", "g13", "h13", "i13", "j13", "k13", "l13", "m13", "n13", "o13", "p13",
//         "a14", "b14", "c14", "d14", "e14", "f14", "g14", "h14", "i14", "j14", "k14", "l14", "m14", "n14", "o14", "p14",
//         "a15", "b15", "c15", "d15", "e15", "f15", "g15", "h15", "i15", "j15", "k15", "l15", "m15", "n15", "o15", "p15",
//         "a16", "b16", "c16", "d16", "e16", "f16", "g16", "h16", "i16", "j16", "k16", "l16", "m16", "n16", "o16", "p16",
//     };
//     return TABLE[debug ? sq : (sq - 17)];
// }

// inline char* encodeString(char* out, Piece piece, bool lowercase = false)
// {
//     switch (piece)
//     {
//     case P : *(out++) = lowercase ? 'p' : 'P'; break;
//     case N : *(out++) = lowercase ? 'n' : 'N'; break;
//     case B : *(out++) = lowercase ? 'b' : 'B'; break;
//     case R : *(out++) = lowercase ? 'r' : 'R'; break;
//     case Q : *(out++) = lowercase ? 'q' : 'Q'; break;
//     case K : *(out++) = lowercase ? 'k' : 'K'; break;
//     default: break;
//     }
//     return out;
// }



    // template <Piece piece, std::size_t chunk>
    // inline BB &gen_slide_attacks(SQ sq, const BB &occ, BB &attacks)
    // {
    //     static_assert(piece == Rook || piece == Bishop,
    //                   "gen_slide_attacks: only Rook and Bishop are supported.");

    //     if constexpr (piece == Rook)
    //     {
    //         hyperbola_quintessence<chunk>(sq, occ, VERTICAL_ATTACK[sq], attacks);
    //         obstruction_difference<chunk>(sq, occ, attacks);
    //     }

    //     if constexpr (piece == Bishop)
    //     {
    //         BB temp;
    //         hyperbola_quintessence<chunk>(sq, occ, DIAGONAL_ATTACK[sq], temp);
    //         hyperbola_quintessence<chunk>(sq, occ, ANTIDIAGONAL_ATTACK[sq], attacks);
    //         attacks |= temp;
    //     }

    //     return attacks;
    // }

    // inline bool collision(const BB &bb1, const BB &bb2)
    // {
    //     return (bb1.chunks[0] & bb2.chunks[0]) ||
    //            (bb1.chunks[1] & bb2.chunks[1]) ||
    //            (bb1.chunks[2] & bb2.chunks[2]) ||
    //            (bb1.chunks[3] & bb2.chunks[3]);
    // }

    // template <Player player, std::size_t chunk>
    // bool isSquareAttacked(const Position &pos, Square sq)
    // {
    //     BB bishop, rook;
    //     constexpr auto opp1 = toGuild(player) == RY ? Blue : Red;
    //     constexpr auto opp2 = toGuild(player) == RY ? Green : Yellow;

    //     return

    //         collision(pos.opp<Knight, player>(), KNIGHT_ATTACK[sq]) ||

    //         (collision(pos.opp<Bishop, player>(), BISHOP_ATTACK[sq]) &&
    //          collision(pos.opp<Bishop, player>(), gen_slide_attacks<Bishop, chunk>(sq, pos.occ<EVERYONE>(), bishop))) ||

    //         (collision(pos.opp<Rook, player>(), Rook_ATTACK[sq]) &&
    //          collision(pos.opp<Rook, player>(), gen_slide_attacks<Rook, chunk>(sq, pos.occ<EVERYONE>(), rook))) ||

    //         collision(pos.occ<Pawn, opp1>(), PAWN_ATTACKS[opp2][sq]) ||
    //         collision(pos.occ<Pawn, opp2>(), PAWN_ATTACKS[opp1][sq]) ||

    //         (collision(pos.opp<King, player>(), KNIGHT_ATTACK[sq]));
    // }

    // template <std::size_t chunk>
    // inline auto extract_moves(Move *moves, BB &attacks, int delta)
    // {
    //     auto &line = attacks.chunks[chunk];
    //     while (line)
    //     {
    //         int target = 64 * chunk + __builtin_ctzll(line);
    //         int source = target - delta;
    //         *(moves++) = Move(static_cast<Square>(source), static_cast<Square>(target));
    //         line &= (line - 1);
    //     }
    //     return moves;
    // }

    // template <Color color>
    // inline auto gen_all_push_moves(const Position &pos, Move *moves)
    // {
    //     BB push(pos.occ<Pawn, color>());
    //     push &= ~PROMOTE[color];
    //     shift<color, 0>(push);
    //     push &= pos.unfilled();

    //     BB advance(push);
    //     advance &= HOMERANK[color];
    //     shift<color, 0>(advance);
    //     advance &= pos.unfilled();

    //     // generate all push moves
    //     moves = extract_moves<0>(moves, push, PAWN_DELTA[color][0]);
    //     moves = extract_moves<1>(moves, push, PAWN_DELTA[color][0]);
    //     moves = extract_moves<2>(moves, push, PAWN_DELTA[color][0]);
    //     moves = extract_moves<3>(moves, push, PAWN_DELTA[color][0]);

    //     // generate all advance moves
    //     moves = extract_moves<0>(moves, advance, PAWN_DELTA[color][0] * 2);
    //     moves = extract_moves<1>(moves, advance, PAWN_DELTA[color][0] * 2);
    //     moves = extract_moves<2>(moves, advance, PAWN_DELTA[color][0] * 2);
    //     moves = extract_moves<3>(moves, advance, PAWN_DELTA[color][0] * 2);

    //     return moves;
    // }

    // template <Color color>
    // inline auto gen_all_take_moves(const Position &pos, Move *moves)
    // {
    //     BB takeR(pos.occ<Pawn, color>());
    //     takeR &= ~PROMOTE[color];
    //     BB takeL(takeR);
    //     shift<color, 1>(takeR);
    //     shift<color, 2>(takeL);
    //     takeR &= pos.opponent<color>();
    //     takeL &= pos.opponent<color>();

    //     // generate all R take moves
    //     moves = extract_moves<0>(moves, takeR, PAWN_DELTA[color][1]);
    //     moves = extract_moves<1>(moves, takeR, PAWN_DELTA[color][1]);
    //     moves = extract_moves<2>(moves, takeR, PAWN_DELTA[color][1]);
    //     moves = extract_moves<3>(moves, takeR, PAWN_DELTA[color][1]);

    //     // generate all L take moves
    //     moves = extract_moves<0>(moves, takeL, PAWN_DELTA[color][2]);
    //     moves = extract_moves<1>(moves, takeL, PAWN_DELTA[color][2]);
    //     moves = extract_moves<2>(moves, takeL, PAWN_DELTA[color][2]);
    //     moves = extract_moves<3>(moves, takeL, PAWN_DELTA[color][2]);

    //     return moves;
    // }

    //     template <Color color>
    //     inline auto gen_promotion_moves(const Position &pos, Move *moves)
    //     {
    //         // const GameState &gs = pos.states.back();

    //         // auto s1 = PUSH_DELTA[gs.turn];
    //         // auto s2 = TAKE_DELTA[gs.turn][0];
    //         // auto s3 = TAKE_DELTA[gs.turn][1];

    //         // auto pawns = pos.board.occ(Pawn, gs.turn) & PROMOTE[gs.turn];

    //         BB pawns(pos.board.occ(Pawn, color));
    //         pawns &= PROMOTE[color];

    //         // early exit
    //         if (!pawns.empty())
    //         {
    //             for (std::size_t chunk = 0; chunk < CHUNK_NB; ++chunk)
    //             {
    //                 while (pawns.chunks[chunk])
    //                 {
    //                     auto source = static_cast<Square>(64 * chunk + __builtin_ctzll(pawns.chunks[chunk]));

    //                     auto t1 = source + R;
    //                     auto t2 = source + U;
    //                     auto t2 = source + L;

    //                     if (pos.board[t1].color() == enemy)
    //                     {
    //                         *(moves++) = Move(source, target, PieceClass(Queen, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Rook, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Bishop, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Knight, gs.turn));
    //                     }

    //                     if (pos.board[t1].color() == enemy)
    //                     {
    //                         *(moves++) = Move(source, target, PieceClass(Queen, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Rook, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Bishop, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Knight, gs.turn));
    //                     }

    //                     if (pos.board[t1].color() == enemy)
    //                     {
    //                         *(moves++) = Move(source, target, PieceClass(Queen, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Rook, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Bishop, gs.turn));
    //                         *(moves++) = Move(source, target, PieceClass(Knight, gs.turn));
    //                     }
    //                 }
    //             }
    //         }

    //         // for (auto source : pawns)
    //         // {
    //         //     auto dest = BB();

    //         //     auto empty = ~pos.board.everyone() & ~BRICK;
    //         //     auto enemy = pos.board.opponent(gs.turn);

    //         //     auto pushS = static_cast<Square>(source + s1);
    //         //     auto takeE = static_cast<Square>(source + s2);
    //         //     auto takeW = static_cast<Square>(source + s3);

    //         //     if (empty.checkSQ(pushS))
    //         //         dest.setSQ(pushS);
    //         //     if (enemy.checkSQ(takeE))
    //         //         dest.setSQ(takeE);
    //         //     if (enemy.checkSQ(takeW))
    //         //         dest.setSQ(takeW);

    //         //     for (auto target : dest)
    //         //     {
    //         //         *(moves++) = Move(source, target, PieceClass(Queen, gs.turn));
    //         //         *(moves++) = Move(source, target, PieceClass(Rook, gs.turn));
    //         //         *(moves++) = Move(source, target, PieceClass(Bishop, gs.turn));
    //         //         *(moves++) = Move(source, target, PieceClass(Knight, gs.turn));
    //         //     }
    //         // }

    //         return moves;
    //     }

    //     template <Color color>
    //     inline auto gen_enpass_moves(const Position &pos, Move *moves)
    //     {
    //         const GameState &gs = pos.states.back(); // fix design

    //         const auto opp1 = constexpr(toGuild(color) == RY ? Blue : Red);
    //         const auto opp2 = constexpr(toGuild(color) == RY ? Green : Yellow);

    //         auto ep1 = gs.enpass[opp1];
    //         if (ep1 != OFFBOARD)
    //         {
    //             auto s1 = ep1 + EAST_ATTACK[ally(color)];
    //             auto s2 = ep1 + WEST_ATTACK[ally(color)];

    //             if (pos.at(s1) == PieceClass(Pawn, color))
    //                 *(moves++) = Move(s1, ep1, opp1);

    //             if (pos.at(s2) == PieceClass(Pawn, color))
    //                 *(moves++) = Move(s2, ep1, opp1);
    //         }

    //         auto ep2 = gs.enpass[opp2];
    //         if (gs.enpass[opp2] != OFFBOARD)
    //         {
    //             auto s1 = ep2 + EAST_ATTACK[ally(color)];
    //             auto s2 = ep2 + WEST_ATTACK[ally(color)];

    //             if (pos.at(s1) == PieceClass(Pawn, color))
    //                 *(moves++) = Move(s1, ep2, opp2);

    //             if (pos.at(s2) == PieceClass(Pawn, color))
    //                 *(moves++) = Move(s2, ep2, opp2);
    //         }

    //         // const GameState &gs = pos.states.back();
    //         // for (auto opp : OPPONENTS[gs.turn])
    //         // {
    //         //     auto epsq = gs.enpass[opp];
    //         //     if (epsq != OFFBOARD)
    //         //     {
    //         //         auto pawns = pos.board.occ(Pawn, gs.turn) & COLOR_ATTACK[opp][epsq];
    //         //         for (auto source : pawns)
    //         //             *(moves++) = Move(source, epsq, opp);
    //         //     }
    //         // }
    //         return moves;
    //     }

    //     template <Color color>
    //     inline auto gen_castle_moves(const Position &pos, Move *moves)
    //     {
    //         const GameState &gs = pos.states.back(); // ffix

    //         // King castling rights
    //         if (gs.castle.checkSQ(CASTLE_KING_SOURCE[color]))
    //         {
    //             // Rook castling right
    //             if (gs.castle.checkSQ(CASTLE_ROOK_SOURCE[color][KingSide]))
    //             {
    //                 BB pass(PASS[color][KingSide][pos.setup]);
    //                 pass &= pos.everyone();

    //                 // Passing right
    //                 if (pass.empty())
    //                 {
    //                     auto safe1 = SAFE[color][KingSide][0][pos.setup];
    //                     auto safe2 = SAFE[color][KingSide][1][pos.setup];

    //                     if (!isSquareAttacked<color>(pos, safe1) && !isSquareAttacked<color>(pos, safe2))
    //                         *(moves++) = CASTLE_MOVES[pos.setup][gs.turn][side];
    //                 }
    //             }
    //         }

    //         // const GameState &gs = pos.states.back();
    //         // for (auto side : SIDES)
    //         // {
    //         //     if (!hasCastle(gs.castle, RIGHTS[pos.setup][gs.turn][side]))
    //         //         continue;

    //         //     if (pos.board.everyone() & PASS[pos.setup][gs.turn][side])
    //         //         continue;

    //         //     bool isCastle = true;
    //         //     auto safe = SAFE[pos.setup][gs.turn][side];
    //         //     for (auto sq : safe)
    //         //         if (isSquareAttacked(pos, sq, gs.turn))
    //         //             isCastle = false;

    //         //     if (isCastle)
    //         //         *(moves++) = CASTLE_MOVES[pos.setup][gs.turn][side];
    //         // }
    //         return moves;
    //     }

    // template <std::size_t chunk>
    // inline auto extract_moves(Move *moves, Square sq, BB &attacks)
    // {
    //     auto &line = attacks.chunks[chunk];
    //     while (line)
    //     {
    //         int bit = __builtin_ctzll(line);
    //         *(moves++) = Move(sq, static_cast<Square>(64 * chunk + bit));
    //         line &= (line - 1);
    //     }
    //     return moves;
    // }

    // template <std::size_t chunk>
    // auto gen_all_rook_moves(const Position &pos, Move *moves, BB &rooks, const BB &allowed)
    // {
    //     auto &lane = rooks.chunks[chunk];
    //     while (lane)
    //     {
    //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
    //         lane &= (lane - 1);

    //         BB attacks;
    //         hyperbola_quintessence(sq, pos.everyone(), VERTICAL_ATTACK[sq], attacks);
    //         obstruction_difference(sq, pos.everyone(), attacks);
    //         attacks &= allowed;

    //         moves = extract_moves<0>(moves, sq, attacks);
    //         moves = extract_moves<1>(moves, sq, attacks);
    //         moves = extract_moves<2>(moves, sq, attacks);
    //         moves = extract_moves<3>(moves, sq, attacks);
    //     }

    //     return moves;
    // }

    // template <std::size_t chunk>
    // auto gen_all_bishop_moves(const Position &pos, Move *moves, BB &bishops, const BB &allowed)
    // {
    //     auto &lane = bishops.chunks[chunk];
    //     while (lane)
    //     {
    //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
    //         lane &= (lane - 1);

    //         BB diagonal;
    //         BB attacks;
    //         hyperbola_quintessence(sq, pos.everyone(), DIAGONAL_ATTACK[sq], diagonal);
    //         hyperbola_quintessence(sq, pos.everyone(), ANTIDIAGONAL_ATTACK[sq], attacks);
    //         attacks |= diagonal;
    //         attacks &= allowed;

    //         // std::cout << "CHUNK= " << (int)chunk << std::endl;
    //         // diagonal.print();

    //         moves = extract_moves<0>(moves, sq, attacks);
    //         moves = extract_moves<1>(moves, sq, attacks);
    //         moves = extract_moves<2>(moves, sq, attacks);
    //         moves = extract_moves<3>(moves, sq, attacks);
    //     }

    //     return moves;
    // }

    // template <std::size_t chunk>
    // auto gen_all_knight_moves(Move *moves, BB &knights, const BB &allowed)
    // {
    //     auto &lane = knights.chunks[chunk];
    //     while (lane)
    //     {
    //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
    //         lane &= (lane - 1);

    //         BB attacks(KNIGHT_ATTACK[sq]);
    //         attacks &= allowed;

    //         moves = extract_moves<0>(moves, sq, attacks);
    //         moves = extract_moves<1>(moves, sq, attacks);
    //         moves = extract_moves<2>(moves, sq, attacks);
    //         moves = extract_moves<3>(moves, sq, attacks);
    //     }

    //     return moves;
    // }

    // template <std::size_t chunk>
    // auto gen_all_king_moves(Move *moves, BB &king, const BB &allowed)
    // {
    //     auto &lane = king.chunks[chunk];
    //     while (lane)
    //     {
    //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
    //         lane &= (lane - 1);

    //         BB attacks(KING_ATTACK[sq]);
    //         attacks &= allowed;

    //         moves = extract_moves<0>(moves, sq, attacks);
    //         moves = extract_moves<1>(moves, sq, attacks);
    //         moves = extract_moves<2>(moves, sq, attacks);
    //         moves = extract_moves<3>(moves, sq, attacks);
    //     }

    //     return moves;
    // }

    // template <MoveFlag flag, Color color>
    // std::size_t gen_moves(const Position &pos, Move *moves)
    // {
    //     const auto start = moves;

    //     BitBoard allowed;
    //     if constexpr (flag == Noisy)
    //         allowed = pos.opponent<color>();
    //     if constexpr (flag == Quiet)
    //         allowed = pos.unfilled();

    //     // gen all rook moves
    //     BB rBB(pos.occ<Rook, color>());
    //     rBB |= pos.occ<Queen, color>();
    //     moves = gen_all_rook_moves<0>(pos, moves, rBB, allowed);
    //     moves = gen_all_rook_moves<1>(pos, moves, rBB, allowed);
    //     moves = gen_all_rook_moves<2>(pos, moves, rBB, allowed);
    //     moves = gen_all_rook_moves<3>(pos, moves, rBB, allowed);

    //     // gen all bishp moves
    //     BB bBB(pos.occ<Bishop, color>());
    //     bBB |= pos.occ<Queen, color>();
    //     moves = gen_all_bishop_moves<0>(pos, moves, bBB, allowed);
    //     moves = gen_all_bishop_moves<1>(pos, moves, bBB, allowed);
    //     moves = gen_all_bishop_moves<2>(pos, moves, bBB, allowed);
    //     moves = gen_all_bishop_moves<3>(pos, moves, bBB, allowed);

    //     // gen all knight moves
    //     BB nBB(pos.occ<Knight, color>());
    //     moves = gen_all_knight_moves<0>(moves, nBB, allowed);
    //     moves = gen_all_knight_moves<1>(moves, nBB, allowed);
    //     moves = gen_all_knight_moves<2>(moves, nBB, allowed);
    //     moves = gen_all_knight_moves<3>(moves, nBB, allowed);

    //     // gen all king moves
    //     BB kBB(pos.occ<King, color>());
    //     moves = gen_all_king_moves<0>(moves, kBB, allowed);
    //     moves = gen_all_king_moves<1>(moves, kBB, allowed);
    //     moves = gen_all_king_moves<2>(moves, kBB, allowed);
    //     moves = gen_all_king_moves<3>(moves, kBB, allowed);

    //     if constexpr (flag == Noisy)
    //         moves = gen_all_take_moves<color>(pos, moves);

    //     if constexpr (flag == Quiet)
    //         moves = gen_all_push_moves<color>(pos, moves);

    //     return moves - start;
    // }

    // template <MoveFlag flag>
    // std::size_t gen_all_pseudo_legal_moves(const Position &pos, Move *moves)
    // {
    //     const GameState &gs = pos.state();

    //     switch (gs.turn)
    //     {
    //     case Red:
    //         return gen_moves<flag, Red>(pos, moves);
    //     case Blue:
    //         return gen_moves<flag, Blue>(pos, moves);
    //     case Yellow:
    //         return gen_moves<flag, Yellow>(pos, moves);
    //     case Green:
    //         return gen_moves<flag, Green>(pos, moves);
    //     default:
    //         return 0;
    //     }
    // }

    // template std::size_t gen_all_pseudo_legal_moves<MoveFlag::Noisy>(const Position &, Move *);
    // template std::size_t gen_all_pseudo_legal_moves<MoveFlag::Quiet>(const Position &, Move *);



















// add concepts and constraints TChunk TGuild TColorTPlayer
// add draw conditions like insufficient material 
// add fiftymove
// always there must be four kings
// make compress as member function of SQ
// add assert for size of classes !!!





template<std::size_t chunk>
inline SQ popLSB(u64& lane)
{
    static_assert(chunk == 0 || chunk == 1 || 
                  chunk == 2 || chunk == 3, "popLSB: invalid chunk");

    auto sq = static_cast<SQ>(64 * chunk + __builtin_ctzll(lane));
    lane &= (lane - 1); 
    return sq;
}

template<std::size_t chunk>
inline auto build(Move* moves, SQ source, BB& targets, Type type, Flag flag)
{
    static_assert(chunk == 0 || chunk == 1 || 
                  chunk == 2 || chunk == 3, "build: invalid chunk");
    
    // static_assert(type == MoveType::Jumper || type == MoveType::Slider ||
    //               type == MoveType::Enpass || type == MoveType::Castle ||
    //               type == MoveType::Evolve, "build: invalid MoveType");

    auto lane = targets.chunks[chunk];
    while (lane)
        *(moves++) = Move(source, popLSB<chunk>(lane), type, flag);
    return moves;
}

template<std::size_t chunk>
inline auto build(Move* moves, BB& targets, int delta, Type type, Flag flag)
{
    static_assert(chunk == 0 || chunk == 1 || 
                  chunk == 2 || chunk == 3, "build: invalid chunk");

    // static_assert(type == MoveType::Pushed || type == MoveType::Strike ||
    //               type == MoveType::Stride, "build: invalid MoveType");

    auto& lane = targets.chunks[chunk];
    while (lane)
    {
        auto target = popLSB<chunk>(lane);
        auto source = static_cast<SQ>(target.encoded - delta);
        *(moves++) = Move(source, target, type, flag);
    }
    return moves;
}


inline auto build(Move* moves, SQ source, SQ target, Color color)
{
    *(moves++) = Move(source, target, PC(Piece::Q, color));
    *(moves++) = Move(source, target, PC(Piece::R, color));
    *(moves++) = Move(source, target, PC(Piece::B, color));
    *(moves++) = Move(source, target, PC(Piece::N, color));
    return moves;
}

template<std::size_t chunk, Color color>
inline auto genEvolveMoves(const PS& ps, Move* moves, BB& bb, const BB& opp, const BB& emp)
{    
    constexpr int pDELTA = 
    (color.encoded == Color::r) ? +16 :
    (color.encoded == Color::b) ? +1  :
    (color.encoded == Color::y) ? -16 :
    (color.encoded == Color::g) ? -1  : 0;

    constexpr int eDELTA = 
    (color.encoded == Color::r) ? +17 :
    (color.encoded == Color::b) ? -15 :
    (color.encoded == Color::y) ? -17 :
    (color.encoded == Color::g) ? +15 : 0;
    
    constexpr int wDELTA = 
    (color.encoded == Color::r) ? +15 :
    (color.encoded == Color::b) ? +17 :
    (color.encoded == Color::y) ? -15 :
    (color.encoded == Color::g) ? -17 : 0;

    auto& lane = bb.chunks[chunk];
    while (lane)
    {
        const auto sq = popLSB<chunk>(lane);

        const auto pSQ = static_cast<SQ>(sq.encoded + pDELTA);
        const auto eSQ = static_cast<SQ>(sq.encoded + eDELTA);
        const auto wSQ = static_cast<SQ>(sq.encoded + wDELTA);

        if (opp.checkSQ(eSQ)) moves = build(moves, sq, eSQ, color);
        if (opp.checkSQ(wSQ)) moves = build(moves, sq, wSQ, color);
        if (emp.checkSQ(pSQ)) moves = build(moves, sq, pSQ, color); 
    }

    return moves;
}

template<Color color>
inline auto genEvolveMoves(const PS& ps, Move* moves, BB& bb, const BB& opp, const BB& emp)
{
    if (!bb.empty())
    {
        moves = genEvolveMoves<0, color>(ps, moves, bb, opp, emp);
        moves = genEvolveMoves<1, color>(ps, moves, bb, opp, emp);
        moves = genEvolveMoves<2, color>(ps, moves, bb, opp, emp);
        moves = genEvolveMoves<3, color>(ps, moves, bb, opp, emp);
    }
    return moves;
}




template<Color color>
inline auto genEnpassMoves(Move* moves, const PS& ps, const GS& gs)
{
    // static_assert(color == Red    || color == Blue || 
    //               color == Yellow || color == Green, "gen_enpass_moves: invalid Player enum value.");

    constexpr auto opp1 = Color(color.encoded & 1 ? Color::r : Color::b);
    constexpr auto opp2 = Color(color.encoded & 1 ? Color::y : Color::g);

    constexpr auto pc = PC(Piece::P, color);

    // constexpr auto SHIFT_EAST = PAWN_DELTA[ally(color)][2];
    // constexpr auto SHIFT_WEST = PAWN_DELTA[ally(color)][3]; 

    constexpr int eDELTA = 
    (color.encoded == Color::r) ? -17 :
    (color.encoded == Color::b) ? +15 :
    (color.encoded == Color::y) ? +17 :
    (color.encoded == Color::g) ? -15 : 0;
    
    constexpr int wDELTA = 
    (color.encoded == Color::r) ? -15 :
    (color.encoded == Color::b) ? -17 :
    (color.encoded == Color::y) ? +15 : 
    (color.encoded == Color::g) ? +17 : 0;
     
    const auto ep1 = gs.enpass[opp1];
    const auto ep2 = gs.enpass[opp2];

    if (ep1.encoded != SQ::OFFBOARD)
    {
        const auto s1 = static_cast<SQ>(ep1.encoded + eDELTA);
        const auto s2 = static_cast<SQ>(ep1.encoded + wDELTA);

        if (ps.mailbox[s1.encoded].encoded == pc.encoded) *(moves++) = Move(s1, ep1, opp1);
        if (ps.mailbox[s2.encoded].encoded == pc.encoded) *(moves++) = Move(s2, ep1, opp1);
    }

    if (ep2.encoded != SQ::OFFBOARD)
    {
        const auto s1 = static_cast<SQ>(ep2.encoded + eDELTA); 
        const auto s2 = static_cast<SQ>(ep2.encoded + wDELTA);

        if (ps.mailbox[s1.encoded].encoded == pc.encoded) *(moves++) = Move(s1, ep2, opp2);
        if (ps.mailbox[s2.encoded].encoded == pc.encoded) *(moves++) = Move(s2, ep2, opp2);
    }

    return moves;
}


// template<std::size_t chunk, Piece piece>
// inline BB& genSlideAttacks(SQ sq, const BB& all, BB& attacks)
// {
//     static_assert(chunk == 0 || chunk == 1 || 
//                   chunk == 2 || chunk == 3, "genSlideAttacks: invalid chunk");

//     static_assert(piece.encoded == Piece::R || piece.encoded == Piece::B,
//                  "genSlideAttacks: invalid piece");

//     if constexpr (piece.encoded == Piece::R)
//     {
//         const auto& [verti, horiz, lower, upper, padding] = STRAIGHT[compress<chunk>(sq).encoded];
//         attacks.hyperbola_quintessence<chunk>(sq, all, verti);
//         attacks.obstruction_difference<chunk>(sq, all, horiz, lower, upper);
//     } 

//     if constexpr (piece.encoded == Piece::B)
//     {
//         const auto& [diag, anti] = DIAGONAL[compress<chunk>(sq).encoded];
//         attacks.hyperbola_quintessence<chunk>(sq, all, diag);
//         attacks.hyperbola_quintessence<chunk>(sq, all, anti);
//     }

//     return attacks;
// }

template<std::size_t chunk, Piece piece>
auto genSliderMoves(Move* moves, BB& bb, const BB& all, const BB& msk, Flag flag)
{   
    auto& lane = bb.chunks[chunk];
    while (lane)
    {
        const auto sq = popLSB<chunk>(lane);

        BB attacks;
        genSlideAttacks<chunk, piece>(sq, all, attacks);
        attacks &= msk;

        moves = build<0>(moves, sq, attacks, Type::Slider, flag);
        moves = build<1>(moves, sq, attacks, Type::Slider, flag);
        moves = build<2>(moves, sq, attacks, Type::Slider, flag);
        moves = build<3>(moves, sq, attacks, Type::Slider, flag);
    }

    return moves;
}

template<std::size_t chunk, Piece piece>
auto genJumperMoves(Move* moves, BB& bb, const BB& msk, Flag flag)
{
    auto& lane = bb.chunks[chunk];
    while (lane)
    {
        const auto sq = popLSB<chunk>(lane);
        const auto& [king, knight] = ADJACENT[compress<chunk>(sq).encoded];

        BB attacks(piece.encoded == Piece::N ? knight : king);
        attacks &= msk;

        moves = build<0>(moves, sq, attacks, Type::Jumper, flag);
        moves = build<1>(moves, sq, attacks, Type::Jumper, flag);
        moves = build<2>(moves, sq, attacks, Type::Jumper, flag);
        moves = build<3>(moves, sq, attacks, Type::Jumper, flag);
    }

    return moves;
}

template<Color color>
inline auto genPushMoves(const PS& ps, Move* moves, const BB& bb, const BB& emp, const BB& homerank)
{
    static_assert(color.encoded == Color::r || color.encoded == Color::b ||
                  color.encoded == Color::y || color.encoded == Color::g, "genPushMoves: invalid color");

    constexpr int deltaP = 
    (color.encoded == Color::r) ? +16 :
    (color.encoded == Color::b) ? +1  :
    (color.encoded == Color::y) ? -16 :
    (color.encoded == Color::g) ? -1  : 0;

    constexpr int deltaS = 
    (color.encoded == Color::r) ? +32 :
    (color.encoded == Color::b) ? +2  :
    (color.encoded == Color::y) ? -32 :
    (color.encoded == Color::g) ? -2  : 0;

    BB pBB(bb);
    pBB.shift<deltaP>();
    pBB &= emp;

    BB sBB(pBB);
    sBB &= homerank;
    sBB.shift<deltaP>();
    sBB &= emp;

    moves = build<0>(moves, pBB, deltaP, Type::Pushed, Quiet);
    moves = build<1>(moves, pBB, deltaP, Type::Pushed, Quiet);
    moves = build<2>(moves, pBB, deltaP, Type::Pushed, Quiet);
    moves = build<3>(moves, pBB, deltaP, Type::Pushed, Quiet);

    moves = build<0>(moves, sBB, deltaS, Type::Stride, Quiet);
    moves = build<1>(moves, sBB, deltaS, Type::Stride, Quiet);
    moves = build<2>(moves, sBB, deltaS, Type::Stride, Quiet);
    moves = build<3>(moves, sBB, deltaS, Type::Stride, Quiet);

    return moves;
}

template<Color color>
inline auto genTakeMoves(const PS& ps, Move* moves, const BB& bb, const BB& opp)
{
    static_assert(color.encoded == Color::r || color.encoded == Color::b ||
                  color.encoded == Color::y || color.encoded == Color::g, "genTakeMoves: invalid color");

    constexpr int deltaE = 
    (color.encoded == Color::r) ? +17 :
    (color.encoded == Color::b) ? -15 :
    (color.encoded == Color::y) ? -17 :
    (color.encoded == Color::g) ? +15 : 0;
    
    constexpr int deltaW = 
    (color.encoded == Color::r) ? +15 :
    (color.encoded == Color::b) ? +17 :
    (color.encoded == Color::y) ? -15 :
    (color.encoded == Color::g) ? -17 : 0;

    BB eBB(bb);
    BB wBB(bb); 

    eBB.shift<deltaE>();
    wBB.shift<deltaW>(); 
    
    eBB &= opp;
    wBB &= opp;

    moves = build<0>(moves, eBB, deltaE, Type::Strike, Noisy);
    moves = build<1>(moves, eBB, deltaE, Type::Strike, Noisy);
    moves = build<2>(moves, eBB, deltaE, Type::Strike, Noisy);
    moves = build<3>(moves, eBB, deltaE, Type::Strike, Noisy);

    moves = build<0>(moves, wBB, deltaW, Type::Strike, Noisy);
    moves = build<1>(moves, wBB, deltaW, Type::Strike, Noisy);
    moves = build<2>(moves, wBB, deltaW, Type::Strike, Noisy);
    moves = build<3>(moves, wBB, deltaW, Type::Strike, Noisy);

    return moves;
}






// template <Side side, GameSetup setup, Player player>
// inline auto gen_castle_moves(const Position &pos, Move *moves)
// {
//     // if (castle.checkSQ(SOURCE_CASTLE[setup][player][side]))
//     // {
//         BB pass(PASS[setup][player][side]);
//         pass &= pos.occ<All, RBYG>();

//         constexpr auto safe0 = SAFE[setup][player][side][0];
//         constexpr auto safe1 = SAFE[setup][player][side][1];

//         if (pass.empty()
//             && !isSquareAttacked<player, chunkSQ(safe0)>(pos, safe0) // reduce size with guild where poss
//             && !isSquareAttacked<player, chunkSQ(safe1)>(pos, safe1))
//             *(moves++) = Move(SOURCE_ROYAL[setup][player], TARGET_ROYAL[setup][player][side], side);
//     // }

//     return moves;
// }

// template <GameSetup setup, Player player>
// inline auto gen_all_castle_moves(const Position &pos, Move *moves)
// {
//     const auto &castle = pos.state().castle;

//     if (castle.checkSQ(SOURCE_ROYAL[setup][player]))
//     {
//         if (castle.checkSQ(SOURCE_CASTLE[setup][player][KingSide] )) moves = gen_castle_moves<KingSide , setup, player>(pos, moves);
//         if (castle.checkSQ(SOURCE_CASTLE[setup][player][QueenSide])) moves = gen_castle_moves<QueenSide, setup, player>(pos, moves);
//         // // KingSide Right
//         // if (castle.checkSQ(SOURCE_CASTLE[setup][player][KingSide]))
//         // {
//         //     BB pass(PASS[setup][player][KingSide]);
//         //     pass &= pos.occ<All, RBYG>();

//         //     constexpr auto safe0 = SAFE[setup][player][KingSide][0];
//         //     constexpr auto safe1 = SAFE[setup][player][KingSide][1];
            
//         //     if (pass.empty()
//         //         && !isSquareAttacked<player, CHUNK_ROYAL[setup][player][KingSide][0]>(pos, SAFE[setup][player][KingSide][0])
//         //         && !isSquareAttacked<player, CHUNK_ROYAL[setup][player][KingSide][1]>(pos, SAFE[setup][player][KingSide][1]))
//         //         *(moves++) = Move(SOURCE_ROYAL[setup][player], TARGET_ROYAL[setup][player][KingSide], KingSide);;
//         // }

//         // // QueenSide Right
//         // if (castle.checkSQ(SOURCE_CASTLE[setup][player][QueenSide]))
//         // {
//         //     BB pass(PASS[setup][player][QueenSide]);
//         //     pass &= pos.occ<All, RBYG>();

//         //     constexpr auto safe0 = SAFE[setup][player][QueenSide][0];
//         //     constexpr auto safe1 = SAFE[setup][player][QueenSide][1];

//         //     if (pass.empty()
//         //         && !isSquareAttacked<player, chunkSQ(safe0)>(pos, safe0)
//         //         && !isSquareAttacked<player, chunkSQ(safe1)>(pos, safe1))
//         //         *(moves++) = Move(SOURCE_ROYAL[setup][player], TARGET_ROYAL[setup][player][QueenSide], QueenSide);
//         // }
//     }

//     return moves;
// }


template<Color color, Side side, Setup setup>
inline auto genCastleMoves(Move* moves, const PS& ps, const BB& all)
{
    constexpr auto& RI  = RIGHTS_INFO[side + color * 2 + setup * 8];
    constexpr auto& CI  = CASTLE_INFO[side + color * 2 + setup * 8];
    constexpr auto move = Move(CI.king_source, CI.king_target, side);

    if (!RI.pass.collision(all)
    &&  !ps.isSquareAttacked<color.enem()>(RI.safe1)
    &&  !ps.isSquareAttacked<color.enem()>(RI.safe2)) *(moves++) = move;

    return moves;
}

template<Color color>
inline auto genCastleMoves(Move* moves, const PS& ps, const GS& gs, const BB& all, Setup setup)
{
    if (gs.rights(color))
    {
        const auto ks = gs.rights(color, KS);
        const auto qs = gs.rights(color, QS);

        if (ks) switch (setup)
        {
        case Modern : moves = genCastleMoves<color, KS, Modern >(moves, ps, all); break;
        case Classic: moves = genCastleMoves<color, KS, Classic>(moves, ps, all); break;
        default: break;
        }

        if (qs) switch (setup)
        {
        case Modern : moves = genCastleMoves<color, QS, Modern >(moves, ps, all); break;
        case Classic: moves = genCastleMoves<color, QS, Classic>(moves, ps, all); break;
        default: break;
        }
    }
    return moves;
}








template<MoveFlag flag, Color color>
auto genMoves(const PS& ps, const GS& gs, Move* moves, Setup setup)
{
    // static_assert(flag == MoveFlag::Noisy || flag == MoveFlag::Quiet, "genMoves: invalid MoveFlag");

    // add static assert
    const auto& opp = ps.bitboards[(color.encoded & 1) ? PC::ORANGE : PC::CYAN]; // fix constexpr
    const auto& all = ps.bitboards[PC::EVERYTHING];
    const auto  emp = ~all; // fix
    const auto& msk = (flag == Noisy) ? opp : emp;

    constexpr auto nIDX = PC(Piece::N, color).encoded;
    constexpr auto kIDX = PC(Piece::K, color).encoded;

    const auto& nBB = ps.bitboards[nIDX];
    const auto& kBB = ps.bitboards[kIDX];

    BB nSRC(nBB);
    moves = genJumperMoves<0, Piece::N>(moves, nSRC, msk, flag);
    moves = genJumperMoves<1, Piece::N>(moves, nSRC, msk, flag);
    moves = genJumperMoves<2, Piece::N>(moves, nSRC, msk, flag);
    moves = genJumperMoves<3, Piece::N>(moves, nSRC, msk, flag);

    BB kSRC(kBB);
    moves = genJumperMoves<0, Piece::K>(moves, kSRC, msk, flag);
    moves = genJumperMoves<1, Piece::K>(moves, kSRC, msk, flag);
    moves = genJumperMoves<2, Piece::K>(moves, kSRC, msk, flag);
    moves = genJumperMoves<3, Piece::K>(moves, kSRC, msk, flag);

    constexpr auto pIDX = PC(Piece::P, color).encoded;
    constexpr auto qIDX = PC(Piece::Q, color).encoded;
    constexpr auto bIDX = PC(Piece::B, color).encoded;
    constexpr auto rIDX = PC(Piece::R, color).encoded;

    const auto& pBB = ps.bitboards[pIDX];
    const auto& qBB = ps.bitboards[qIDX];
    const auto& bBB = ps.bitboards[bIDX];
    const auto& rBB = ps.bitboards[rIDX];

    BB rSRC(rBB, qBB);
    moves = genSliderMoves<0, Piece::R>(moves, rSRC, all, msk, flag);
    moves = genSliderMoves<1, Piece::R>(moves, rSRC, all, msk, flag);
    moves = genSliderMoves<2, Piece::R>(moves, rSRC, all, msk, flag);
    moves = genSliderMoves<3, Piece::R>(moves, rSRC, all, msk, flag);

    BB bSRC(bBB, qBB);
    moves = genSliderMoves<0, Piece::B>(moves, bSRC, all, msk, flag);
    moves = genSliderMoves<1, Piece::B>(moves, bSRC, all, msk, flag);
    moves = genSliderMoves<2, Piece::B>(moves, bSRC, all, msk, flag);
    moves = genSliderMoves<3, Piece::B>(moves, bSRC, all, msk, flag);

    constexpr auto& PI = PAWN_INFO[color.encoded];
    constexpr auto& promotes = PI.promotes;
    constexpr auto& homerank = PI.homerank;
    constexpr auto  nonpromo = ~promotes;

    BB pSRC(pBB); 
    BB eSRC(pBB);
    pSRC &= nonpromo;
    eSRC &= promotes;

    if constexpr (flag == Noisy) moves = genTakeMoves<color>(ps, moves, pSRC, opp);
    if constexpr (flag == Quiet) moves = genPushMoves<color>(ps, moves, pSRC, emp, homerank);
    
    // Special moves
    if constexpr (flag == Noisy) moves = genEvolveMoves<color>(ps, moves, eSRC, opp, emp);
    if constexpr (flag == Noisy) moves = genEnpassMoves<color>(moves, ps, gs);
    if constexpr (flag == Quiet) moves = genCastleMoves<color>(moves, ps, gs, all, setup);

    return moves;
}

template<MoveFlag flag>
auto genAllMoves(const PS& ps, Move* moves, Setup setup, u8 depth)
{
    const GS& gs = ps.states[depth];
    switch (gs.turn.encoded)
    {
    case Color::r: return genMoves<flag, Color::r>(ps, gs, moves, setup);
    case Color::b: return genMoves<flag, Color::b>(ps, gs, moves, setup);
    case Color::y: return genMoves<flag, Color::y>(ps, gs, moves, setup);
    case Color::g: return genMoves<flag, Color::g>(ps, gs, moves, setup);
    default: return moves;
    }
}

void MoveList::genNoisyMoves(const PS& ps, Setup setup, u8 depth) { size += (genAllMoves<Noisy>(ps, (moves + size), setup, depth) - (moves + size)); }
void MoveList::genQuietMoves(const PS& ps, Setup setup, u8 depth) { size += (genAllMoves<Quiet>(ps, (moves + size), setup, depth) - (moves + size)); }

} // namespace athena