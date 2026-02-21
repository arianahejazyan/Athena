#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "position.h"
#include "attacks.h"
#include "bitboard.h"
#include "chess.h"
#include "color.h"
#include "utility.h"
#include <string_view>
#include "uci.h"
#include <algorithm>

namespace athena
{

void Position::init(FEN fen)
{
    const auto tokens = tokenize(fen, '-');
    GameState& gs = states_[play_ = 0];

    // Reset gs variables
    gs.capture = PieceClass::Empty();
    gs.enpass  = Square::Offboard;

    // 1. Parse turn //
    turn_ = Color(tokens[0]);

    // 2. Parse castling rights //
    Castle castle(tokens[2], tokens[3]);
    gs.castle = castle;

    // uint8_t castle = 0;

    // if (tokens[2][0] == '1') castle |= static_cast<uint8_t>(Castle::rKS);
    // if (tokens[2][2] == '1') castle |= static_cast<uint8_t>(Castle::bKS);
    // if (tokens[2][4] == '1') castle |= static_cast<uint8_t>(Castle::yKS);
    // if (tokens[2][6] == '1') castle |= static_cast<uint8_t>(Castle::gKS);
    // if (tokens[3][0] == '1') castle |= static_cast<uint8_t>(Castle::rQS);
    // if (tokens[3][2] == '1') castle |= static_cast<uint8_t>(Castle::bQS);
    // if (tokens[3][4] == '1') castle |= static_cast<uint8_t>(Castle::yQS);
    // if (tokens[3][6] == '1') castle |= static_cast<uint8_t>(Castle::gQS);

    // gs.castle = static_cast<Castle>(castle);

    // 3. Parse fiftymove //
    gs.fiftymove = std::stoi(std::string(tokens[5]));

    // 4. Parse enpassant //
    enpass_.fill(Square::Offboard);

    if (tokens.size() == 8) // // {'enPassant':('k3:k4','d3:','','')}
    {
        const auto lbrace = tokens[6].find('(');
        const auto rbrace = tokens[6].find(')');
        const auto substr = tokens[6].substr(lbrace + 1, rbrace - lbrace - 1);

        const auto enPassant = tokenize(substr, ',');
        for (Color color : {Color::Red, Color::Blue, Color::Yellow, Color::Green})
        {
            const std::size_t lquote = 1;
            const std::size_t rquote = enPassant[static_cast<uint8_t>(color)].size() - 2;

            const auto square = enPassant[static_cast<uint8_t>(color)].substr(lquote, rquote);

            if (square.empty())
                continue;

            enpass_[static_cast<uint8_t>(color)] = Square(tokenize(square, ':')[0]); 
        }
    }

    // 5. Parse board //
    royals_.fill(Square::Offboard);
    
    pieces_.fill(Bitboard(0,0,0,0));
    colors_.fill(Bitboard(0,0,0,0));

    for (int sq = 0; sq < SQUARE_NB; ++sq)
    {
        auto s = static_cast<Square>(sq);
        auto r = s.rank();
        auto f = s.file();
        board_[static_cast<uint8_t>(s)] = s.stone() ? PieceClass::Stone() : PieceClass::Empty();
    }
 
    // Split board into ranks
    const auto ranks = tokenize(tokens.back(), '/');

    // Initializing board
    int row = 14, col = 1;
    for (const auto rank: ranks)
    {
        const auto files = tokenize(rank, ',');
        for (const auto file: files)
        {
            Square sq = Square(row, col++);

            // Handling stones
            if (sq.stone()) continue;

            // Handling numbers
            if (std::all_of(file.begin(), file.end(), ::isdigit))
            {
                col += std::stoi(std::string(file)) - 1;
                continue;
            }

            const auto color = Color(file.substr(0, 1));
            const auto piece = Piece(file.substr(1, 1));
            
            setPiece(sq, PieceClass(color, piece));
        
            // Handling royal squares
            if (piece == Piece::King) royals_[static_cast<uint8_t>(color)] = sq;     
        }

        // Moving to the next rank
        row--, col = 1;
    }
}

void Position::makemove(Move move)
{
    play_++;

    GameState& currGS = states_[play_ - 1];
    GameState& nextGS = states_[play_ - 0];

    auto source = move.source();
    auto target = move.target();

    auto type = move.type();
    auto flag = move.flag();

    auto attacker = board_[static_cast<uint8_t>(source)];
    auto defender = board_[static_cast<uint8_t>(target)];

    nextGS.capture = defender;

    if (defender != PieceClass::Empty())
    {
        pieces_[static_cast<uint8_t>(defender.piece())].pop(target);
        colors_[static_cast<uint8_t>(defender.color())].pop(target);
    }

    popPiece(source);
    setPiece(target, attacker);
    
    // Update enpassant
    nextGS.enpass = enpass_[static_cast<uint8_t>(turn_)];
    enpass_[static_cast<uint8_t>(turn_)] = Square::Offboard;
    
    // Update castling rights
    nextGS.castle = currGS.castle;
    nextGS.castle &= castling_rights(source);
    nextGS.castle &= castling_rights(target);

    if (attacker.piece() == Piece::King) {
        royals_[static_cast<uint8_t>(turn_)] = target;
    }

    // Handle movetypes
    if (type == MoveType::Stride)
    {
        enpass_[static_cast<uint8_t>(turn_)] = (source + target) >> 1;
    }
    
    else if (type == MoveType::Enpass)
    {
        const auto offset = 
        turn_ == Color::Red    ? push_offsets(Color::Red   )[0] : 
        turn_ == Color::Blue   ? push_offsets(Color::Blue  )[0] : 
        turn_ == Color::Yellow ? push_offsets(Color::Yellow)[0] : 
        turn_ == Color::Green  ? push_offsets(Color::Green )[0] : Offset::None;

        board_[static_cast<uint8_t>(source + offset)] = PieceClass::Empty();
    }
        
    else if (type == MoveType::Castle)
    {
        std::array<Square, 4> squares {};
        if (move.castle() == Side::KingSide)
        {
            squares =
            turn_ == Color::Red    ? castle_squares(Color::Red   , Side::KingSide) : 
            turn_ == Color::Blue   ? castle_squares(Color::Blue  , Side::KingSide) : 
            turn_ == Color::Yellow ? castle_squares(Color::Yellow, Side::KingSide) : 
            turn_ == Color::Green  ? castle_squares(Color::Green , Side::KingSide) : std::array<Square, 4>();
        }

        else
        {
            squares =
            turn_ == Color::Red    ? castle_squares(Color::Red   , Side::QueenSide) : 
            turn_ == Color::Blue   ? castle_squares(Color::Blue  , Side::QueenSide) : 
            turn_ == Color::Yellow ? castle_squares(Color::Yellow, Side::QueenSide) : 
            turn_ == Color::Green  ? castle_squares(Color::Green , Side::QueenSide) : std::array<Square, 4>();
        }

        board_[static_cast<uint8_t>(squares[2])] = PieceClass::Empty();
        board_[static_cast<uint8_t>(squares[3])] = PieceClass(turn_, Piece::Rook);   
    }

    else if (type == MoveType::Evolve) 
    { 
        board_[static_cast<uint8_t>(target)] = PieceClass(turn_, move.evolve()); 
    }

    // Update fiftymove clock rule
    if (attacker.piece() == Piece::Pawn || flag == MoveFlag::Noisy || type == MoveType::Castle)
         nextGS.fiftymove = 0;
    else nextGS.fiftymove = currGS.fiftymove + 1;
    
    turn_ = turn_.next();
}

void Position::undomove(Move move)
{
    play_--;

    GameState& prevGS = states_[play_ + 0];
    GameState& currGS = states_[play_ + 1];

    turn_ = turn_.prev();

    auto source = move.source();
    auto target = move.target();

    auto type = move.type(); 
    auto flag = move.flag();

    auto attacker = board_[static_cast<uint8_t>(target)];
    auto defender = currGS.capture;

    setPiece(target, defender);
    setPiece(source, attacker);

    // Update enpassant
    enpass_[static_cast<uint8_t>(turn_)] = currGS.enpass;

    if (attacker.piece() == Piece::King) {
        royals_[static_cast<uint8_t>(turn_)] = source;
    }

    else if (type == MoveType::Enpass)
    {
        const auto offset = 
        turn_ == Color::Red    ? push_offsets(Color::Red   )[0] : 
        turn_ == Color::Blue   ? push_offsets(Color::Blue  )[0] : 
        turn_ == Color::Yellow ? push_offsets(Color::Yellow)[0] : 
        turn_ == Color::Green  ? push_offsets(Color::Green )[0] : Offset::None;

        board_[static_cast<uint8_t>(source + offset)] = PieceClass(move.enpass(), Piece::Pawn);
    }
        
    else if (type == MoveType::Castle)
    {
        std::array<Square, 4> squares {};
        if (move.castle() == Side::KingSide)
        {
            squares =
            turn_ == Color::Red    ? castle_squares(Color::Red   , Side::KingSide) : 
            turn_ == Color::Blue   ? castle_squares(Color::Blue  , Side::KingSide) : 
            turn_ == Color::Yellow ? castle_squares(Color::Yellow, Side::KingSide) : 
            turn_ == Color::Green  ? castle_squares(Color::Green , Side::KingSide) : std::array<Square, 4>();
        }

        else
        {
            squares =
            turn_ == Color::Red    ? castle_squares(Color::Red   , Side::QueenSide) : 
            turn_ == Color::Blue   ? castle_squares(Color::Blue  , Side::QueenSide) : 
            turn_ == Color::Yellow ? castle_squares(Color::Yellow, Side::QueenSide) : 
            turn_ == Color::Green  ? castle_squares(Color::Green , Side::QueenSide) : std::array<Square, 4>();
        }

        board_[static_cast<uint8_t>(squares[2])] = PieceClass(turn_, Piece::Rook);
        board_[static_cast<uint8_t>(squares[3])] = PieceClass::Empty();   
    }

    else if (type == MoveType::Evolve)
    {
        board_[static_cast<uint8_t>(source)] = PieceClass(turn_, Piece::Pawn);
    }   
}

bool Position::inCheck(Color color, Square sq) const
{
    // Checked by Knight
    for (auto offset : crawl_offsets(Piece::Knight))
    {
        PieceClass pc = board(sq + offset);

        if (pc.piece() == Piece::Knight &&  pc.color().is_opponent(color))
        {
            return true;
        }
    }

    // Checked by King
    for (auto offset : crawl_offsets(Piece::King))
    {
        PieceClass pc = board(sq + offset);

        if (pc.piece() == Piece::King &&  pc.color().is_opponent(color))
        {
            return true;
        }
    }

    // Checked by Queen or Bishop
    for (auto offset : slide_offsets(Piece::Bishop))
    {
        Square target = sq;
        bool blocked = false;

        for (int step = 0; step < max_step(Piece::Bishop) && !blocked; ++step)
        {
            target += offset;
            PieceClass pc = board(target);
    
            bool is_stone = (pc == PieceClass::Stone());
            bool is_empty = (pc == PieceClass::Empty());
            bool is_enemy = pc.color().is_opponent(color) && (pc.piece() == Piece::Bishop || pc.piece() == Piece::Queen);

            if (!is_stone && !is_empty && is_enemy)
                return true;

            blocked = is_stone || !is_empty;
        }
    }

    // Checked by Queen or Rook
    for (auto offset : slide_offsets(Piece::Rook))
    {
        Square target = sq;
        bool blocked = false;

        for (int step = 0; step < max_step(Piece::Rook) && !blocked; ++step)
        {
            target += offset;
            PieceClass pc = board(target);
    
            bool is_stone = (pc == PieceClass::Stone());
            bool is_empty = (pc == PieceClass::Empty());
            bool is_enemy = pc.color().is_opponent(color) && (pc.piece() == Piece::Rook || pc.piece() == Piece::Queen);

            if (!is_stone && !is_empty && is_enemy)
                return true;

            blocked = is_stone || !is_empty;
        }
    }

    constexpr std::array<Offset, TAKE_NB> takeR = take_offsets(Color::Red   );
    constexpr std::array<Offset, TAKE_NB> takeB = take_offsets(Color::Blue  );
    constexpr std::array<Offset, TAKE_NB> takeY = take_offsets(Color::Yellow);
    constexpr std::array<Offset, TAKE_NB> takeG = take_offsets(Color::Green );

    // Checked by Pawn
    for (Color opponent: opponents(color))
    {
        for (auto offset : (opponent == Color::Red ? takeY : opponent == Color::Blue ? takeG : opponent == Color::Yellow ? takeR : takeB))
        {
            if (board(sq + offset) == PieceClass(opponent, Piece::Pawn))
            {
                return true;
            }
        }
    }

    return false;
}

bool Position::inCheck(Color color) const {
    return inCheck(color, royal(color));
}

void Position::compute_masks()
{
    const auto& color = turn();
    const auto& sq = royal(color);

    // auto& [checkmask, pinned] = masks_;

    // Reset movegen masks
    // checkmask_ = 0;
    // pinned_ = 0;

    // Bitboard checkers = 0;

    // // Checked by King or Knight
    // auto [king, knight] = crawl_attacks(sq);

    // king   &= opponent(Piece::King  );
    // knight &= opponent(Piece::Knight);

    // checkers |= king  ;
    // checkers |= knight;

    // // Checked by Bishop
    // auto bishop = bishop_attacks(sq, occupied);
    // bishop &= opponent(Piece::Queen, Piece::Bishop);
    // checkers |= bishop;

    // // Checked by Rook
    // auto rook = rook_attacks(sq, occupied);
    // rook &= opponent(Piece::Queen, Piece::Rook);
    // checkers |= rook;

    // // Checked by Pawn
    // auto pawn = pawn_attacks(sq, color.oppo());
    // pawn &= opponent(Piece::Pawn);
    // checkers |= pawn;

    // Bitboard candidates =
    // bishop_attacks(sq, opponent(Piece::Bishop, Piece::Queen)) |
    //   rook_attacks(sq, opponent(Piece::Rook  , Piece::Queen));

    Bitboard bishop_candidates = bishop_attacks(sq) & pieces(Piece::Bishop) & colors(color);
    while (!bishop_candidates.empty())
    {
        Square candidate;

        count = between<Bishop>(sq, candidate) & occupany;

        // Candidate is checker
        if (count == 0)
        {
            checkers |= candidate;
        }

        else if (count == 1 && self)
        {
            pinned_square;

            // Candidate is pinner
            if (self)
            {
                pinned |= pinned_square;
            }
        }
    }

    // // Compute checkmask
    // switch (checkers.count())
    // {
    //     case 0:
    //         checkmask = UINT64_MAX;
    //         break;
    //     case 1:
    //         checkmask = checkers;
    //         break;
    //     case 2:
    //         checkmask = 0;
    //         break;
    // }
}

std::string Position::fen() const//{'enPassant':('k3:k4','d3:','','')}
{
    const auto& gs = state();
    std::string output = "";

    // 1. Converting turn //
    output += std::toupper(turn().uci()[0]);
    output += '-';

    // 2. Converting status //
    output += "0,0,0,0";
    output += '-';

    // 3. Converting kingside castling rights //
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::rKS)) ? "1" : "0"); output += ',';
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::bKS)) ? "1" : "0"); output += ',';
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::yKS)) ? "1" : "0"); output += ',';
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::gKS)) ? "1" : "0");
    output += gs.castle.uci(Side::KingSide);
    output += '-';

    // 4. Converting queenside castling rights  //
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::rQS)) ? "1" : "0"); output += ',';
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::bQS)) ? "1" : "0"); output += ',';  
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::yQS)) ? "1" : "0"); output += ',';
    // output += (static_cast<uint8_t>(gs.castle) & (static_cast<uint8_t>(Castle::gQS)) ? "1" : "0");
    output += gs.castle.uci(Side::QueenSide);
    output += '-';

    // 5. Converting scores //
    output += "0,0,0,0";
    output += '-';

    // 6. converting fiftymove clock rule //
    output += std::to_string(gs.fiftymove);
    output += '-';

    // 7. Converting enpassant //
    constexpr auto prefix = "{\'enPassant\':(";
    constexpr auto suffix = ")}";

    std::string ep = "";
    bool any = false;
    for (Color color : {Color::Red, Color::Blue, Color::Yellow, Color::Green})
    {
        Square sq1 = enpass(color);
        Square sq2 = enpass(color);

        switch (static_cast<uint8_t>(color))
        {
            case static_cast<uint8_t>(Color::Red)   : sq2 += push_offsets(Color::Red   )[0]; break;
            case static_cast<uint8_t>(Color::Blue)  : sq2 += push_offsets(Color::Blue  )[0]; break;
            case static_cast<uint8_t>(Color::Yellow): sq2 += push_offsets(Color::Yellow)[0]; break;
            case static_cast<uint8_t>(Color::Green) : sq2 += push_offsets(Color::Green )[0]; break;
            default: break;
        }

        if (sq1 == Square::Offboard)
        {
            ep += "\'\'";
            continue;
        }

        any = true;
        ep += '\'';
        ep += sq1.uci();
        ep += ':';
        ep += sq2.uci();
        ep += '\'';

        // Avoiding trailing comma
        if (color != Color::Green) ep += ',';
    }

    if (any) output += prefix + ep + suffix + '-';

    // 8. Converting board //
    int count = 0;
    for (int row = 14; row >= 1; row--)
    {
        for (int col = 1; col <= 14; col++)
        {
            Square sq = Square(row, col);
            PieceClass pc = board_[static_cast<uint8_t>(sq)];

            if (pc == PieceClass::Stone())
            {
                if (count)
                {
                    output += std::to_string(count) + ",";
                    count = 0;
                }

                output += 'x';
                output += ',';
            }
                
            else if (pc != PieceClass::Empty())
            {
                if (count)
                {
                    output += std::to_string(count) + ",";
                    count = 0;
                }

                output += pc.uci();
                output += ',';
            }

            else count++;
        }

        if (count)
        {
            output += std::to_string(count) + ",";
            count = 0;
        }

        // Replace trainling ',' with '/'
        output.back() = '/';
    }

    // Remove trailing '/'
    output.pop_back();

    return output;
}

void Position::print() const
{
    constexpr std::size_t KEY_WIDTH = 12;
    const auto& gs = state();

    int start = 1;
    int end   =  RANK_NB - 1;

    std::cout << "\n     ";
    for (int file = start; file < end; ++file)
        std::cout << static_cast<char>('a' + (file - start)) << "  ";
    std::cout << "\n   +" << std::string((end - start) * 3 + 1, '-') << "+\n";

    for (int rank = end - 1; rank >= start; --rank)
    {
        std::cout << (((rank - start + 1) < 10) ? " " : "") << (rank - start + 1) << " | ";
        for (int file = start; file < end; ++file)
        {
            auto sq = Square(rank, file);
            auto pc = board_[static_cast<uint8_t>(sq)];

            auto piece = pc.piece();
            auto color = pc.color();

            const char* sqStr;
            switch (static_cast<uint8_t>(piece))
            {
                case static_cast<uint8_t>(Piece::Pawn)  : sqStr = "\u2659"; break;
                case static_cast<uint8_t>(Piece::Queen) : sqStr = "\u2655"; break;
                case static_cast<uint8_t>(Piece::Bishop): sqStr = "\u2657"; break;
                case static_cast<uint8_t>(Piece::Rook)  : sqStr = "\u2656"; break;
                case static_cast<uint8_t>(Piece::Knight): sqStr = "\u2658"; break;
                case static_cast<uint8_t>(Piece::King)  : sqStr = "\u2654"; break;
                case static_cast<uint8_t>(Piece::Stone) : sqStr = "x"     ; break;
                case static_cast<uint8_t>(Piece::Empty) : sqStr = "\033[2m.\033[0m"; break;
            }

            const char* colorCode;
            switch (static_cast<uint8_t>(color))
            {
                case static_cast<uint8_t>(Color::Red)   : colorCode = "\033[31m"; break;
                case static_cast<uint8_t>(Color::Blue)  : colorCode = "\033[34m"; break;
                case static_cast<uint8_t>(Color::Yellow): colorCode = "\033[33m"; break;
                case static_cast<uint8_t>(Color::Green) : colorCode = "\033[32m"; break;
                default: colorCode = ""; break;
            }
            std::cout << colorCode << sqStr << "\033[0m  ";
            }
            std::cout << "| " << (rank - start + 1) << "\n";
    }

    std::cout << "   +" << std::string((end - start) * 3 + 1, '-') << "+\n";
    std::cout << "     ";
    for (int file = start; file < end; ++file)
        std::cout << static_cast<char>('a' + (file - start)) << "  ";
    std::cout << "\n";

    std::cout << "\n" << std::left << std::setw(KEY_WIDTH) << "turn:";
    switch (static_cast<uint8_t>(turn()))
    {
        case Color::Red   : std::cout << "\033[1;31mRed\033[0m   " << "\n"; break;
        case Color::Blue  : std::cout << "\033[1;34mBlue\033[0m  " << "\n"; break;
        case Color::Yellow: std::cout << "\033[1;33mYellow\033[0m" << "\n"; break;
        case Color::Green : std::cout << "\033[1;32mGreen\033[0m " << "\n"; break;
        default           : std::cout << "unknown"                 << "\n"; break;
    }

    // std::string ks = "";
    // std::string qs = "";

    // ks += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::rKS)) ? "1" : "0"; ks += ',';      
    // ks += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::bKS)) ? "1" : "0"; ks += ',';
    // ks += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::yKS)) ? "1" : "0"; ks += ',';
    // ks += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::gKS)) ? "1" : "0";
    // qs += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::rQS)) ? "1" : "0"; qs += ',';
    // qs += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::bQS)) ? "1" : "0"; qs += ',';
    // qs += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::yQS)) ? "1" : "0"; qs += ',';
    // qs += (static_cast<uint8_t>(gs.castle) & static_cast<uint8_t>(Castle::gQS)) ? "1" : "0";

    std::string ks = gs.castle.uci(Side::KingSide );
    std::string qs = gs.castle.uci(Side::QueenSide);

    std::cout << std::left << std::setw(KEY_WIDTH) << "kingside:"  << ks << "\n"
              << std::left << std::setw(KEY_WIDTH) << "queenside:" << qs << "\n"
              << std::left << std::setw(KEY_WIDTH) << "fiftymove:" << static_cast<int>(gs.fiftymove) << "\n"
              << std::left << std::setw(KEY_WIDTH)
              << "enpassant:";


    for (Color color : {Color::Red, Color::Blue, Color::Yellow, Color::Green})
        std::cout << enpass(color).uci() << (color == Color::Green ? "" : ",");

    std::cout << "\n";
    std::cout << std::left << std::setw(KEY_WIDTH)
              << "royals:";

    for (Color color : {Color::Red, Color::Blue, Color::Yellow, Color::Green})
        std::cout << royal(color).uci() << (color == Color::Green ? "" : ",");

    std::cout << std::endl;
}

// template<Piece piece, Color color, Type type, Flag flag, Side side, Piece promo, Color enpass, Setup setup>
// void makemove(PS& ps, Move move)
// {
//     // const
//     // GS& curr = ps.states[ps.ply + 0];
//     // GS& next = ps.states[ps.ply + 1];

//     // const auto 
//     // source = move.source(),
//     // target = move.target();

//     // const auto 
//     // srcINDEX = 1ULL << source.index(),
//     // tgtINDEX = 1ULL << target.index();
    
//     // const auto 
//     // srcCHUNK = source.chunk(),
//     // tgtCHUNK = target.chunk();

//     // constexpr auto
//     // sourcePC = PC(PC::NOTHING),
//     // targetPC = PC(piece,color);

//     // if constexpr (flag == Flag::Noisy && type != Type::Enpass)
//     // {
//     //     const auto capture = ps.mailbox[target];
//     //     next.capture = capture;
//     //     ps.pieces[capture.piece()].chunks[tgtCHUNK] ^= tgtINDEX,
//     //     ps.colors[capture.color()].chunks[tgtCHUNK] ^= tgtINDEX;
//     // }

//     // else next.capture = PC::NOTHING;

//     // if constexpr (
//     // type == Type::Jumper ||
//     // type == Type::Slider ||
//     // type == Type::Pushed ||
//     // type == Type::Strike ||
//     // type == Type::Stride ||
//     // type == Type::Enpass)
//     // {
//     //     ps.mailbox[source] = sourcePC;
//     //     ps.mailbox[target] = targetPC;

//     //     ps.pieces[piece].chunks[srcCHUNK] ^= srcINDEX;
//     //     ps.pieces[piece].chunks[tgtCHUNK] ^= tgtINDEX;
//     //     ps.colors[color].chunks[srcCHUNK] ^= srcINDEX;
//     //     ps.colors[color].chunks[tgtCHUNK] ^= tgtINDEX;
//     // }

//     // if constexpr (type == Type::Enpass)
//     // {
//     //     const auto ep = target + DELTA_INFO[enpass].pushS;
//     //     const auto epIDX = 1ULL << ep.index();
//     //     const auto epCHK = ep.chunk();

//     //     constexpr auto epPIECE = Piece::P;
//     //     constexpr auto epCOLOR = enpass;

//     //     if constexpr (enpass == Color::r)
//     //     ps.pieces[epPIECE].chunks[1] ^= epIDX,
//     //     ps.colors[epCOLOR].chunks[1] ^= epIDX;

//     //     else if constexpr (enpass == Color::y)
//     //     ps.pieces[epPIECE].chunks[2] ^= epIDX,
//     //     ps.colors[epCOLOR].chunks[2] ^= epIDX;

//     //     else
//     //     ps.pieces[epPIECE].chunks[epCHK] ^= epIDX,
//     //     ps.colors[epCOLOR].chunks[epCHK] ^= epIDX;
//     // }
    
//     // if constexpr (type == Type::Evolve)
//     // {
//     //     constexpr auto evolvePC = PC(promo, color);
//     //     ps.mailbox[source] = sourcePC;
//     //     ps.mailbox[target] = evolvePC;

//     //     if constexpr (color == Color::r)
//     //     ps.pieces[piece].chunks[2] ^= srcINDEX,
//     //     ps.pieces[promo].chunks[2] ^= tgtINDEX,
//     //     ps.colors[color].chunks[2] ^= srcINDEX,
//     //     ps.colors[color].chunks[2] ^= tgtINDEX; 

//     //     else if constexpr (color == Color::y)
//     //     ps.pieces[piece].chunks[1] ^= srcINDEX,
//     //     ps.pieces[promo].chunks[1] ^= tgtINDEX,
//     //     ps.colors[color].chunks[1] ^= srcINDEX,
//     //     ps.colors[color].chunks[1] ^= tgtINDEX;

//     //     else
//     //     ps.pieces[piece].chunks[srcCHUNK] ^= srcINDEX,
//     //     ps.pieces[promo].chunks[tgtCHUNK] ^= tgtINDEX,
//     //     ps.colors[color].chunks[srcCHUNK] ^= srcINDEX,
//     //     ps.colors[color].chunks[tgtCHUNK] ^= tgtINDEX;
//     // }

//     // if constexpr (type == Type::Castle)
//     // {
//     //     constexpr auto 
//     //     rookSOURCE = CASTLE_INFO[color][side].rookSOURCE,
//     //     rookTARGET = CASTLE_INFO[color][side].rookTARGET,
//     //     kingSOURCE = CASTLE_INFO[color][side].kingSOURCE,
//     //     kingTARGET = CASTLE_INFO[color][side].kingTARGET;

//     //     constexpr auto 
//     //     rsIDX = 1ULL << rookSOURCE.index(),
//     //     rtIDX = 1ULL << rookTARGET.index(),
//     //     ksIDX = 1ULL << kingSOURCE.index(),
//     //     ktIDX = 1ULL << kingTARGET.index();

//     //     constexpr auto 
//     //     rsCHK = rookSOURCE.chunk(),
//     //     rtCHK = rookTARGET.chunk(),
//     //     ksCHK = kingSOURCE.chunk(),
//     //     ktCHK = kingTARGET.chunk();

//     //     constexpr auto
//     //     rsPC = PC(PC::NOTHING),
//     //     ksPC = PC(PC::NOTHING),
//     //     rtPC = PC(Piece::R, color),
//     //     ktPC = PC(Piece::K, color);

//     //     ps.mailbox[rookSOURCE] = rsPC;
//     //     ps.mailbox[rookTARGET] = rtPC;
//     //     ps.mailbox[kingSOURCE] = ksPC;
//     //     ps.mailbox[kingTARGET] = ktPC;

//     //     constexpr auto 
//     //     rPIECE = Piece::R,
//     //     kPIECE = Piece::K;

//     //     constexpr auto 
//     //     rCOLOR = color,
//     //     kCOLOR = color;

//     //     constexpr auto 
//     //     kIDX = (ksIDX ^ ktIDX),
//     //     rIDX = (rsIDX ^ rtIDX);

//     //     // --- King ---
//     //     if constexpr (ksCHK == ktCHK)
//     //     ps.pieces[kPIECE].chunks[ksCHK] ^= kIDX,
//     //     ps.colors[kCOLOR].chunks[ksCHK] ^= kIDX;
//     //     else
//     //     ps.pieces[kPIECE].chunks[ksCHK] ^= ksIDX,
//     //     ps.pieces[kPIECE].chunks[ktCHK] ^= ktIDX,
//     //     ps.colors[kCOLOR].chunks[ksCHK] ^= ksIDX,
//     //     ps.colors[kCOLOR].chunks[ktCHK] ^= ktIDX;
        
//     //     // --- Rook ---
//     //     if constexpr (rsCHK == rtCHK)
//     //     ps.pieces[rPIECE].chunks[rsCHK] ^= rIDX,
//     //     ps.colors[rCOLOR].chunks[rsCHK] ^= rIDX;
//     //     else
//     //     ps.pieces[rPIECE].chunks[rsCHK] ^= rsIDX,
//     //     ps.pieces[rPIECE].chunks[rtCHK] ^= rtIDX,
//     //     ps.colors[rCOLOR].chunks[rsCHK] ^= rsIDX,
//     //     ps.colors[rCOLOR].chunks[rtCHK] ^= rtIDX;
//     // }

//     // // // --- gs.rights ---
//     // // next.rights = curr.rights;
//     // // if constexpr (piece == Piece::K)
//     // // {
//     // //     constexpr u8 mask =
//     // //     color == Color::r ? static_cast<u8>(~RS::rKQ): 
//     // //     color == Color::r ? static_cast<u8>(~RS::bKQ): 
//     // //     color == Color::r ? static_cast<u8>(~RS::yKQ): 
//     // //     color == Color::r ? static_cast<u8>(~RS::gKQ): static_cast<u8>(~RS::RIGHTS);
//     // //     next.rights &= mask;
//     // // }

//     // // if constexpr (piece == Piece::R)
//     // // {
//     // //     if (UNLIKELY(srcINDEX & CASTLING_MASK))
//     // //     {
//     // //         constexpr auto
//     // //         team1 = color.team().encoded == Color::o ? Color::r : Color::b,
//     // //         team2 = color.team().encoded == Color::o ? Color::y : Color::g;

//     // //         constexpr RS
//     // //         mask1 = (team1 == Color::o ? static_cast<u8>(~RS::rKS) : static_cast<u8>(~RS::bKS)),
//     // //         mask2 = (team2 == Color::o ? static_cast<u8>(~RS::yQS) : static_cast<u8>(~RS::gQS));

//     // //         const SQ
//     // //         sq1 = CASTLE_INFO[ps.setup][team1][Side::KS].rookSOURCE,
//     // //         sq2 = CASTLE_INFO[ps.setup][team2][Side::QS].rookSOURCE;

//     // //         if (source.encoded == sq1) next.rights &= mask1;
//     // //         if (source.encoded == sq2) next.rights &= mask2;
//     // //     }
//     // // }

//     // // if constexpr (
//     // // flag == Flag::Noisy  && (
//     // // type == Type::Jumper ||
//     // // type == Type::Slider ||
//     // // type == Type::Strike))
//     // // {
//     // //     if (UNLIKELY(tgtINDEX & CASTLING_MASK))
//     // //     {
//     // //         constexpr auto
//     // //         enem1 = color.enem() == Color::o ? Color::r : Color::b,
//     // //         enem2 = color.enem() == Color::o ? Color::y : Color::g;

//     // //         constexpr RS
//     // //         mask1 = (enem1 == Color::o ? ~RS::rKS : ~RS::bKS),
//     // //         mask2 = (enem1 == Color::o ? ~RS::rQS : ~RS::bQS),
//     // //         mask3 = (enem1 == Color::o ? ~RS::yKS : ~RS::gKS),
//     // //         mask4 = (enem1 == Color::o ? ~RS::yQS : ~RS::gQS);

//     // //         constexpr SQ
//     // //         sq1 = CASTLE_INFO[ps.setup][enem1][KS].rookSOURCE,
//     // //         sq2 = CASTLE_INFO[ps.setup][enem1][QS].rookSOURCE,
//     // //         sq3 = CASTLE_INFO[ps.setup][enem2][KS].rookSOURCE,
//     // //         sq4 = CASTLE_INFO[ps.setup][enem2][QS].rookSOURCE;

//     // //         if (target == sq1) gs.rights &= mask1; else
//     // //         if (target == sq2) gs.rights &= mask2; else
//     // //         if (target == sq3) gs.rights &= mask3; else
//     // //         if (target == sq4) gs.rights &= mask4;
//     // //     }
//     // // }

//     // // --- gs.royals ---
//     // next.royals = curr.royals;
//     // if constexpr (piece == Piece::K) next.royals[color] = target;
//     // if constexpr (piece != Piece::K) {}

//     // // --- gs.enpass ---
//     // next.enpass = curr.enpass;
//     // if constexpr (type == Type::Stride) next.enpass[color] = source + DELTA_INFO[color].pushS;
//     // if constexpr (type != Type::Stride) next.enpass[color] = SQ::OFFBOARD;

//     // // --- gs.fiftymove ---
//     // if constexpr (piece == Piece::P || flag == Flag::Noisy || type == Type::Castle) next.fiftymove = 0;
//     // if constexpr (piece != Piece::P && flag != Flag::Noisy && type != Type::Castle) next.fiftymove = curr.fiftymove + 1;

//     // ps.ply++;
// }

// template<Piece piece, Color color, Type type, Flag flag, Side side, Piece promo, Color enpass, Setup setup>
// void undomove(PS& ps, Move move)
// {
//     // const GS& 
//     // prev = ps.states[ps.ply - 1],
//     // curr = ps.states[ps.ply + 0];

//     // const auto 
//     // source = move.source(),
//     // target = move.target();

//     // const auto
//     // srcINDEX = 1ULL << source.index(),
//     // tgtINDEX = 1ULL << target.index();
    
//     // const auto 
//     // srcCHUNK = source.chunk(),
//     // tgtCHUNK = target.chunk();

//     // constexpr auto
//     // sourcePC = PC(piece, color),
//     // targetPC = curr.capture;

//     // if constexpr (
//     // type == Type::Jumper ||
//     // type == Type::Slider ||
//     // type == Type::Pushed ||
//     // type == Type::Strike ||
//     // type == Type::Stride ||
//     // type == Type::Enpass)
//     // {
//     //     ps.mailbox[source] = sourcePC;
//     //     ps.mailbox[target] = targetPC;

//     //     ps.pieces[piece].chunks[srcCHUNK] ^= srcINDEX;
//     //     ps.pieces[piece].chunks[tgtCHUNK] ^= tgtINDEX;
//     //     ps.colors[color].chunks[srcCHUNK] ^= srcINDEX;
//     //     ps.colors[color].chunks[tgtCHUNK] ^= tgtINDEX;
//     // }

//     // if constexpr (type == Type::Enpass)
//     // {
//     //     const auto ep = target + DELTA_INFO[enpass].pushS;
//     //     const auto epIDX = 1ULL << ep.index();
//     //     const auto epCHK = ep.chunk();

//     //     constexpr auto epPIECE = Piece::P;
//     //     constexpr auto epCOLOR = enpass;

//     //     if constexpr (enpass == Color::r)
//     //     ps.pieces[epPIECE].chunks[1] ^= epIDX,
//     //     ps.colors[epCOLOR].chunks[1] ^= epIDX;

//     //     else if constexpr (enpass == Color::y)
//     //     ps.pieces[epPIECE].chunks[2] ^= epIDX,
//     //     ps.colors[epCOLOR].chunks[2] ^= epIDX;

//     //     else
//     //     ps.pieces[epPIECE].chunks[epCHK] ^= epIDX,
//     //     ps.colors[epCOLOR].chunks[epCHK] ^= epIDX;
//     // }

//     // if constexpr (type == Type::Evolve)
//     // {
//     //     constexpr auto evolvePC = PC(promo, color);
//     //     ps.mailbox[source] = sourcePC;
//     //     ps.mailbox[target] = targetPC;

//     //     if constexpr (color == Color::r)
//     //     ps.pieces[piece].chunks[2] ^= srcINDEX,
//     //     ps.pieces[promo].chunks[2] ^= tgtINDEX,
//     //     ps.colors[color].chunks[2] ^= srcINDEX,
//     //     ps.colors[color].chunks[2] ^= tgtINDEX; 

//     //     else if constexpr (color == Color::y)
//     //     ps.pieces[piece].chunks[1] ^= srcINDEX,
//     //     ps.pieces[promo].chunks[1] ^= tgtINDEX,
//     //     ps.colors[color].chunks[1] ^= srcINDEX,
//     //     ps.colors[color].chunks[1] ^= tgtINDEX;

//     //     else
//     //     ps.pieces[piece].chunks[srcCHUNK] ^= srcINDEX,
//     //     ps.pieces[promo].chunks[tgtCHUNK] ^= tgtINDEX,
//     //     ps.colors[color].chunks[srcCHUNK] ^= srcINDEX,
//     //     ps.colors[color].chunks[tgtCHUNK] ^= tgtINDEX;
//     // }

//     // if constexpr (type == Type::Castle)
//     // {
//     //     constexpr auto 
//     //     rookSOURCE = CASTLE_INFO[setup][color][side].rookSOURCE,
//     //     rookTARGET = CASTLE_INFO[setup][color][side].rookTARGET,
//     //     kingSOURCE = CASTLE_INFO[setup][color][side].kingSOURCE,
//     //     kingTARGET = CASTLE_INFO[setup][color][side].kingTARGET;

//     //     constexpr auto 
//     //     rsIDX = 1ULL << rookSOURCE.index(),
//     //     rtIDX = 1ULL << rookTARGET.index(),
//     //     ksIDX = 1ULL << kingSOURCE.index(),
//     //     ktIDX = 1ULL << kingTARGET.index();

//     //     constexpr auto 
//     //     rsCHK = rookSOURCE.chunk(),
//     //     rtCHK = rookTARGET.chunk(),
//     //     ksCHK = kingSOURCE.chunk(),
//     //     ktCHK = kingTARGET.chunk();

//     //     constexpr auto
//     //     rsPC = PC(Piece::R, color),
//     //     ksPC = PC(Piece::K, color),
//     //     rtPC = PC(PC::NOTHING),
//     //     ktPC = PC(PC::NOTHING);

//     //     ps.mailbox[rookSOURCE] = rsPC;
//     //     ps.mailbox[rookTARGET] = rtPC;
//     //     ps.mailbox[kingSOURCE] = ksPC;
//     //     ps.mailbox[kingTARGET] = ktPC;

//     //     constexpr auto 
//     //     rPIECE = Piece::R,
//     //     kPIECE = Piece::K;

//     //     constexpr auto 
//     //     rCOLOR = color,
//     //     kCOLOR = color;

//     //     constexpr auto 
//     //     kIDX = (ksIDX ^ ktIDX),
//     //     rIDX = (rsIDX ^ rtIDX);

//     //     // --- King ---
//     //     if constexpr (ksCHK == ktCHK)
//     //     ps.pieces[kPIECE].chunks[ksCHK] ^= kIDX,
//     //     ps.colors[kCOLOR].chunks[ksCHK] ^= kIDX;
//     //     else
//     //     ps.pieces[kPIECE].chunks[ksCHK] ^= ksIDX,
//     //     ps.pieces[kPIECE].chunks[ktCHK] ^= ktIDX,
//     //     ps.colors[kCOLOR].chunks[ksCHK] ^= ksIDX,
//     //     ps.colors[kCOLOR].chunks[ktCHK] ^= ktIDX;
        
//     //     // --- Rook ---
//     //     if constexpr (rsCHK == rtCHK)
//     //     ps.pieces[rPIECE].chunks[rsCHK] ^= rIDX,
//     //     ps.colors[rCOLOR].chunks[rsCHK] ^= rIDX;
//     //     else
//     //     ps.pieces[rPIECE].chunks[rsCHK] ^= rsIDX,
//     //     ps.pieces[rPIECE].chunks[rtCHK] ^= rtIDX,
//     //     ps.colors[rCOLOR].chunks[rsCHK] ^= rsIDX,
//     //     ps.colors[rCOLOR].chunks[rtCHK] ^= rtIDX;
//     // }

//     // ps.ply--;
// }

} // namespace athena