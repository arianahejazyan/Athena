#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "position.h"
#include "castle.h"
#include "chess/bitboard.h"
#include "chess/constants.h"
#include "chess/piececolor.h"
#include "color.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include "position.h"
#include "attacks.h"

namespace athena::chess {

void Position::set_board(Square sq, PieceColor pc) noexcept {
    board_[static_cast<uint8_t>(sq.id())] = pc;
    piece_[static_cast<uint8_t>(pc.piece().id())].set_bit(sq);
    color_[static_cast<uint8_t>(pc.color().id())].set_bit(sq);
}

void Position::pop_board(Square sq, PieceColor pc) noexcept {
    board_[static_cast<uint8_t>(sq.id())] = PieceColor::empty();
    piece_[static_cast<uint8_t>(pc.piece().id())].pop_bit(sq);
    color_[static_cast<uint8_t>(pc.color().id())].pop_bit(sq);
}

void Position::make_move(Move move) noexcept {

    auto& curr_state = state_[play_ + 0];
    auto& next_state = state_[play_ + 1];

    const auto source = move.source();
    const auto target = move.target();
    const auto policy = move.policy();

    const auto attacker = board(source);
    const auto defender = board(target);

    next_state.capture = defender;
    next_state.castle = curr_state.castle;

    if (defender != PieceColor::empty()) {
        pop_board(target, defender);

        if (defender.piece() == Piece::ID::Rook) {  
            constexpr auto ks = Castle::Side::KingSide;
            constexpr auto qs = Castle::Side::QueenSide;
            const auto next = turn_.next().id();
            const auto prev = turn_.prev().id();    
            if (target == Castle::rook_source(setup_, next, ks)) next_state.castle.pop(Castle(next, ks)); else
            if (target == Castle::rook_source(setup_, next, qs)) next_state.castle.pop(Castle(next, qs)); else
            if (target == Castle::rook_source(setup_, prev, ks)) next_state.castle.pop(Castle(prev, ks)); else
            if (target == Castle::rook_source(setup_, prev, qs)) next_state.castle.pop(Castle(prev, qs));
        }
    }

    pop_board(source, attacker);
    set_board(target, attacker);
     
    next_state.enpass = enpass(turn_.id());
    set_enpass(Square::offboard(), turn_.id());

    if (attacker.piece() == Piece::ID::King) {
        set_royal(target, turn_.id());
        next_state.castle.pop(Castle(turn_.id(), Castle::Side::KingSide ));
        next_state.castle.pop(Castle(turn_.id(), Castle::Side::QueenSide));
    }

    if (attacker.piece() == Piece::ID::Rook) {
        constexpr auto ks = Castle::Side::KingSide;
        constexpr auto qs = Castle::Side::QueenSide;
        if (target == Castle::rook_source(setup_, turn_.id(), ks)) next_state.castle.pop(Castle(turn_.id(), ks)); else
        if (target == Castle::rook_source(setup_, turn_.id(), qs)) next_state.castle.pop(Castle(turn_.id(), qs));
    }

    switch (policy)
    {
        case Move::Policy::Stride: {
            enpass_[static_cast<uint8_t>(turn_.id())] = Square::middle(source, target);
            break;
        }

        case Move::Policy::Enpass: {
            const auto pawn = PieceColor(move.enpass(), Piece::ID::Pawn);
            const auto offset = Square::push(turn_.id(), 0);
            pop_board(source + offset, pawn);
            break;
        }

        case Move::Policy::Castle: {
            const auto rook_source = Castle::rook_source(setup_, turn_.id(), move.castle());
            const auto rook_target = Castle::rook_target(setup_, turn_.id(), move.castle());
            const auto rook = PieceColor(turn_.id(), Piece::ID::Rook);
            pop_board(rook_source, rook);
            set_board(rook_target, rook);
            break;
        }

        case Move::Policy::Evolve: {
            const auto promoted = PieceColor(turn_.id(), move.evolve());
            pop_board(target, attacker);
            set_board(target, promoted);
            if (move.enpass() != Color::ID::None) {
                const auto pawn = PieceColor(move.enpass(), Piece::ID::Pawn);
                pop_board(source + Square::push(turn_.id(), 0), pawn);
            }
            break;
        }

        default: break;
    }

    const auto reset =
    (attacker.piece() == Piece::ID::Pawn ) |
    (defender.piece() != Piece::ID::Empty);
    next_state.fifty_move_clock = (
    next_state.fifty_move_clock + 1) * (reset ^ 1);

    play_++;
    turn_ = turn_.next();
    set_occupany_bitboards();
}

void Position::undo_move(Move move) noexcept {

    turn_ = turn_.prev();
    play_--;

    auto& prev_state = state_[play_ + 0];
    auto& curr_state = state_[play_ + 1];

    const auto source = move.source();
    const auto target = move.target();
    const auto policy = move.policy();

    const auto attacker = board(target);
    const auto defender = curr_state.capture;

    pop_board(target, attacker);
    set_board(source, attacker);

    if (defender != PieceColor::empty()) {
        set_board(target, defender);
    }

    set_enpass(curr_state.enpass, turn_.id());

    if (attacker.piece() == Piece::ID::King) {
        set_enpass(source, turn_.id());
    }

    switch (policy)
    {
        case Move::Policy::Enpass: {
            const auto pawn = PieceColor(move.enpass(), Piece::ID::Pawn);
            const auto offset = Square::push(turn_.id(), 0);
            set_board(source + offset, pawn);
            break;
        }

        case Move::Policy::Castle: {
            const auto rook_source = Castle::rook_source(setup_, turn_.id(), move.castle());
            const auto rook_target = Castle::rook_target(setup_, turn_.id(), move.castle());
            const auto rook = PieceColor(turn_.id(), Piece::ID::Rook);
            set_board(rook_source, rook);
            pop_board(rook_target, rook);
            break;
        }

        case Move::Policy::Evolve: {
            const auto promoted = PieceColor(turn_.id(), move.evolve());
            pop_board(source, promoted);
            set_board(source, attacker);
            set_board(target, defender);
            if (move.enpass() != Color::ID::None) {
                const auto pawn = PieceColor(move.enpass(), Piece::ID::Pawn);
                set_board(source + Square::push(turn_.id(), 0), pawn);
            }
            break;
        }

        default: break;
    }
}

void Position::undo_move(const std::string& move, bool board16x16) noexcept {}
void Position::make_move(const std::string& move, bool board16x16) noexcept {

    bool promote = !std::isdigit(move.back());
    auto x = std::isdigit(move[2]) ? 3 : 2;
    auto evolve = promote ? Piece(move.back()) : Piece::ID::Empty;
    auto source = Square(move.substr(0, x), board16x16);
    auto target = Square(promote ? move.substr(x, move.size() - x - 1)
                                 : move.substr(x), board16x16);

    const auto push_0 = (source + Square::push(turn_.id(), 0));      
    const auto push_1 = (source + Square::push(turn_.id(), 1));                        
    bool e1 = push_0 != target;
    bool e2 = board(target.id()) != PieceColor::empty();
    bool c1 = board(source.id()).piece() == Piece::ID::King;
    bool c2 = (source.rank() - target.rank() == 2) || (source.file() - target.file() == 2);
    bool s1 = board(source.id()).piece() == Piece::ID::Pawn;
    bool s2 = push_1 == target;

    auto enpass = e1 && e2 ? board(push_0.id()).color() : Color::ID::None;
    auto castle = c1 && c2 ? (Castle::king_target(setup_, turn_.id(), Castle::Side::KingSide) == target
        ? Castle::Side::KingSide
        : Castle::Side::QueenSide) 
        : Castle::Side(0b10);

    Move::Policy policy = 
    promote  ? Move::Policy::Evolve : 
    e1 && e2 ? Move::Policy::Enpass :
    c1 && c2 ? Move::Policy::Castle : 
    s1 && s2 ? Move::Policy::Stride :
               Move::Policy::Normal ; 

    make_move(Move(source.id(), target.id(), evolve.id(), enpass.id(), castle, policy));
}

// bool Position::legal_old(Move* move) const noexcept {

//     const auto& [pinned, checkers] = get_pinned_checkers_bitboards();
//     const auto ksq = royal(turn_.id());

//     const auto source = move->source();
//     const auto target = move->target();
//     const auto policy = move->policy();
//     const auto enpass = move->enpass();
   
//     if (board(target).piece() == Piece::ID::King) {
//         return true;
//     } 

//     if (policy == Move::Policy::Castle) {
//         const auto occ = occupied();
//         auto middle = Square::middle(source, target);
//         return (
//             get_attackers_bitboard(target, turn(), occ) | 
//             get_attackers_bitboard(middle, turn(), occ) | 
//             checkers).empty();
//     }
    
//     if (source == ksq) {
//         auto occ = occupied().pop_bit(ksq);
//         return get_attackers_bitboard(target, turn(), occ).empty();
//     }

//     if (enpass != Color::ID::None) {
//         auto occ = occupied().pop_bit(source).pop_bit(source + Square::push(turn_.id(), 0));
//         return get_attackers_bitboard(target, turn(), occ).empty();
//     }

//     return !pinned.has_bit(source) || get_line_through_mask(source, target).has_bit(ksq);
// }

// void Position::set_pinned_checkers_bitboards_old() noexcept {
//     auto& [pinned, checkers] = pinned_checkers_[play_];
//     pinned = Bitboard::zero();

//     const auto teammate_bb = teammate();
//     const auto opponent_bb = opponent();
//     const auto occupied_bb = occupied();

//     auto ksq = royal(turn_.id());
//     auto candidates =
//     (get_slide_attacks<Piece::ID::Bishop>(ksq) & (bitboard(Piece::ID::Bishop) | bitboard(Piece::ID::Queen))) |
//     (get_slide_attacks<Piece::ID::Rook  >(ksq) & (bitboard(Piece::ID::Rook  ) | bitboard(Piece::ID::Queen)));
//     candidates &= opponent_bb;

//     for (Bitboard::ChunkID chunk_id = 0; chunk_id < CHUNK_NB; chunk_id++) {
//         auto& chunk = candidates.chunk(chunk_id);
//         while(chunk) {
//             auto sq = Bitboard::pop_lsb(chunk, chunk_id);
//             auto bb = get_line_between_mask(sq, ksq) & occupied_bb;
//             auto bt = bb & bitboard(turn().id());
//             if (bb.count() == 1 && 
//                 bt.count() == 1) {
//                 pinned.set_bit(Bitboard::pop_lsb(bt));
//             }
//         }
//     }

//     checkers = get_attackers_bitboard(ksq, turn(), occupied_bb);
// }

// bool Position::legal(Move* move) const noexcept {

//     const auto& [pinned, checkers] = get_pinned_checkers_bitboards();
//     const auto ksq = royal(turn_.id());

//     const auto source = move->source();
//     const auto target = move->target();
//     const auto policy = move->policy();
//     const auto enpass = move->enpass();
   
//     if (board(target).piece() == Piece::ID::King) {
//         return true;
//     } 

//     if (policy == Move::Policy::Castle) {
//         const auto occ = occupied();
//         auto middle = Square::middle(source, target);
//         return (
//             get_attackers_bitboard(target, turn(), occ) | 
//             get_attackers_bitboard(middle, turn(), occ) | 
//             checkers).empty();
//     }
    
//     if (source == ksq) {
//         auto occ = occupied().pop_bit(ksq);
//         return get_attackers_bitboard(target, turn(), occ).empty();
//     }

//     if (enpass != Color::ID::None) {
//         auto occ = occupied().pop_bit(source).pop_bit(source + Square::push(turn_.id(), 0));
//         return get_attackers_bitboard(target, turn(), occ).empty();
//     }

//     return !pinned.has_bit(source) || get_line_through_mask(source, target).has_bit(ksq);
// }



// Bitboard Position::get_pinned_bitboard(
//     const Bitboard& teammate, 
//     const Bitboard& opponent,
//     const Bitboard& occupied,
//     const Bitboard& us) const noexcept {

//     Bitboard pinned = Bitboard::zero();

//     auto ksq = royal(turn_.id());
//     auto candidates =
//     (get_slide_attacks<Piece::ID::Bishop>(ksq) & (bitboard(Piece::ID::Bishop) | bitboard(Piece::ID::Queen))) |
//     (get_slide_attacks<Piece::ID::Rook  >(ksq) & (bitboard(Piece::ID::Rook  ) | bitboard(Piece::ID::Queen)));
//     candidates &= opponent;

//     set_pinned_bitboard<Bitboard::ChunkID(0)>(ksq, pinned, candidates, occupied, us);
//     set_pinned_bitboard<Bitboard::ChunkID(1)>(ksq, pinned, candidates, occupied, us);
//     set_pinned_bitboard<Bitboard::ChunkID(2)>(ksq, pinned, candidates, occupied, us);
//     set_pinned_bitboard<Bitboard::ChunkID(3)>(ksq, pinned, candidates, occupied, us);

//     return pinned;
// }

template<Bitboard::ChunkID chunk_id>
void set_pinned_bitboard(
    Square ksq, Bitboard& pinned, Bitboard& candidates, const Bitboard& occupied, const Bitboard& us) noexcept {
    auto& chunk = candidates.chunk(chunk_id);
    while(chunk) {
        auto sq = Bitboard::pop_lsb(chunk, chunk_id);
        auto bb = get_line_between_mask(sq, ksq) & occupied;
        auto bt = bb & us;
        if (bb.count() == 1 && 
            bt.count() == 1) {
            pinned.set_bit(Bitboard::pop_lsb(bt));
        }
    }
}

void Position::set_pinned_checks_bitboards() noexcept {
    const auto ksq = royal(turn().id());
    auto& [pinned, checks] = get_pinned_checks_bitboards();
    pinned = Bitboard::zero();
    checks = Bitboard::ones();
    
    // Set checks
    const auto attackers = get_attackers_bitboard(ksq, turn().id(), occupied());
    if (attackers.any()) {
        if (attackers.count() == 1) {
            const auto sq = Bitboard::lsb(attackers);
            checks &= get_line_between_mask(ksq, sq);
            checks |= (opponent() & bitboard(Piece::ID::King)); 
            checks.set_bit(sq);    
        } else {
            checks = (opponent() & bitboard(Piece::ID::King)); 
        }  
    }

    // Set pinned
    auto candidates =
    (get_slide_attacks<Piece::ID::Bishop>(ksq) & (bitboard(Piece::ID::Bishop) | bitboard(Piece::ID::Queen))) |
    (get_slide_attacks<Piece::ID::Rook  >(ksq) & (bitboard(Piece::ID::Rook  ) | bitboard(Piece::ID::Queen)));
    candidates &= opponent();

    const auto us = bitboard(turn().id());
    set_pinned_bitboard<Bitboard::ChunkID(0)>(ksq, pinned, candidates, occupied(), us);
    set_pinned_bitboard<Bitboard::ChunkID(1)>(ksq, pinned, candidates, occupied(), us);
    set_pinned_bitboard<Bitboard::ChunkID(2)>(ksq, pinned, candidates, occupied(), us);
    set_pinned_bitboard<Bitboard::ChunkID(3)>(ksq, pinned, candidates, occupied(), us);
}

Bitboard Position::get_attackers_bitboard(Square sq, Color color, const Bitboard& occupied) const noexcept {

    return ((bitboard(turn().next().id())  | 
             bitboard(turn().prev().id())) & ( 
        (get_slide_attacks<Piece::ID::Bishop>(sq, occupied) & (bitboard(Piece::ID::Bishop) | bitboard(Piece::ID::Queen))) |
        (get_slide_attacks<Piece::ID::Rook  >(sq, occupied) & (bitboard(Piece::ID::Rook  ) | bitboard(Piece::ID::Queen))) |
        (get_crawl_attacks<Piece::ID::Knight>(sq) & bitboard(Piece::ID::Knight))   |
        (get_crawl_attacks<Piece::ID::King  >(sq) & bitboard(Piece::ID::King  )))) |
        (get_pawn_attacks(sq, color.next().id())  & bitboard(Piece::ID::Pawn) & bitboard(color.prev().id())) |
        (get_pawn_attacks(sq, color.prev().id())  & bitboard(Piece::ID::Pawn) & bitboard(color.next().id()));
}

void Position::init(const FEN& fen) noexcept {

    std::istringstream ss(fen);
    std::string token;
    clear_state();
    clear_board();
    auto& st = state();

    // 1. Parse turn //
    std::getline(ss, token, '-');
    turn_ = Color(std::tolower(token[0]));
    std::getline(ss, token, '-');

    // 2. Parse castling rights //
    std::getline(ss, token, '-');
    auto ks = Castle(token, Castle::Side::KingSide );
    std::getline(ss, token, '-');
    auto qs = Castle(token, Castle::Side::QueenSide);
    st.castle = Castle(
        static_cast<uint8_t>(ks.id()) | 
        static_cast<uint8_t>(qs.id())
    );

    // 3. Parse fiftymove //
    std::getline(ss, token, '-');
    std::getline(ss, token, '-');
    st.fifty_move_clock = std::stoi(token);

    // 4. Parse enpassant //
    std::getline(ss, token, '-');

    bool hasEnpass = token.find('(') != std::string::npos;
    if (hasEnpass) {
        const auto lbrace = token.find('(');
        const auto rbrace = token.find(')');
        const auto substr = token.substr(lbrace + 1, rbrace - lbrace - 1);

        std::istringstream epss(substr);
        std::string epToken;
        for (auto color : 
                {Color::ID::Red,
                 Color::ID::Blue,
                 Color::ID::Yellow,
                 Color::ID::Green}) {

            std::getline(epss, epToken, ',');
            const std::size_t lquote = 1;
            const std::size_t rquote = epToken.size() - 2;
            const auto square = epToken.substr(lquote, rquote);
            if (square.empty()) continue;
            std::string sqToken = square.substr(0, square.find(':'));
            set_enpass(Square(sqToken), color);
        }

        std::getline(ss, token, '-');
    }

    // 5. Parse board //
    std::istringstream boardss(token);
    std::string rankStr;
    int row = 14, col = 1;
    while (std::getline(boardss, rankStr, '/')) {
        std::istringstream rankss(rankStr);
        std::string fileStr;
        while (std::getline(rankss, fileStr, ',')) {
            Square sq = Square(col++, row);
            if (sq.isStone()) continue;
            if (std::all_of(fileStr.begin(), fileStr.end(), ::isdigit)) {
                col += std::stoi(fileStr) - 1;
                continue;
            }
            const auto color = Color(fileStr[0]); 
            const auto piece = Piece(fileStr[1]);
            set_board(sq, PieceColor(color.id(), piece.id()));
            if (piece == Piece::ID::King) set_royal(sq, color.id());
        }
        row--, col = 1;
    }

    set_occupany_bitboards();
}

auto Position::fen() const noexcept {

    const auto& st = state();
    std::string output = "";

    // 1. Turn //
    output += std::toupper(turn().uci());
    output += '-';

    // 2. Status //
    output += "0,0,0,0";
    output += '-';

    // 3. Castle //
    output += st.castle.uci(Castle::Side::KingSide ); output += '-';
    output += st.castle.uci(Castle::Side::QueenSide); output += '-';

    // 5. Scores //
    output += "0,0,0,0";
    output += '-';

    // 6. Clock  //
    output += std::to_string(st.fifty_move_clock);
    output += '-';

    // 7. Enpassant //
    constexpr auto prefix = "{\'enPassant\':(";
    constexpr auto suffix = ")}";

    std::string ep = "";
    bool any = false;
    for (auto color : {Color::ID::Red, Color::ID::Blue, Color::ID::Yellow, Color::ID::Green}) {

        Square sq1 = enpass(color);
        Square sq2 = enpass(color);
        sq2 += Square::push(color, 0);

        if (sq1 == Square::offboard()) {
            ep += "\'\'";
            continue;
        }

        any = true;
        ep += '\'';
        ep += sq1.uci();
        ep += ':';
        ep += sq2.uci();
        ep += '\'';

        if (color != Color::ID::Green) ep += ',';
    }

    if (any) output += prefix + ep + suffix + '-';

    // 8. Board //
    int count = 0;
    for (int row = 14; row >= 1; row--) {
    for (int col = 1; col <= 14; col++) {

        auto sq = Square(col, row);
        auto pc = board(sq.id());

        if (pc == PieceColor::stone()) {
            if (count) {
                output += std::to_string(count) + ",";
                count = 0;
            }
            output += 'x';
            output += ',';
            continue;
        }
            
        if (pc != PieceColor::empty()) {
            if (count) {
                output += std::to_string(count) + ",";
                count = 0;
            }
            output += pc.uci();
            output += ',';
            continue;
        }

        count++;
    }

    if (count) {
        output += std::to_string(count) + ",";
        count = 0;
    }
    output.back() = '/';
    }

    output.pop_back();
    return output;
}

void Position::print(bool board16x16) const {
    constexpr std::size_t KEY_WIDTH = 12;
    const auto& st = state();
    int start = board16x16 ? 0 : 1;
    int end   = board16x16 ? RANK_NB:  RANK_NB - 1;
    std::cout << "\n     ";
    for (int file = start; file < end; ++file)std::cout << static_cast<char>('a' + (file - start)) << "  ";
    std::cout << "\n   +" << std::string((end - start) * 3 + 1, '-') << "+\n";
    for (int rank = end - 1; rank >= start; --rank) {
        std::cout << (((rank - start + 1) < 10) ? " " : "") << (rank - start + 1) << " | ";
        for (int file = start; file < end; ++file) {
            auto sq = Square(file, rank);
            auto pc = board(sq.id());
            auto piece = pc.piece();
            auto color = pc.color();
            const char* sqStr;
            switch (piece.id()) {
                case Piece::ID::Pawn  : sqStr = "\u2659"; break;
                case Piece::ID::Queen : sqStr = "\u2655"; break;
                case Piece::ID::Bishop: sqStr = "\u2657"; break;
                case Piece::ID::Rook  : sqStr = "\u2656"; break;
                case Piece::ID::Knight: sqStr = "\u2658"; break;
                case Piece::ID::King  : sqStr = "\u2654"; break;
                case Piece::ID::Stone : sqStr = "x"     ; break;
                case Piece::ID::Empty : sqStr = "\033[2m.\033[0m"; break;
            }
            const char* colorCode;
            switch (color.id()) {
                case Color::ID::Red   : colorCode = "\033[31m"; break;
                case Color::ID::Blue  : colorCode = "\033[34m"; break;
                case Color::ID::Yellow: colorCode = "\033[33m"; break;
                case Color::ID::Green : colorCode = "\033[32m"; break;
                default: colorCode = ""; break;
            }
            std::cout << colorCode << sqStr << "\033[0m  ";
            }
            std::cout << "| " << (rank - start + 1) << "\n";
    }
    std::cout << "   +" << std::string((end - start) * 3 + 1, '-') << "+\n";
    std::cout << "     ";
    for (int file = start; file < end; ++file) std::cout << static_cast<char>('a' + (file - start)) << "  ";
    std::cout << "\n";
    std::cout << "\n" << std::left << std::setw(KEY_WIDTH) << "turn:";
    switch (turn_.id()) {
        case Color::ID::Red   : std::cout << "\033[1;31mRed\033[0m   " << "\n"; break;
        case Color::ID::Blue  : std::cout << "\033[1;34mBlue\033[0m  " << "\n"; break;
        case Color::ID::Yellow: std::cout << "\033[1;33mYellow\033[0m" << "\n"; break;
        case Color::ID::Green : std::cout << "\033[1;32mGreen\033[0m " << "\n"; break;
        default               : std::cout << "unknown"                 << "\n"; break;
    }
    std::string ks = st.castle.uci(Castle::Side::KingSide );
    std::string qs = st.castle.uci(Castle::Side::QueenSide);
    std::cout << std::left << std::setw(KEY_WIDTH) << "kingside:"  << ks << "\n"
              << std::left << std::setw(KEY_WIDTH) << "queenside:" << qs << "\n"
              << std::left << std::setw(KEY_WIDTH) << "fiftymove:" << static_cast<int>(st.fifty_move_clock) << "\n"
              << std::left << std::setw(KEY_WIDTH)
              << "enpassant:";
    for (auto color : 
            {Color::ID::Red, 
             Color::ID::Blue, 
             Color::ID::Yellow, 
             Color::ID::Green}){
        auto ep = enpass(color);
        auto uci = ep.id() == Square::offboard() ? "-" : ep.uci(board16x16);
        std::cout << uci << (color == Color::ID::Green ? "" : ",");
    }
    std::cout << "\n";
    std::cout << std::left << std::setw(KEY_WIDTH)
              << "royals:";
    for (auto color : 
            {Color::ID::Red, 
             Color::ID::Blue, 
             Color::ID::Yellow, 
             Color::ID::Green})
        std::cout << royal(color).uci(board16x16)  << (color == Color::ID::Green ? "" : ",");
    std::cout << "\n";
}

} // namespace athena