#ifndef MOVE_H
#define MOVE_H

#include "position.h"
#include "utility.h"

namespace athena
{

// constexpr std::size_t MOVES_NB = 256;

// enum MoveFlag : u8
// {
//     Noisy = 0,
//     Quiet = 1,
// };

// enum MoveType : u8
// {
//     Jumper = 0,
//     Slider = 1,
//     Pushed = 2,
//     Strike = 3,
//     Stride = 4,
//     Enpass = 5,
//     Castle = 6,
//     Evolve = 7,
// };

// using Flag = MoveFlag;
// using Type = MoveType;

// class Move
// {
//     private:
//     uint32_t encoded;

//     public:
//     inline constexpr Move() {}

//     inline constexpr Move(SQ source, SQ target, MoveType type)
//         : encoded(source.encoded ^ (target.encoded << 24) ^ (type   << 8)) {}

//     inline constexpr Move(SQ source, SQ target, Color color)
//         : encoded(source.encoded ^ (target.encoded << 24) ^ (Enpass << 8) ^ (color.encoded << 12)) {}

//     inline constexpr Move(SQ source, SQ target, Piece piece)
//         : encoded(source.encoded ^ (target.encoded << 24) ^ (Evolve << 8) ^ (piece.encoded << 15)) {}

//     inline consteval Move(SQ source, SQ target, Side side)
//         : encoded(source.encoded ^ (target.encoded << 24) ^ (Castle << 8) ^ (side << 18)) {}   

//     inline constexpr auto source() const {
//         return static_cast<SQ>(encoded & 0xFF);
//     }

//     inline constexpr auto target() const {
//         return static_cast<SQ>(encoded >> 24);
//     }

//     inline constexpr auto enpass() const {
//         return static_cast<Color>((encoded >> 12) & 0x7);
//     }

//     inline constexpr auto evolve() const {
//         return static_cast<Piece>((encoded >> 15) & 0x7);
//     }

//     inline constexpr auto castle() const {
//         return static_cast<Side>((encoded >> 23) & 0x1);
//     }

//     inline constexpr auto type() const {
//         return static_cast<Type>((encoded >> 8 ) & 0x7);
//     }

//     inline constexpr auto flag() const {
//         return static_cast<Flag>((encoded >> 11) & 0x1);
//     }

//     const char* uci(bool debug = false) const;
// };

class alignas(CACHELINE_SIZE) MoveList
{
    public:
    Move moves[MOVES_NB];
    std::size_t size = 0;

    MoveList() {}

    Move& operator[](std::size_t idx) {
        return moves[idx];
    }

    void genNoisyMoves(const PS& ps, Setup setup = Modern, u8 depth = 0); 
    void genQuietMoves(const PS& ps, Setup setup = Modern, u8 depth = 0);

    void print(bool legal = false, bool debug = false) const;
};

//     template <Piece piece, std::size_t chunk>
//     inline BB &gen_slide_attacks(SQ sq, const BB &occ, BB &attacks)
//     {
//         static_assert(piece == Rook || piece == Bishop,
//                       "gen_slide_attacks: only Rook and Bishop are supported.");

//         if constexpr (piece == Rook)
//         {
//             hyperbola_quintessence<chunk>(sq, occ, VERTICAL_ATTACKS[sq], attacks);
//             obstruction_difference<chunk>(sq, occ, attacks);
//         }

//         if constexpr (piece == Bishop)
//         {
//             // const auto& [diag, anti] = DIAGONAL_BUNDLE[sq];

//             BB temp;
//             hyperbola_quintessence<chunk>(sq, occ, DIAGONAL_ATTACKS[sq], temp);
//             hyperbola_quintessence<chunk>(sq, occ, ANTIDIAGONAL_ATTACKS[sq], attacks);
//             attacks |= temp;
//         }

//         return attacks;
//     }

//     inline bool collision(const BB &bb1, const BB &bb2)
//     {
//         return (bb1.chunks[0] & bb2.chunks[0]) ||
//                (bb1.chunks[1] & bb2.chunks[1]) ||
//                (bb1.chunks[2] & bb2.chunks[2]) ||
//                (bb1.chunks[3] & bb2.chunks[3]);
//     }

//     inline bool collision(const BB &bb1, const BB &bb2, const BB& bb3)
//     {
//         return ((bb1.chunks[0] | bb2.chunks[0]) & bb3.chunks[0]) ||
//                ((bb1.chunks[1] | bb2.chunks[1]) & bb3.chunks[1]) ||
//                ((bb1.chunks[2] | bb2.chunks[2]) & bb3.chunks[2]) ||
//                ((bb1.chunks[3] | bb2.chunks[3]) & bb3.chunks[3]);
//     }

//     template <Player player, std::size_t chunk>
//     bool isSquareAttacked(const Position &pos, SQ sq)
//     {
//         BB bishop, rook;
//         constexpr auto opp1 = (player & 0b1) == Guild_RY ? Blue : Red;
//         constexpr auto opp2 = (player & 0b1) == Guild_RY ? Green : Yellow;

//         return

//             collision(pos.opp<Knight, player>(), KNIGHT_ATTACKS[sq]) ||

//             (collision(pos.opp<Queen, player>(), pos.opp<Bishop, player>(), BISHOP_ATTACKS[sq]) &&
//              collision(pos.opp<Queen, player>(), pos.opp<Bishop, player>(), gen_slide_attacks<Bishop, chunk>(sq, pos.occ<All, RBYG>(), bishop))) ||

//             (collision(pos.opp<Queen, player>(), pos.opp<Rook, player>(), ROOK_ATTACKS[sq]) &&
//              collision(pos.opp<Queen, player>(), pos.opp<Rook, player>(), gen_slide_attacks<Rook, chunk>(sq, pos.occ<All, RBYG>(), rook))) ||

//             collision(pos.occ<Pawn, opp1>(), PAWN_ATTACKS[opp2][sq]) ||
//             collision(pos.occ<Pawn, opp2>(), PAWN_ATTACKS[opp1][sq]) || // add early exit

//             (collision(pos.opp<King, player>(), KING_ATTACKS[sq]));
//     }


// template <std::size_t chunk>
// inline auto extract_moves(Move *moves, Square sq, BB &attacks, MoveNature nature, MoveFlag flag)
// {
//     auto &line = attacks.chunks[chunk];
//     while (line)
//     {
//         int bit = __builtin_ctzll(line);
//         line &= (line - 1);
//         *(moves++) = Move(sq, static_cast<Square>(64 * chunk + bit), nature, flag);
//     }
//     return moves;
// }


// template <MoveFlag flag, std::size_t chunk, Piece piece>
// auto gen_all_slider_moves(const Position &pos, Move *moves, BB &sliders, const BB &allowed)
// {   
//     auto &lane = sliders.chunks[chunk];
//     while (lane)
//     {
//         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
//         lane &= (lane - 1);

//         BB attacks;
//         gen_slide_attacks<piece, chunk>(sq, pos.occ<All, RBYG>(), attacks);

//         // hyperbola_quintessence(sq, pos.everyone(), VERTICAL_ATTACK[sq], attacks);
//         // obstruction_difference(sq, pos.everyone(), attacks);
//         attacks &= allowed;

//         moves = extract_moves<0>(moves, sq, attacks, Slider, flag);
//         moves = extract_moves<1>(moves, sq, attacks, Slider, flag);
//         moves = extract_moves<2>(moves, sq, attacks, Slider, flag);
//         moves = extract_moves<3>(moves, sq, attacks, Slider, flag);
//     }

//     return moves;
// }

// // template <std::size_t chunk>
// // auto gen_all_rook_moves(const Position &pos, Move *moves, BB &rooks, const BB &allowed)
// // {
// //     auto &lane = rooks.chunks[chunk];
// //     while (lane)
// //     {
// //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
// //         lane &= (lane - 1);

// //         BB attacks;
// //         gen_slide_attacks<Rook, chunk>(sq, pos.occ<All, RBYG>(), attacks);
// //         // hyperbola_quintessence(sq, pos.everyone(), VERTICAL_ATTACK[sq], attacks);
// //         // obstruction_difference(sq, pos.everyone(), attacks);
// //         attacks &= allowed;

// //         moves = extract_moves<0>(moves, sq, attacks);
// //         moves = extract_moves<1>(moves, sq, attacks);
// //         moves = extract_moves<2>(moves, sq, attacks);
// //         moves = extract_moves<3>(moves, sq, attacks);
// //     }

// //     return moves;
// // }

// // template <std::size_t chunk>
// // auto gen_all_bishop_moves(const Position &pos, Move *moves, BB &bishops, const BB &allowed)
// // {
// //     auto &lane = bishops.chunks[chunk];
// //     while (lane)
// //     {
// //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
// //         lane &= (lane - 1);

// //         // BB diagonal;
// //         BB attacks;
// //         gen_slide_attacks<Bishop, chunk>(sq, pos.occ<All, RBYG>(), attacks);
// //         // hyperbola_quintessence(sq, pos.everyone(), DIAGONAL_ATTACK[sq], diagonal);
// //         // hyperbola_quintessence(sq, pos.everyone(), ANTIDIAGONAL_ATTACK[sq], attacks);
// //         // attacks |= diagonal;
// //         attacks &= allowed;

// //         // std::cout << "CHUNK= " << (int)chunk << std::endl;
// //         // diagonal.print();

// //         moves = extract_moves<0>(moves, sq, attacks);
// //         moves = extract_moves<1>(moves, sq, attacks);
// //         moves = extract_moves<2>(moves, sq, attacks);
// //         moves = extract_moves<3>(moves, sq, attacks);
// //     }

// //     return moves;
// // }

// template <MoveFlag flag, std::size_t chunk, Piece piece>
// auto gen_all_jumper_moves(Move *moves, BB &jumpers, const BB &allowed)
// {
//     auto &lane = jumpers.chunks[chunk];
//     while (lane)
//     {
//         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
//         lane &= (lane - 1);

//         BB attacks(piece == Knight ? KNIGHT_ATTACKS[sq] : KING_ATTACKS[sq]);
//         attacks &= allowed;

//         moves = extract_moves<0>(moves, sq, attacks, Jumper, flag);
//         moves = extract_moves<1>(moves, sq, attacks, Jumper, flag);
//         moves = extract_moves<2>(moves, sq, attacks, Jumper, flag);
//         moves = extract_moves<3>(moves, sq, attacks, Jumper, flag);
//     }

//     return moves;
// }

// // template <std::size_t chunk>
// // auto gen_all_knight_moves(Move *moves, BB &knights, const BB &allowed)
// // {
// //     auto &lane = knights.chunks[chunk];
// //     while (lane)
// //     {
// //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
// //         lane &= (lane - 1);

// //         BB attacks(KNIGHT_ATTACKS[sq]);
// //         attacks &= allowed;

// //         moves = extract_moves<0>(moves, sq, attacks);
// //         moves = extract_moves<1>(moves, sq, attacks);
// //         moves = extract_moves<2>(moves, sq, attacks);
// //         moves = extract_moves<3>(moves, sq, attacks);
// //     }

// //     return moves;
// // }

// // template <std::size_t chunk>
// // auto gen_all_king_moves(Move *moves, BB &king, const BB &allowed)
// // {
// //     auto &lane = king.chunks[chunk];
// //     while (lane)
// //     {
// //         auto sq = static_cast<Square>(64 * chunk + __builtin_ctzll(lane));
// //         lane &= (lane - 1);

// //         BB attacks(KING_ATTACKS[sq]);
// //         attacks &= allowed;

// //         moves = extract_moves<0>(moves, sq, attacks);
// //         moves = extract_moves<1>(moves, sq, attacks);
// //         moves = extract_moves<2>(moves, sq, attacks);
// //         moves = extract_moves<3>(moves, sq, attacks);
// //     }

// //     return moves;
// // }

// template<std::size_t chunk>
// inline auto extract_moves(Move *moves, BB &attacks, int delta, MoveNature nature, MoveFlag flag)
// {
//     auto &line = attacks.chunks[chunk];
//     while (line)
//     {
//         auto target = static_cast<Square>(64 * chunk + __builtin_ctzll(line));
//         auto source = static_cast<Square>(target - delta);
//         line &= (line - 1);
//         *(moves++) = Move(source, target, nature, flag);
//     }
//     return moves;
// }

// template<Player player>
// inline auto gen_all_push_moves(const Position &pos, Move *moves)
// {
//     BB push(pos.occ<Pawn, player>());
//     push &= ~PROMOTE[player];
//     shift<player, 0>(push);
//     push &= pos.occ<Empty, None>();

//     BB advance(push);
//     advance &= HOMERANK[player];
//     shift<player, 0>(advance);
//     advance &= pos.occ<Empty, None>();

//     // generate all push moves
//     moves = extract_moves<0>(moves, push, PAWN_DELTA[player][0], Pushed, Quiet);
//     moves = extract_moves<1>(moves, push, PAWN_DELTA[player][0], Pushed, Quiet);
//     moves = extract_moves<2>(moves, push, PAWN_DELTA[player][0], Pushed, Quiet);
//     moves = extract_moves<3>(moves, push, PAWN_DELTA[player][0], Pushed, Quiet);

//     // generate all advance moves
//     moves = extract_moves<0>(moves, advance, PAWN_DELTA[player][1], Stride, Quiet);
//     moves = extract_moves<1>(moves, advance, PAWN_DELTA[player][1], Stride, Quiet);
//     moves = extract_moves<2>(moves, advance, PAWN_DELTA[player][1], Stride, Quiet);
//     moves = extract_moves<3>(moves, advance, PAWN_DELTA[player][1], Stride, Quiet);

//     return moves;
// }

// template <Player player>
// inline auto gen_all_take_moves(const Position &pos, Move *moves)
// {
//     BB takeE(pos.occ<Pawn, player>());
//     takeE &= ~PROMOTE[player];
//     BB takeW(takeE);
//     shift<player, 2>(takeE);
//     shift<player, 3>(takeW);
//     takeE &= pos.occ<All, opponent(player)>();
//     takeW &= pos.occ<All, opponent(player)>();

//     // generate all east take moves
//     moves = extract_moves<0>(moves, takeE, PAWN_DELTA[player][2], Strike, Noisy);
//     moves = extract_moves<1>(moves, takeE, PAWN_DELTA[player][2], Strike, Noisy);
//     moves = extract_moves<2>(moves, takeE, PAWN_DELTA[player][2], Strike, Noisy);
//     moves = extract_moves<3>(moves, takeE, PAWN_DELTA[player][2], Strike, Noisy);

//     // generate all west take moves
//     moves = extract_moves<0>(moves, takeW, PAWN_DELTA[player][3], Strike, Noisy);
//     moves = extract_moves<1>(moves, takeW, PAWN_DELTA[player][3], Strike, Noisy);
//     moves = extract_moves<2>(moves, takeW, PAWN_DELTA[player][3], Strike, Noisy);
//     moves = extract_moves<3>(moves, takeW, PAWN_DELTA[player][3], Strike, Noisy);

//     return moves;
// }

// template <Player player>
// inline auto gen_evolve_moves(const Position& ps, Move* moves, SQ source, SQ target)
// {
//     *(moves++) = Move(source, target, PC(Queen , player));
//     *(moves++) = Move(source, target, PC(Rook  , player));
//     *(moves++) = Move(source, target, PC(Bishop, player));
//     *(moves++) = Move(source, target, PC(Knight, player));
//     return moves;
// }

// template <Player player>
// inline auto gen_all_evolve_moves(const Position& pos, Move* moves)
// {
//     static_assert(player == Red    || player == Blue || 
//                   player == Yellow || player == Green, "gen_evolve_moves: invalid Player enum value.");

    

//     // early exit
//     if (collision(pos.occ<Pawn, player>(), PROMOTE[player]))
//     {
//         BB pawns(pos.occ<Pawn, player>());
//         pawns &= PROMOTE[player];

//         for (std::size_t chunk = 0; chunk < CHUNK_NB; ++chunk)
//         {
//             auto& lane = pawns.chunks[chunk];
//             while (lane)
//             {
//                 auto sq = static_cast<SQ>(64 * chunk + __builtin_ctzll(lane));
//                 lane &= (lane - 1);

//                 // 
//                 auto push  = static_cast<SQ>(sq + PAWN_DELTA[player][0]);
//                 auto takeE = static_cast<SQ>(sq + PAWN_DELTA[player][2]);
//                 auto takeW = static_cast<SQ>(sq + PAWN_DELTA[player][3]);

//                 if (pos.occ<All, opponent(player)>().checkSQ(takeE)) moves = gen_evolve_moves<player>(pos, moves, sq, takeE);
//                 if (pos.occ<All, opponent(player)>().checkSQ(takeW)) moves = gen_evolve_moves<player>(pos, moves, sq, takeW);
//                 if (pos.occ<Empty, None>().checkSQ(push))            moves = gen_evolve_moves<player>(pos, moves, sq, push );
//             }
//         }
//     }

//     return moves;
// }

// template<Player player>
// inline auto gen_all_enpass_moves(const Position& pos, Move* moves)
// {
//     static_assert(player == Red    || player == Blue || 
//                   player == Yellow || player == Green, "gen_enpass_moves: invalid Player enum value.");

//     const auto &gs = pos.state();

//     constexpr auto opp1 = (toGuild(player) == RY ? Blue  : Red   );
//     constexpr auto opp2 = (toGuild(player) == RY ? Green : Yellow);

//     constexpr auto SHIFT_EAST = PAWN_DELTA[ally(player)][2];
//     constexpr auto SHIFT_WEST = PAWN_DELTA[ally(player)][3]; 
    
//     const auto ep1 = gs.enpass[opp1];
//     if (ep1 != OFFBOARD)
//     {
//         const auto s1 = ep1 + SHIFT_EAST;
//         const auto s2 = ep1 + SHIFT_WEST;

//         if (pos.at(s1) == PC(Pawn, player)) *(moves++) = Move(s1, ep1, opp1);
//         if (pos.at(s2) == PC(Pawn, player)) *(moves++) = Move(s2, ep1, opp1);
//     }

//     const auto ep2 = gs.enpass[opp2];
//     if (ep2 != OFFBOARD)
//     {
//         const auto s1 = ep2 + SHIFT_EAST;
//         const auto s2 = ep2 + SHIFT_WEST;

//         if (pos.at(s1) == PC(Pawn, player)) *(moves++) = Move(s1, ep2, opp2);
//         if (pos.at(s2) == PC(Pawn, player)) *(moves++) = Move(s2, ep2, opp2);
//     }

//     return moves;
// }

//         template <Side side, GameSetup setup, Player player>
//         inline auto gen_castle_moves(const Position &pos, Move *moves)
//         {
//             // if (castle.checkSQ(SOURCE_CASTLE[setup][player][side]))
//             // {
//                 BB pass(PASS[setup][player][side]);
//                 pass &= pos.occ<All, RBYG>();

//                 constexpr auto safe0 = SAFE[setup][player][side][0];
//                 constexpr auto safe1 = SAFE[setup][player][side][1];

//                 if (pass.empty()
//                     && !isSquareAttacked<player, chunkSQ(safe0)>(pos, safe0) // reduce size with guild where poss
//                     && !isSquareAttacked<player, chunkSQ(safe1)>(pos, safe1))
//                     *(moves++) = Move(SOURCE_ROYAL[setup][player], TARGET_ROYAL[setup][player][side], side);
//             // }

//             return moves;
//         }

//         template <GameSetup setup, Player player>
//         inline auto gen_all_castle_moves(const Position &pos, Move *moves)
//         {
//             const auto &castle = pos.state().castle;

//             if (castle.checkSQ(SOURCE_ROYAL[setup][player]))
//             {
//                 if (castle.checkSQ(SOURCE_CASTLE[setup][player][KingSide] )) moves = gen_castle_moves<KingSide , setup, player>(pos, moves);
//                 if (castle.checkSQ(SOURCE_CASTLE[setup][player][QueenSide])) moves = gen_castle_moves<QueenSide, setup, player>(pos, moves);
//                 // // KingSide Right
//                 // if (castle.checkSQ(SOURCE_CASTLE[setup][player][KingSide]))
//                 // {
//                 //     BB pass(PASS[setup][player][KingSide]);
//                 //     pass &= pos.occ<All, RBYG>();

//                 //     constexpr auto safe0 = SAFE[setup][player][KingSide][0];
//                 //     constexpr auto safe1 = SAFE[setup][player][KingSide][1];
                    
//                 //     if (pass.empty()
//                 //         && !isSquareAttacked<player, CHUNK_ROYAL[setup][player][KingSide][0]>(pos, SAFE[setup][player][KingSide][0])
//                 //         && !isSquareAttacked<player, CHUNK_ROYAL[setup][player][KingSide][1]>(pos, SAFE[setup][player][KingSide][1]))
//                 //         *(moves++) = Move(SOURCE_ROYAL[setup][player], TARGET_ROYAL[setup][player][KingSide], KingSide);;
//                 // }

//                 // // QueenSide Right
//                 // if (castle.checkSQ(SOURCE_CASTLE[setup][player][QueenSide]))
//                 // {
//                 //     BB pass(PASS[setup][player][QueenSide]);
//                 //     pass &= pos.occ<All, RBYG>();

//                 //     constexpr auto safe0 = SAFE[setup][player][QueenSide][0];
//                 //     constexpr auto safe1 = SAFE[setup][player][QueenSide][1];

//                 //     if (pass.empty()
//                 //         && !isSquareAttacked<player, chunkSQ(safe0)>(pos, safe0)
//                 //         && !isSquareAttacked<player, chunkSQ(safe1)>(pos, safe1))
//                 //         *(moves++) = Move(SOURCE_ROYAL[setup][player], TARGET_ROYAL[setup][player][QueenSide], QueenSide);
//                 // }
//             }

//             return moves;
//         }

//     template <MoveFlag flag, Player player>
//     std::size_t gen_moves(const Position &pos, Move *moves)
//     {
//         const auto start = moves;

//         BitBoard allowed;
//         if constexpr (flag == Noisy) allowed = pos.occ<All, opponent(player)>();
//         if constexpr (flag == Quiet) allowed = pos.occ<Empty, None>();

//         // gen all rook moves
//         BB rBB(pos.occ<Rook, player>());
//         rBB |= pos.occ<Queen, player>();
//         moves = gen_all_slider_moves<flag, 0, Rook>(pos, moves, rBB, allowed);
//         moves = gen_all_slider_moves<flag, 1, Rook>(pos, moves, rBB, allowed);
//         moves = gen_all_slider_moves<flag, 2, Rook>(pos, moves, rBB, allowed);
//         moves = gen_all_slider_moves<flag, 3, Rook>(pos, moves, rBB, allowed);


//         // moves = gen_all_rook_moves<0>(pos, moves, rBB, allowed);
//         // moves = gen_all_rook_moves<1>(pos, moves, rBB, allowed);
//         // moves = gen_all_rook_moves<2>(pos, moves, rBB, allowed);
//         // moves = gen_all_rook_moves<3>(pos, moves, rBB, allowed);

//         // gen all bishp moves
//         BB bBB(pos.occ<Bishop, player>());
//         bBB |= pos.occ<Queen, player>();
//         moves = gen_all_slider_moves<flag, 0, Bishop>(pos, moves, bBB, allowed);
//         moves = gen_all_slider_moves<flag, 1, Bishop>(pos, moves, bBB, allowed);
//         moves = gen_all_slider_moves<flag, 2, Bishop>(pos, moves, bBB, allowed);
//         moves = gen_all_slider_moves<flag, 3, Bishop>(pos, moves, bBB, allowed);
        
//         // moves = gen_all_bishop_moves<0>(pos, moves, bBB, allowed);
//         // moves = gen_all_bishop_moves<1>(pos, moves, bBB, allowed);
//         // moves = gen_all_bishop_moves<2>(pos, moves, bBB, allowed);
//         // moves = gen_all_bishop_moves<3>(pos, moves, bBB, allowed);

//         // gen all knight moves
//         BB nBB(pos.occ<Knight, player>());
//         moves = gen_all_jumper_moves<flag, 0, Knight>(moves, nBB, allowed);
//         moves = gen_all_jumper_moves<flag, 1, Knight>(moves, nBB, allowed);
//         moves = gen_all_jumper_moves<flag, 2, Knight>(moves, nBB, allowed);
//         moves = gen_all_jumper_moves<flag, 3, Knight>(moves, nBB, allowed);

//         // moves = gen_all_knight_moves<0>(moves, nBB, allowed);
//         // moves = gen_all_knight_moves<1>(moves, nBB, allowed);
//         // moves = gen_all_knight_moves<2>(moves, nBB, allowed);
//         // moves = gen_all_knight_moves<3>(moves, nBB, allowed);

//         // gen all king moves
//         BB kBB(pos.occ<King, player>());
//         moves = gen_all_jumper_moves<flag, 0, King>(moves, kBB, allowed);
//         moves = gen_all_jumper_moves<flag, 1, King>(moves, kBB, allowed);
//         moves = gen_all_jumper_moves<flag, 2, King>(moves, kBB, allowed);
//         moves = gen_all_jumper_moves<flag, 3, King>(moves, kBB, allowed);

//         // moves = gen_all_king_moves<0>(moves, kBB, allowed);
//         // moves = gen_all_king_moves<1>(moves, kBB, allowed);
//         // moves = gen_all_king_moves<2>(moves, kBB, allowed);
//         // moves = gen_all_king_moves<3>(moves, kBB, allowed);

//         if constexpr (flag == Noisy) moves = gen_all_take_moves<player>(pos, moves);
//         if constexpr (flag == Quiet) moves = gen_all_push_moves<player>(pos, moves);

//         if constexpr (flag == Noisy) {
//             moves = gen_all_evolve_moves<player>(pos, moves);
//             moves = gen_all_enpass_moves<player>(pos, moves);
//         }
            
//         if constexpr (flag == Quiet)
//         {
//             if (pos.setup == Modern)
//             moves = gen_all_castle_moves<Modern , player>(pos, moves); else
//             moves = gen_all_castle_moves<Classic, player>(pos, moves);
//         }

//         return moves - start;
//     }

//     template <MoveFlag flag>
//     std::size_t gen_all_pseudo_legal_moves(const Position &pos, Move *moves)
//     {
//         const GameState &gs = pos.state();

//         switch (gs.turn)
//         {
//         case Red:
//             return gen_moves<flag, Red>(pos, moves);
//         case Blue:
//             return gen_moves<flag, Blue>(pos, moves);
//         case Yellow:
//             return gen_moves<flag, Yellow>(pos, moves);
//         case Green:
//             return gen_moves<flag, Green>(pos, moves);
//         default:
//             return 0;
//         }
//     }

//     template std::size_t gen_all_pseudo_legal_moves<MoveFlag::Noisy>(const Position &, Move *);
//     template std::size_t gen_all_pseudo_legal_moves<MoveFlag::Quiet>(const Position &, Move *);

// std::size_t genAllPseudoNoisyMoves(const Position& pos, Move* moves, uint8_t play);
// std::size_t genAllPseudoQueitMoves(const Position& pos, Move* moves, uint8_t play);





// std::size_t gen_all_pseudo_noisy_moves(const PS& ps, Move* moves, GameSetup setup, uint8_t ply); 
// std::size_t gen_all_pseudo_quiet_moves(const PS& ps, Move* moves, GameSetup setup, uint8_t ply);

} // namespace athena

#endif /* MOVE_H */