// #include <algorithm>
// #include <cctype>
// #include <cstdint>
// #include <iostream>
// #include <iomanip>
#include "chess.h"
// #include "utility.h"

namespace athena
{

// Move::Move(std::string_view move, const Position& pos, const Config& cfg)
// {
//     Piece promotion;
//     if (std::isalpha(move.back()))
//     {
//         promotion = Piece(move.back());
//         move.remove_suffix(1);
//     }

//     else promotion = Piece::Empty;

//     size_t idx = std::isdigit(move[2]) ? 3 : 2;
//     Square source = Square(move.substr(0, idx));
//     Square target = Square(move.substr(idx));

//     PieceColor attacker = pos.board(source);
//     PieceColor defender = pos.board(target);
//     Piece piece = attacker.piece();
//     Color color = attacker.color();

//     if (piece == Piece::King)
//     {
//         auto ks = CASTLE_SQUARES[cfg.setup][color.encoded_][Side::KingSide ];
//         auto qs = CASTLE_SQUARES[cfg.setup][color.encoded_][Side::QueenSide];

//         // Noisy move
//         if (defender != PieceColor::empty())
//             *this = Move(source, target, MoveType::Simple, MoveFlag::Noisy, Piece::Empty, Color::None, Side(0x2));

//         // Castle move
//         else if (ks.king_source == source && ks.king_target == target)
//             *this = Move(source, target, MoveType::Castle, MoveFlag::Quiet, Piece::Empty, Color::None, Side::KingSide);
        
//         // Castle move
//         else if (qs.king_source == source && qs.king_target == target)
//             *this = Move(source, target, MoveType::Castle, MoveFlag::Quiet, Piece::Empty, Color::None, Side::QueenSide);

//         // Quiet move
//         else *this = Move(source, target, MoveType::Simple, MoveFlag::Quiet, Piece::Empty, Color::None, Side(0x2));
//     }

//     else if (piece == Piece::Pawn)
//     {
//         PawnOffsets offsets = PAWN_OFFSETS[color.encoded_];

//         // Promotion move
//         if (promotion != Piece::Empty)
//             *this = Move(source, target, MoveType::Evolve, MoveFlag::Noisy, promotion, Color::None, Side(0x2));

//         else if ((source + offsets.take[0] == target) || (source + offsets.take[1] == target))
//         {
//             // Enpassant move
//             if (defender == PieceColor::empty())
//                 *this = Move(source, target, MoveType::Enpass, MoveFlag::Noisy, Piece::Empty,  pos.board(source + offsets.push).color(), Side(0x2));

//             // Capture move
//             else *this = Move(source, target, MoveType::Simple, MoveFlag::Noisy, Piece::Empty, Color::None, Side(0x2));
//         }

//         // Push move
//         else if (source + offsets.push == target)
//             *this = Move(source, target, MoveType::Simple, MoveFlag::Quiet, Piece::Empty, Color::None, Side(0x2));
        
//         // Double Push move
//         else *this = Move(source, target, MoveType::Simple, MoveFlag::Quiet, Piece::Empty, Color::None, Side(0x2));
//     }

//     // Quiet moves
//     else if (defender == PieceColor::empty())
//         *this = Move(source, target, MoveType::Simple, MoveFlag::Quiet, Piece::Empty, Color::None, Side(0x2));
    
//     // Noisy moves
//     else Move(source, target, MoveType::Simple, MoveFlag::Noisy, Piece::Empty, Color::None, Side(0x2));
// }

// void Position::makemove(Move move, const Config& cfg)
// {
//     play_++;

//     GameState& curr = states_[play_ - 1];
//     GameState& next = states_[play_ - 0];

//     auto source = move.source();
//     auto target = move.target();

//     auto type = move.type();
//     auto flag = move.flag();

//     auto attacker = board_[source.encoded_];
//     auto defender = board_[target.encoded_];

//     next.capture = defender;

//     board_[target.encoded_] = attacker;
//     board_[source.encoded_] = PieceColor::empty(); // set pop zobrist

//     // Update enpassant
//     next.enpass = enpass_[turn_.encoded_];
//     enpass_[turn_.encoded_] = Square::OFFBOARD;
    
//     // Update castling rights
//     next.rights = curr.rights;
//     next.rights &= RIGHTS[cfg.setup][source.encoded_];
//     next.rights &= RIGHTS[cfg.setup][target.encoded_];

//     // Update royal square
//     if (attacker.piece() == Piece::King)
//         royals_[turn_.encoded_] = target;

//     // Handle movetypes
//     if (type == MoveType::Stride)
//         enpass_[turn_.encoded_] = (source.encoded_ + target.encoded_) / 2;
    
//     if (type == MoveType::Enpass)
//         board_[(source + PAWN_OFFSETS[turn_.encoded_].push).encoded_] = PieceColor::empty();

//     if (type == MoveType::Castle)
//     {
//         const auto castle = CASTLE_SQUARES[cfg.setup][turn_.encoded_];

//         board_[castle[Side::KingSide].rook_source.encoded_] = PieceColor::empty();
//         board_[castle[Side::KingSide].rook_target.encoded_] = PieceColor(Piece::Rook, turn_);
//     }

//     if (type == MoveType::Evolve) { board_[target.encoded_] = PieceColor(move.evolve(), turn_); } 

//     // Update fiftymove clock rule
//     bool condition = attacker.piece() == Piece::Pawn || flag == MoveFlag::Noisy || type == MoveType::Castle;
//     next.fiftymove = condition ? 0 : curr.fiftymove + 1;

//     // Update turn
//     turn_ = turn_.next();
// }

// void Position::undomove(Move move, const Config& cfg)
// {
//     turn_ = turn_.prev();

//     play_--;

//     GameState& prev = states_[play_ + 0];
//     GameState& curr = states_[play_ + 1];

//     auto source = move.source();
//     auto target = move.target();

//     auto type = move.type(); 
//     auto flag = move.flag();

//     auto attacker = board_[target.encoded_];
//     auto defender = curr.capture;

//     board_[target.encoded_] = defender;
//     board_[source.encoded_] = attacker;

//     // Update enpassant
//     enpass_[turn_.encoded_] = curr.enpass;

//     // Update royal square
//     if (attacker.piece() == Piece::King)
//         royals_[turn_.encoded_] = source;

//     if (type == MoveType::Enpass)
//         board_[(source + PAWN_OFFSETS[turn_.encoded_].push).encoded_] = PieceColor(Piece::Pawn, move.enpass());
  
//     if (type == MoveType::Castle)
//     {
//         const auto castle = CASTLE_SQUARES[cfg.setup][turn_.encoded_];

//         board_[castle[Side::KingSide].rook_source.encoded_] = PieceColor(Piece::Rook, turn_);
//         board_[castle[Side::KingSide].rook_target.encoded_] = PieceColor::empty();   
//     }

//     if (type == MoveType::Evolve)
//         board_[source.encoded_] = PieceColor(Piece::Pawn, turn_);
// }

} // namespace athena