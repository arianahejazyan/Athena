#include "movegen.h"
#include "attacks.h"
#include "bitboard.h"
#include "castle.h"
#include "chess.h"
#include "constants.h"
#include "move.h"
#include "offset.h"
#include "piece.h"
#include "piececlass.h"
#include "position.h"
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace athena
{

template<std::size_t chunk_id>
Move* add_moves(Move* moves, const Square source, Bitboard& targets, const MoveFlag flag, const MoveType type) noexcept
{
    auto& chunk = targets.chunk<chunk_id>();
    while(chunk)
    {
        const auto target = targets.pop_lsb<chunk_id>();
        *(moves++) = Move(source, target, flag, type);
    }
    
    return moves;
}

template<std::size_t chunk_id>
Move* add_moves(Move* moves, const Offset offset, Bitboard& targets, const MoveFlag flag, const MoveType type) noexcept
{
    auto& chunk = targets.chunk<chunk_id>();
    while(chunk)
    {
        const auto target = targets.pop_lsb<chunk_id>();
        const auto source = target - offset;
        *(moves++) = Move(source, target, flag, type);
    }
    
    return moves;
}

Move* generate_king_moves(const Position& pos, Move* moves, const Square sq, const Bitboard& mask, const MoveFlag flag) noexcept
{
    auto [targets, _] = crawl_attacks(sq);
    targets &= mask;

    moves = add_moves<CHUNK_0>(moves, sq, targets, flag, MoveType::Crawl);
    moves = add_moves<CHUNK_1>(moves, sq, targets, flag, MoveType::Crawl);
    moves = add_moves<CHUNK_2>(moves, sq, targets, flag, MoveType::Crawl);
    moves = add_moves<CHUNK_3>(moves, sq, targets, flag, MoveType::Crawl);

    return moves;
}

template<std::size_t chunk_id>
Move* generate_knight_moves(const Position& pos, Move* moves, Bitboard& sources, const Bitboard& mask, const MoveFlag flag) noexcept
{
    auto& chunk = sources.chunk<chunk_id>();
    while(chunk)
    {
        const auto source = sources.pop_lsb<chunk_id>();
        auto [_, targets] = crawl_attacks(source);
        targets &= mask;

        moves = add_moves<CHUNK_0>(moves, source, targets, flag, MoveType::Crawl);
        moves = add_moves<CHUNK_1>(moves, source, targets, flag, MoveType::Crawl);
        moves = add_moves<CHUNK_2>(moves, source, targets, flag, MoveType::Crawl);
        moves = add_moves<CHUNK_3>(moves, source, targets, flag, MoveType::Crawl);       
    }

    return moves;
}

template<std::size_t chunk_id>
Move* generate_bishop_moves(const Position& pos, Move* moves, Bitboard& sources, const Bitboard& mask, const Bitboard& occupancy, const MoveFlag flag) noexcept
{
    auto& chunk = sources.chunk<chunk_id>();
    while(chunk)
    {
        const auto source = sources.pop_lsb<chunk_id>();
        auto targets = bishop_attacks(source, occupancy);
        targets &= mask;

        moves = add_moves<CHUNK_0>(moves, source, targets, flag, MoveType::Slide);
        moves = add_moves<CHUNK_1>(moves, source, targets, flag, MoveType::Slide);
        moves = add_moves<CHUNK_2>(moves, source, targets, flag, MoveType::Slide);
        moves = add_moves<CHUNK_3>(moves, source, targets, flag, MoveType::Slide);        
    }

    return moves;
}

template<std::size_t chunk_id>
Move* generate_rook_moves(const Position& pos, Move* moves, Bitboard& sources, const Bitboard& mask, const Bitboard& occupancy, const MoveFlag flag) noexcept
{
    auto& chunk = sources.chunk<chunk_id>();
    while(chunk)
    {
        const auto source = sources.pop_lsb<chunk_id>();
        auto targets = rook_attacks(source, occupancy); // std::cout << "square: " << source.uci(true) << std::endl; occupancy.print(); targets.print(); mask.print();
        targets &= mask;

        moves = add_moves<CHUNK_0>(moves, source, targets, flag, MoveType::Slide);
        moves = add_moves<CHUNK_1>(moves, source, targets, flag, MoveType::Slide);
        moves = add_moves<CHUNK_2>(moves, source, targets, flag, MoveType::Slide);
        moves = add_moves<CHUNK_3>(moves, source, targets, flag, MoveType::Slide);        
    }

    return moves;
}

template<Color color>
Move* generate_take_moves(const Position& pos, Move* moves, Bitboard& pawn, const Bitboard& mask)
{
    constexpr auto east_offset = take_offsets(color)[0];
    constexpr auto west_offset = take_offsets(color)[1];

    auto& east = pawn;
    auto  west = pawn;

    east.shift<east_offset>();
    west.shift<west_offset>();

    east &= mask;
    west &= mask;

    moves = add_moves<CHUNK_0>(moves, east_offset, east, MoveFlag::Noisy, MoveType::Take);
    moves = add_moves<CHUNK_1>(moves, east_offset, east, MoveFlag::Noisy, MoveType::Take);
    moves = add_moves<CHUNK_2>(moves, east_offset, east, MoveFlag::Noisy, MoveType::Take);
    moves = add_moves<CHUNK_3>(moves, east_offset, east, MoveFlag::Noisy, MoveType::Take);     
    moves = add_moves<CHUNK_0>(moves, west_offset, west, MoveFlag::Noisy, MoveType::Take);
    moves = add_moves<CHUNK_1>(moves, west_offset, west, MoveFlag::Noisy, MoveType::Take);
    moves = add_moves<CHUNK_2>(moves, west_offset, west, MoveFlag::Noisy, MoveType::Take);
    moves = add_moves<CHUNK_3>(moves, west_offset, west, MoveFlag::Noisy, MoveType::Take);     

    return moves;
}

template<Color color>
Move* generate_push_moves(const Position& pos, Move* moves, Bitboard& pawn, const Bitboard& mask, const Bitboard& occupancy)
{
    constexpr auto single_push_offset = push_offsets(color)[0];
    constexpr auto double_push_offset = push_offsets(color)[1];

    pawn.shift<single_push_offset>();
    auto push = pawn & ~occupancy & homerank_bitboard(color);
    push.shift<single_push_offset>();
    pawn &= mask;
    push &= mask;

    moves = add_moves<CHUNK_0>(moves, single_push_offset, pawn, MoveFlag::Quiet, MoveType::Push  );
    moves = add_moves<CHUNK_1>(moves, single_push_offset, pawn, MoveFlag::Quiet, MoveType::Push  );
    moves = add_moves<CHUNK_2>(moves, single_push_offset, pawn, MoveFlag::Quiet, MoveType::Push  );
    moves = add_moves<CHUNK_3>(moves, single_push_offset, pawn, MoveFlag::Quiet, MoveType::Push  );     
    moves = add_moves<CHUNK_0>(moves, double_push_offset, push, MoveFlag::Quiet, MoveType::Stride);
    moves = add_moves<CHUNK_1>(moves, double_push_offset, push, MoveFlag::Quiet, MoveType::Stride);
    moves = add_moves<CHUNK_2>(moves, double_push_offset, push, MoveFlag::Quiet, MoveType::Stride);
    moves = add_moves<CHUNK_3>(moves, double_push_offset, push, MoveFlag::Quiet, MoveType::Stride);     

    return moves;
}

Move* add_moves(Move* moves, const Square source, const Square target) noexcept
{
    *(moves++) = Move(source, target, Piece(Piece::Queen ));
    *(moves++) = Move(source, target, Piece(Piece::Rook  ));
    *(moves++) = Move(source, target, Piece(Piece::Bishop));
    *(moves++) = Move(source, target, Piece(Piece::Knight)); return moves;
    // if (promotion)
    // {
    //     *(moves++) = Move(source, target, Piece(Piece::Queen ));
    //     *(moves++) = Move(source, target, Piece(Piece::Rook  ));
    //     *(moves++) = Move(source, target, Piece(Piece::Bishop));
    //     *(moves++) = Move(source, target, Piece(Piece::Knight));        
    // }

    // else *(moves++) = Move(source, target, MoveFlag::Noisy, MoveType::Enpass); return moves; // problem
}

template<std::size_t chunk_id, Color color>
Move* generate_evolve_moves(const Position& pos, Move* moves, Bitboard& pawn) noexcept
{
    constexpr auto east_offset = take_offsets(color)[0];
    constexpr auto west_offset = take_offsets(color)[1];
    constexpr auto push_offset = push_offsets(color)[0];

    auto& chunk = pawn.chunk<chunk_id>();
    while(chunk)
    {
        const auto source = pawn.pop_lsb<chunk_id>();
        const auto east = source + east_offset;
        const auto west = source + west_offset;
        const auto push = source + push_offset;

        const bool b1 = pos.board(east).piece() != Piece::Empty && pos.board(east).color().team() != color.team();
        const bool b2 = pos.board(west).piece() != Piece::Empty && pos.board(west).color().team() != color.team();
        const bool b3 = pos.board(push).piece() == Piece::Empty;

        if (b1) moves = add_moves(moves, source, east);
        if (b2) moves = add_moves(moves, source, west);
        if (b3) moves = add_moves(moves, source, push);
    }

    return moves;
}

template<Color color>
Move* generate_evolve_moves(const Position& pos, Move* moves, Bitboard& pawn) noexcept
{
    if (!pawn.empty())
    {
        moves = generate_evolve_moves<CHUNK_0, color>(pos, moves, pawn);
        moves = generate_evolve_moves<CHUNK_1, color>(pos, moves, pawn);
        moves = generate_evolve_moves<CHUNK_2, color>(pos, moves, pawn);
        moves = generate_evolve_moves<CHUNK_3, color>(pos, moves, pawn);
    }

    return moves;
}

template<Color color, Color opponent>
Move* generate_enpass_moves(const Position& pos, Move* moves, const Bitboard& pawn) noexcept
{
    constexpr auto offsets = take_offsets(color.ally());
    const auto target = pos.enpass(opponent);

    if (!target.offboard())
    {
        for (auto offset: offsets)
        {
            const auto source = target + offset;
            if (pos.board(source) == PieceClass(color, Piece::Pawn))
            {
                if (pawn.has(source)) 
                {
                    moves = add_moves(moves, source, target);
                    continue;
                }

                *(moves++) = Move(source, target, opponent);
            }
        }
    }

    return moves;
}

template<Color color>
Move* generate_enpass_moves(const Position& pos, Move* moves, const Bitboard& pawn) noexcept
{
    moves = generate_enpass_moves<color, color.next()>(pos, moves, pawn);
    moves = generate_enpass_moves<color, color.prev()>(pos, moves, pawn);
    return moves;
}

template<Color color, Side side>
Move* generate_castle_moves(const Position& pos, Move* moves, GameSetup setup) noexcept
{
    const auto squares = castle_squares(color, side, setup);
    const auto& gs = pos.state();

    if (gs.castle.has(color, side))
    {
        constexpr auto path = path_squares(color, side);
        if (
        pos.board(path[0]) == PieceClass::Empty() &&
        pos.board(path[1]) == PieceClass::Empty() &&
        pos.board(path[2]) == PieceClass::Empty()
        )

        *(moves++) = Move(squares.king_source, squares.king_target, side);
    }
    
    return moves;
}

template<Color color>
Move* generate_castle_moves(const Position& pos, Move* moves, GameSetup setup) noexcept
{
    moves = generate_castle_moves<color, Side::KingSide >(pos, moves, setup);
    moves = generate_castle_moves<color, Side::QueenSide>(pos, moves, setup);
    return moves;
}

Square pop_lsb(uint64_t& chunk, std::size_t chunk_id) noexcept
{
    int b = __builtin_ctzll(chunk);
    chunk &= chunk - 1;
    return Square(chunk_id * CHUNK_SIZE + b);
}

template<MoveFlag flag, Color color>
std::size_t generate_moves(const Position& pos, Move* moves, GameSetup setup) noexcept
{
    Move* start = moves;
    // const auto& [checkmask, pinnedmask] = pos.check_pinned_masks_[pos.play_];

    const Bitboard teammate = pos.bitboard(color       ) | pos.bitboard(color.ally());
    const Bitboard opponent = pos.bitboard(color.next()) | pos.bitboard(color.prev());
    const Bitboard occupancy = teammate | opponent;

    // // Compute occupancy mask
    // const auto occupancy = 
    // pos.bitboard(Team(Team::RY)) |
    // pos.bitboard(Team(Team::BG));

    // Compute filter mask
    Bitboard mask;
    if constexpr (flag == MoveFlag::Noisy) mask = opponent;
    if constexpr (flag == MoveFlag::Quiet) mask = ~occupancy;

    // Generate King moves
    moves = generate_king_moves(pos, moves, pos.royal(color), mask, flag);

    // // Handle checks
    // mask &= checkmask;

    const Bitboard us = pos.bitboard(color);
    // 
    auto knights = pos.bitboard(Piece(Piece::Knight));
    knights &= us;
    moves = generate_knight_moves<CHUNK_0>(pos, moves, knights, mask, flag);
    moves = generate_knight_moves<CHUNK_1>(pos, moves, knights, mask, flag);
    moves = generate_knight_moves<CHUNK_2>(pos, moves, knights, mask, flag);
    moves = generate_knight_moves<CHUNK_3>(pos, moves, knights, mask, flag);

    //
    auto bishops = pos.bitboard(Piece(Piece::Bishop));
    auto rooks   = pos.bitboard(Piece(Piece::Rook  ));
    auto queens  = pos.bitboard(Piece(Piece::Queen ));

    //
    bishops |= queens;
    bishops &= us;
    moves = generate_bishop_moves<CHUNK_0>(pos, moves, bishops, mask, occupancy, flag);
    moves = generate_bishop_moves<CHUNK_1>(pos, moves, bishops, mask, occupancy, flag);
    moves = generate_bishop_moves<CHUNK_2>(pos, moves, bishops, mask, occupancy, flag);
    moves = generate_bishop_moves<CHUNK_3>(pos, moves, bishops, mask, occupancy, flag);

    //
    rooks |= queens;
    rooks &= us;
    moves = generate_rook_moves<CHUNK_0>(pos, moves, rooks, mask, occupancy, flag);
    moves = generate_rook_moves<CHUNK_1>(pos, moves, rooks, mask, occupancy, flag);
    moves = generate_rook_moves<CHUNK_2>(pos, moves, rooks, mask, occupancy, flag);
    moves = generate_rook_moves<CHUNK_3>(pos, moves, rooks, mask, occupancy, flag);

    // 
    auto pawns = pos.bitboard(Piece(Piece::Pawn));
    pawns &= us;
    auto nonevolve = (pawns & ~promotion_bitboard(color));
    auto    evolve = (pawns &  promotion_bitboard(color));

    // Generate Pawn moves
    if constexpr (flag == MoveFlag::Noisy) moves = generate_take_moves<color>(pos, moves, nonevolve, mask);
    if constexpr (flag == MoveFlag::Quiet) moves = generate_push_moves<color>(pos, moves, nonevolve, mask, occupancy);

    // Generate special moves
    if constexpr (flag == MoveFlag::Noisy) moves = generate_enpass_moves<color>(pos, moves, evolve);
    if constexpr (flag == MoveFlag::Noisy) moves = generate_evolve_moves<color>(pos, moves, evolve);
    if constexpr (flag == MoveFlag::Quiet) moves = generate_castle_moves<color>(pos, moves, setup );

    // Set pinned pieces moves as pseudo
    // if (!pinnedmask.empty())
    // {
    //     for (std::size_t chunk_id = 0; chunk_id < CHUNK_NB; chunk_id++)
    //     {
    //         auto chunk = pinnedmask.chunk(chunk_id);
    //         while (chunk)
    //         {
    //             const auto sq = pop_lsb(chunk, chunk_id);
    //             for (Move* move = start; move != moves; move++) {
    //                 move->setpseudo(sq);
    //             }
    //         }
    //     }
    // }

    return moves - start;
}

std::size_t generate_noisy_moves(const Position& pos, Move* moves, GameSetup setup) noexcept
{
    const auto color = pos.turn();
    switch (color.value_) 
    {
        case Color::Red   : return generate_moves<MoveFlag::Noisy, Color::Red   >(pos, moves, setup);
        case Color::Blue  : return generate_moves<MoveFlag::Noisy, Color::Blue  >(pos, moves, setup);
        case Color::Yellow: return generate_moves<MoveFlag::Noisy, Color::Yellow>(pos, moves, setup);
        case Color::Green : return generate_moves<MoveFlag::Noisy, Color::Green >(pos, moves, setup);
        default: return 0;
    }
}

std::size_t generate_quiet_moves(const Position& pos, Move* moves, GameSetup setup) noexcept
{
    const auto color = pos.turn();
    switch (color.value_) 
    {
        case Color::Red   : return generate_moves<MoveFlag::Quiet, Color::Red   >(pos, moves, setup);
        case Color::Blue  : return generate_moves<MoveFlag::Quiet, Color::Blue  >(pos, moves, setup);
        case Color::Yellow: return generate_moves<MoveFlag::Quiet, Color::Yellow>(pos, moves, setup);
        case Color::Green : return generate_moves<MoveFlag::Quiet, Color::Green >(pos, moves, setup);
        default: return 0;
    }
}

} // namespace athena