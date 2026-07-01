#include "movegen.h"
#include "attacks.h"
#include "bitboard.h"
#include "castle.h"
#include "chess/constants.h"
#include "move.h"
#include "piece.h"
#include "piececolor.h"
#include "position.h"

namespace athena::chess {

Move* add_evolve_moves(const Position& pos, Move* moves, Square source, Square target, Color::ID color, Color::ID enpass = Color::ID::None) noexcept {
    
    auto occ = pos.occupied();
    occ.pop_bit(source);
    if (enpass != Color::ID::None) occ.pop_bit(source + Square::push(color, 0));
    if (pos.get_attackers_bitboard(target, pos.turn().id(), occ).any())
        return moves;

    *(moves++) = Move(source.id(), target.id(), Piece::ID::Queen , enpass, Castle::Side(0b10), Move::Policy::Evolve);
    *(moves++) = Move(source.id(), target.id(), Piece::ID::Rook  , enpass, Castle::Side(0b10), Move::Policy::Evolve);
    *(moves++) = Move(source.id(), target.id(), Piece::ID::Bishop, enpass, Castle::Side(0b10), Move::Policy::Evolve);
    *(moves++) = Move(source.id(), target.id(), Piece::ID::Knight, enpass, Castle::Side(0b10), Move::Policy::Evolve); 
    return moves;
}

template<Color::ID color>
Move* generate_evolve_moves(const Position& pos, Move* moves, Bitboard& pawn) noexcept {

    if (pawn.empty()) return moves;

    for (Bitboard::ChunkID chunk_id = 0; chunk_id < CHUNK_NB; chunk_id++) {

        auto& chunk = pawn.chunk(chunk_id);
        while(chunk) {
            const auto source = Bitboard::pop_lsb(chunk, chunk_id);
            const auto east = (source + Square::take(color, 0)).id();
            const auto west = (source + Square::take(color, 1)).id();
            const auto push = (source + Square::push(color, 0)).id();

            if (pos.board(push) == PieceColor::empty()) { moves = add_evolve_moves(pos, moves, source, push, color); }    
            if (pos.board(east).color().diff(color)   ) { moves = add_evolve_moves(pos, moves, source, east, color); }
            if (pos.board(west).color().diff(color)   ) { moves = add_evolve_moves(pos, moves, source, west, color); }
        }
    }

    return moves;
}

template<Color::ID color, Color::ID enemy, Square::Offset offset>
Move* generate_enpass_moves(const Position& pos, Move* moves, Square target) noexcept {

    const auto source = target - offset; 
    if (pos.board(source.id()) != PieceColor(color, Piece::ID::Pawn)) 
        return moves;

    if constexpr (
        color == Color::ID::Red || 
        color == Color::ID::Yellow)
        if (source.rank() + 1 == PROMOTES)
            return add_evolve_moves(pos, moves, source, target, enemy);

    if constexpr (
        color == Color::ID::Blue ||
        color == Color::ID::Green )
        if (source.file() + 1 == PROMOTES)
            return add_evolve_moves(pos, moves, source, target, enemy);

    auto occ = pos.occupied();
    occ.set_bit(target);
    occ.pop_bit(source);
    occ.pop_bit(source + Square::push(color, 0));
    if (pos.get_attackers_bitboard(pos.royal(color), color, occ).any())
        return moves;
        
    *(moves++) = Move(source.id(), target.id(), Piece::ID::Empty, enemy, Castle::Side(0b10), Move::Policy::Enpass);
    
    return moves;
}

template<Color::ID color, Color::ID enemy>
Move* generate_enpass_moves(const Position& pos, Move* moves) noexcept {

    const auto target = pos.enpass(enemy); 
    if (target == Square::offboard()) 
        return moves;

    moves = generate_enpass_moves<color, enemy, Square::take(color, 0)>(pos, moves, target);
    moves = generate_enpass_moves<color, enemy, Square::take(color, 1)>(pos, moves, target);

    return moves;
}

template<Color::ID color>
Move* generate_enpass_moves(const Position& pos, Move* moves) noexcept {
    moves = generate_enpass_moves<color, Color(color).next().id()>(pos, moves);
    moves = generate_enpass_moves<color, Color(color).prev().id()>(pos, moves);
    return moves;
}

template<Color::ID color, Castle::Setup setup, Castle::Side side>
Move* generate_castle_moves(const Position& pos, Move* moves) noexcept {

    if (pos.state().castle.has(Castle(color, side)))
        return moves;

    if (
    pos.board(Castle::path_square(setup, color, side, 0)) == PieceColor::empty() &&
    pos.board(Castle::path_square(setup, color, side, 1)) == PieceColor::empty() &&
    pos.board(Castle::path_square(setup, color, side, 2)) == PieceColor::empty()) 
        return moves;

    if ((
        pos.get_attackers_bitboard(Castle::safe_square(setup, color, side, 0), color, pos.occupied()) |
        pos.get_attackers_bitboard(Castle::safe_square(setup, color, side, 1), color, pos.occupied()) |
        pos.get_attackers_bitboard(Castle::safe_square(setup, color, side, 2), color, pos.occupied())).any()
    )
        return moves;

    *(moves++) = Move(Castle::king_source(setup, color, side), Castle::king_target(setup, color, side), Piece::ID::Empty, Color::ID::None, side, Move::Policy::Castle);
    
    return moves;
}

template<Castle::Setup setup, Color::ID color>
Move* generate_castle_moves(const Position& pos, Move* moves) noexcept {
    moves = generate_castle_moves<color, setup, Castle::Side::KingSide >(pos, moves);
    moves = generate_castle_moves<color, setup, Castle::Side::QueenSide>(pos, moves);
    return moves;
}

template<Color::ID color>
Move* generate_castle_moves(const Position& pos, Move* moves) noexcept {
    switch (pos.setup()) {
        case Castle::Setup::Modern : return generate_castle_moves<Castle::Setup::Modern , color>(pos, moves);
        case Castle::Setup::Classic: return generate_castle_moves<Castle::Setup::Classic, color>(pos, moves);
        default: return moves;
    }
}

template<Bitboard::ChunkID chunk_id>
Move* add_royal_moves(const Position& pos, Move* moves, Square source, Bitboard& targets) noexcept {
    auto& chunk = targets.chunk(chunk_id);
    while(chunk) {
        const auto target = Bitboard::pop_lsb(chunk, chunk_id);

       
        auto occ = pos.occupied();
        occ.pop_bit(source);
        if (pos.get_attackers_bitboard(target, pos.turn().id(), occ).any())
            continue;
        

        *(moves++) = Move(source.id(), target.id(), Piece::ID::Empty, Color::ID::None, Castle::Side(0b10), Move::Policy::Normal);
    }
    return moves;
}

Move* add_royal_moves(const Position& pos, Move* moves, Square source, Bitboard& targets) noexcept {
    moves = add_royal_moves<Bitboard::ChunkID(0)>(pos, moves, source, targets);
    moves = add_royal_moves<Bitboard::ChunkID(1)>(pos, moves, source, targets);
    moves = add_royal_moves<Bitboard::ChunkID(2)>(pos, moves, source, targets);
    moves = add_royal_moves<Bitboard::ChunkID(3)>(pos, moves, source, targets);
    return moves;
}

Move* generate_royal_moves(const Position& pos, Move* moves, Square source, const Bitboard& mask) noexcept {
    auto attacks = get_crawl_attacks<Piece::ID::King>(source);
    attacks &= mask;
    moves = add_royal_moves(pos, moves, source, attacks);
    return moves;
}

template<Bitboard::ChunkID chunk_id>
Move* add_normal_moves(Move* moves, Square source, Bitboard& targets) noexcept {
    auto& chunk = targets.chunk(chunk_id);
    while(chunk) {
        const auto target = Bitboard::pop_lsb(chunk, chunk_id);

        // if constexpr (pin) {
        //     auto occ = pos.occupied();
        //     occ.pop_bit(source);
        //     if (pos.get_attackers_bitboard(target, pos.turn().id(), occ).any())
        //         continue;
        // } 

        *(moves++) = Move(source.id(), target.id(), Piece::ID::Empty, Color::ID::None, Castle::Side(0b10), Move::Policy::Normal);
    }
    return moves;
}

Move* add_normal_moves(Move* moves, Square source, Bitboard& targets) noexcept {
    moves = add_normal_moves<Bitboard::ChunkID(0)>(moves, source, targets);
    moves = add_normal_moves<Bitboard::ChunkID(1)>(moves, source, targets);
    moves = add_normal_moves<Bitboard::ChunkID(2)>(moves, source, targets);
    moves = add_normal_moves<Bitboard::ChunkID(3)>(moves, source, targets);
    return moves;
}

template<Piece::ID piece, Bitboard::ChunkID chunk_id>
Move* generate_crawl_moves(Move* moves, Bitboard& sources, const Bitboard& mask) noexcept {
    auto& chunk = sources.chunk(chunk_id);
    while(chunk) {
        const auto source = Bitboard::pop_lsb(chunk, chunk_id);
        auto attacks = get_crawl_attacks<piece>(source);
        attacks &= mask;
        moves = add_normal_moves(moves, source, attacks);
    }
    return moves;
}

template<Piece::ID piece>
Move* generate_crawl_moves(Move* moves, Bitboard& sources, const Bitboard& mask) noexcept {
    moves = generate_crawl_moves<piece, Bitboard::ChunkID(0)>(moves, sources, mask);
    moves = generate_crawl_moves<piece, Bitboard::ChunkID(1)>(moves, sources, mask);
    moves = generate_crawl_moves<piece, Bitboard::ChunkID(2)>(moves, sources, mask);
    moves = generate_crawl_moves<piece, Bitboard::ChunkID(3)>(moves, sources, mask);
    return moves;
}

template<Piece::ID piece, Bitboard::ChunkID chunk_id>
Move* generate_slide_moves(Move* moves, Bitboard& sources, const Bitboard& mask, const Bitboard& occupied) noexcept {
    auto& chunk = sources.chunk(chunk_id);
    while(chunk) {
        const auto source = Bitboard::pop_lsb(chunk, chunk_id);
        auto attacks = get_slide_attacks<piece>(source, occupied);
        attacks &= mask;
        moves = add_normal_moves(moves, source, attacks);         
    }
    return moves;
}

template<Piece::ID piece>
Move* generate_slide_moves(Move* moves, Bitboard& sources, const Bitboard& mask, const Bitboard& occupied) noexcept {
    moves = generate_slide_moves<piece, Bitboard::ChunkID(0)>(moves, sources, mask, occupied);
    moves = generate_slide_moves<piece, Bitboard::ChunkID(1)>(moves, sources, mask, occupied);
    moves = generate_slide_moves<piece, Bitboard::ChunkID(2)>(moves, sources, mask, occupied);
    moves = generate_slide_moves<piece, Bitboard::ChunkID(3)>(moves, sources, mask, occupied);
    return moves;
}

template<Bitboard::ChunkID chunk_id>
Move* add_pawn_moves(Move* moves, Bitboard& targets, Square::Offset offset, Move::Policy policy) noexcept {
    auto& chunk = targets.chunk(chunk_id);
    while(chunk) {
        const auto target = Bitboard::pop_lsb(chunk, chunk_id);
        const auto source = target - offset;

        // if constexpr (pin) {
        //     auto occ = pos.occupied();
        //     occ.pop_bit(source);
        //     if (pos.get_attackers_bitboard(target, pos.turn().id(), occ).any())
        //         continue;
        // } 

        *(moves++) = Move(source.id(), target.id(), Piece::ID::Empty, Color::ID::None, Castle::Side(0b10), policy);
    }
    return moves;
}

Move* add_pawn_moves(Move* moves, Bitboard& targets, Square::Offset offset, Move::Policy policy) noexcept {
    moves = add_pawn_moves<Bitboard::ChunkID(0)>(moves, targets, offset, policy);
    moves = add_pawn_moves<Bitboard::ChunkID(1)>(moves, targets, offset, policy);
    moves = add_pawn_moves<Bitboard::ChunkID(2)>(moves, targets, offset, policy);
    moves = add_pawn_moves<Bitboard::ChunkID(3)>(moves, targets, offset, policy);
    return moves;
}

template<Color::ID color>
Move* generate_push_moves(Move* moves, Bitboard& pawn, const Bitboard& mask) noexcept {

    pawn.shift<Square::push(color, 0)>();
    auto push = pawn & mask & Bitboard::homerank(color);
    push.shift<Square::push(color, 0)>();

    pawn &= mask;
    push &= mask;

    moves = add_pawn_moves(moves, pawn, Square::push(color, 0), Move::Policy::Normal);
    moves = add_pawn_moves(moves, push, Square::push(color, 1), Move::Policy::Stride);     

    return moves;
}

template<Color::ID color>
Move* generate_take_moves(Move* moves, const Bitboard& pawn, const Bitboard& mask) noexcept {

    auto east = pawn;
    auto west = pawn;

    east.shift<Square::take(color, 0)>();
    west.shift<Square::take(color, 1)>();

    east &= mask;
    west &= mask;

    moves = add_pawn_moves(moves, east, Square::take(color, 0), Move::Policy::Normal);
    moves = add_pawn_moves(moves, west, Square::take(color, 1), Move::Policy::Normal);

    return moves;
}

template<Bitboard::ChunkID chunk_id>
Move* add_pin_moves(const Position& pos, Move* moves, Bitboard& targets, Square::Offset offset, Move::Policy policy) noexcept {
    auto& chunk = targets.chunk(chunk_id);
    while(chunk) {
        const auto target = Bitboard::pop_lsb(chunk, chunk_id);
        const auto source = target - offset;

        auto occ = pos.occupied();
        occ.pop_bit(source);
        if (pos.get_attackers_bitboard(target, pos.turn().id(), occ).any())
            continue;
    
        *(moves++) = Move(source.id(), target.id(), Piece::ID::Empty, Color::ID::None, Castle::Side(0b10), policy);
    }
    return moves;
}

Move* add_pin_moves(const Position& pos, Move* moves, Bitboard& targets, Square::Offset offset, Move::Policy policy) noexcept {
    moves = add_pin_moves<Bitboard::ChunkID(0)>(pos, moves, targets, offset, policy);
    moves = add_pin_moves<Bitboard::ChunkID(1)>(pos, moves, targets, offset, policy);
    moves = add_pin_moves<Bitboard::ChunkID(2)>(pos, moves, targets, offset, policy);
    moves = add_pin_moves<Bitboard::ChunkID(3)>(pos, moves, targets, offset, policy);
    return moves;
}

template<Bitboard::ChunkID chunk_id>
Move* add_pin_moves(const Position& pos, Move* moves, Square source, Bitboard& targets) noexcept {
    auto& chunk = targets.chunk(chunk_id);
    while(chunk) {
        const auto target = Bitboard::pop_lsb(chunk, chunk_id);

        auto occ = pos.occupied();
        occ.pop_bit(source);
        if (pos.get_attackers_bitboard(target, pos.turn().id(), occ).any())
                continue;

        *(moves++) = Move(source.id(), target.id(), Piece::ID::Empty, Color::ID::None, Castle::Side(0b10), Move::Policy::Normal);
    }
    return moves;
}

Move* add_pin_moves(const Position& pos, Move* moves, Square source, Bitboard& targets) noexcept {
    moves = add_pin_moves<Bitboard::ChunkID(0)>(pos, moves, source, targets);
    moves = add_pin_moves<Bitboard::ChunkID(1)>(pos, moves, source, targets);
    moves = add_pin_moves<Bitboard::ChunkID(2)>(pos, moves, source, targets);
    moves = add_pin_moves<Bitboard::ChunkID(3)>(pos, moves, source, targets);
    return moves;
}

template<Move::Flag flag, Color::ID color, Bitboard::ChunkID chunk_id>
Move* generate_pin_moves(const Position& pos, Move* moves, Bitboard& sources, const Bitboard& mask, const Bitboard& checks) noexcept {
    auto& chunk = sources.chunk(chunk_id);
    while(chunk) {
        const auto source = Bitboard::pop_lsb(chunk, chunk_id);
        const auto piece = pos.board(source.id()).piece().id();
        switch (piece) {
            case Piece::ID::Bishop:
            {
                auto attacks = get_slide_attacks<Piece::ID::Bishop>(source, pos.occupied()); 
                attacks &= mask;
                moves = add_pin_moves(pos, moves, source, attacks);  
                break;
            }
            case Piece::ID::Rook:
            {
                auto attacks = get_slide_attacks<Piece::ID::Rook>(source, pos.occupied()); 
                attacks &= mask;
                moves = add_pin_moves(pos, moves, source, attacks);  
                break;
            }
            case Piece::ID::Queen:
            {
                auto attacks = 
                get_slide_attacks<Piece::ID::Bishop>(source, pos.occupied()) |
                get_slide_attacks<Piece::ID::Rook  >(source, pos.occupied()); 
                attacks &= mask;
                moves = add_pin_moves(pos, moves, source, attacks);  
                break;
            }
            case Piece::ID::Knight:
            {
                auto attacks = get_crawl_attacks<Piece::ID::Knight>(source); 
                attacks &= mask;
                moves = add_pin_moves(pos, moves, source, attacks);  
                break;
            }
            case Piece::ID::Pawn:
            {
                Bitboard pawn = pos.bitboard(Piece::ID::Pawn) & pos.bitboard(color);
                Bitboard push = pawn;
                Bitboard east = pawn;
                Bitboard west = pawn;

                pawn.shift<Square::push(color, 0)>();
                push.shift<Square::push(color, 0)>();
                east.shift<Square::take(color, 0)>();
                west.shift<Square::take(color, 1)>();

                pawn &= Bitboard::homerank(color);
                pawn &= flag == Move::Flag::Legal ? ~pos.occupied() & checks : mask;
                push &= flag == Move::Flag::Legal ? ~pos.occupied() & checks : mask;
                east &= flag == Move::Flag::Legal ?  pos.opponent() & checks : mask;
                west &= flag == Move::Flag::Legal ?  pos.opponent() & checks : mask;

                pawn.shift<Square::push(color, 0)>();
                pawn &= flag == Move::Flag::Legal ? ~pos.occupied() & checks : mask;

                if constexpr (flag != Move::Flag::Noisy) moves = add_pin_moves(pos, moves, pawn, Square::push(color, 0), Move::Policy::Normal);
                if constexpr (flag != Move::Flag::Noisy) moves = add_pin_moves(pos, moves, push, Square::push(color, 1), Move::Policy::Stride);
                if constexpr (flag != Move::Flag::Quiet) moves = add_pin_moves(pos, moves, east, Square::take(color, 0), Move::Policy::Normal);
                if constexpr (flag != Move::Flag::Quiet) moves = add_pin_moves(pos, moves, west, Square::take(color, 1), Move::Policy::Normal);
                
                break;
            }
            default: break;
        }
    }
    return moves;
}

template<Move::Flag flag, Color::ID color>
Move* generate_pin_moves(const Position& pos, Move* moves, const Bitboard& pinned, const Bitboard& mask, const Bitboard& checks) noexcept {
    if (pinned.any()) return moves;
    Bitboard sources = pinned;
    moves = generate_pin_moves<flag, color, Bitboard::ChunkID(0)>(pos, moves, sources, mask, checks);
    moves = generate_pin_moves<flag, color, Bitboard::ChunkID(1)>(pos, moves, sources, mask, checks);
    moves = generate_pin_moves<flag, color, Bitboard::ChunkID(2)>(pos, moves, sources, mask, checks);
    moves = generate_pin_moves<flag, color, Bitboard::ChunkID(3)>(pos, moves, sources, mask, checks);
    return moves;
}

template<Move::Flag flag, Color::ID color>
int generate_all_moves(const Position& pos, Move* moves) noexcept {
    Move* start = moves;

    const Bitboard teammate = pos.bitboard(Color(color).self().id()) | pos.bitboard(Color(color).ally().id());
    const Bitboard opponent = pos.bitboard(Color(color).next().id()) | pos.bitboard(Color(color).prev().id());
    const Bitboard occupied = teammate | opponent;
    const auto& [pinned, checks] = pos.get_pinned_checks_bitboards();
    const Bitboard us = pos.bitboard(color) & ~pinned;
    
    Bitboard mask;
    if constexpr (flag == Move::Flag::Noisy) { mask =  opponent; }
    if constexpr (flag == Move::Flag::Quiet) { mask = ~occupied; }
    if constexpr (flag == Move::Flag::Legal) { mask = ~teammate; }

    // Generate king moves
    auto ksq = pos.royal(color);
    moves = generate_royal_moves(pos, moves, ksq, mask);
    mask &= checks;

    // Generate knight moves
    constexpr auto n = Piece::ID::Knight;
    auto nBB = pos.bitboard(n);
    nBB &= us;
    moves = generate_crawl_moves<n>(moves, nBB, mask); 

    // Generate bishop, rook, queen moves
    constexpr auto b = Piece::ID::Bishop;
    constexpr auto r = Piece::ID::Rook;
    constexpr auto q = Piece::ID::Queen;

    auto bBB = pos.bitboard(b);
    auto rBB = pos.bitboard(r);
    auto qBB = pos.bitboard(q);

    bBB |= qBB; bBB &= us;
    rBB |= qBB; rBB &= us;
    
    moves = generate_slide_moves<b>(moves, bBB, mask, occupied);
    moves = generate_slide_moves<r>(moves, rBB, mask, occupied);
    
    // Generate pawn moves
    constexpr auto p = Piece::ID::Pawn; 
    constexpr auto pmask = ~Bitboard::promote(color);
    constexpr auto emask =  Bitboard::promote(color);
    auto pBB = pos.bitboard(p) & us;
    auto eBB = pBB;
    pBB &= pmask;
    eBB &= emask;

    if constexpr (flag != Move::Flag::Quiet) 
        moves = generate_take_moves<color>(moves, pBB, flag == Move::Flag::Legal ?  opponent & checks : mask); 
    if constexpr (flag != Move::Flag::Noisy) 
        moves = generate_push_moves<color>(moves, pBB, flag == Move::Flag::Legal ? ~occupied & checks : mask);

    // Generate special moves
    if constexpr (flag != Move::Flag::Quiet) 
        moves = generate_evolve_moves<color>(pos, moves, eBB);
    if constexpr (flag != Move::Flag::Quiet) 
        moves = generate_enpass_moves<color>(pos, moves);
    if constexpr (flag != Move::Flag::Noisy) 
        moves = generate_castle_moves<color>(pos, moves);

    return generate_pin_moves<flag, color>(pos, moves, pinned, mask, checks) - start;
}

[[nodiscard]] int generate_noisy_moves(Position& pos, Move* moves) noexcept {
    pos.set_pinned_checks_bitboards();
    switch (pos.turn().id()) {
        case Color::ID::Red   : return generate_all_moves<Move::Flag::Noisy, Color::ID::Red   >(pos, moves);
        case Color::ID::Blue  : return generate_all_moves<Move::Flag::Noisy, Color::ID::Blue  >(pos, moves);
        case Color::ID::Yellow: return generate_all_moves<Move::Flag::Noisy, Color::ID::Yellow>(pos, moves);
        case Color::ID::Green : return generate_all_moves<Move::Flag::Noisy, Color::ID::Green >(pos, moves);
        default: return 0;
    }
}

[[nodiscard]] int generate_quiet_moves(Position& pos, Move* moves) noexcept {
    switch (pos.turn().id()) {
        case Color::ID::Red   : return generate_all_moves<Move::Flag::Quiet, Color::ID::Red   >(pos, moves);
        case Color::ID::Blue  : return generate_all_moves<Move::Flag::Quiet, Color::ID::Blue  >(pos, moves);
        case Color::ID::Yellow: return generate_all_moves<Move::Flag::Quiet, Color::ID::Yellow>(pos, moves);
        case Color::ID::Green : return generate_all_moves<Move::Flag::Quiet, Color::ID::Green >(pos, moves);
        default: return 0;
    }
}

[[nodiscard]] int generate_legal_moves(Position& pos, Move* moves) noexcept {
    pos.set_pinned_checks_bitboards();
    switch (pos.turn().id()) {
        case Color::ID::Red   : return generate_all_moves<Move::Flag::Legal, Color::ID::Red   >(pos, moves);
        case Color::ID::Blue  : return generate_all_moves<Move::Flag::Legal, Color::ID::Blue  >(pos, moves);
        case Color::ID::Yellow: return generate_all_moves<Move::Flag::Legal, Color::ID::Yellow>(pos, moves);
        case Color::ID::Green : return generate_all_moves<Move::Flag::Legal, Color::ID::Green >(pos, moves);
        default : return 0;
    }
}

} // namespace athena