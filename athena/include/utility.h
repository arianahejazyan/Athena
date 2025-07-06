#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "bitboard.h"
#include "position.h"

namespace athena
{

inline constexpr Color COLOR_TABLE[26] = 
{
    /* a */ None,    /* b */ Blue,
    /* c */ None,    /* d */ None,
    /* e */ None,    /* f */ None,
    /* g */ Green,   /* h */ None,
    /* i */ None,    /* j */ None,
    /* k */ None,    /* l */ None,
    /* m */ None,    /* n */ None,
    /* o */ None,    /* p */ None,
    /* q */ None,    /* r */ Red,
    /* s */ None,    /* t */ None,
    /* u */ None,    /* v */ None,
    /* w */ None,    /* x */ None,
    /* y */ Yellow,  /* z */ None
};

inline constexpr Piece PIECE_TABLE[26] =
{
    /* a */ Empty,    /* b */ Bishop,
    /* c */ Empty,    /* d */ Empty,
    /* e */ Empty,    /* f */ Empty,
    /* g */ Empty,    /* h */ Empty,
    /* i */ Empty,    /* j */ Empty,
    /* k */ King,     /* l */ Empty,
    /* m */ Empty,    /* n */ Knight,
    /* o */ Empty,    /* p */ Pawn,
    /* q */ Queen,    /* r */ Rook,
    /* s */ Empty,    /* t */ Empty,
    /* u */ Empty,    /* v */ Empty,
    /* w */ Empty,    /* x */ Empty,
    /* y */ Empty,    /* z */ Empty
};

inline auto tokenize(const std::string& line, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, delimiter))
        if (!token.empty())
            tokens.push_back(token);
    return tokens;
}

inline auto concatenate(const std::vector<std::string>& tokens, size_t start, size_t end, char delimiter)
{
    std::string result = tokens[0];
    for (size_t i = start + 1; i < end; ++i) 
    {
        result += delimiter;
        result += tokens[i];
    }
    return result;
}

void fromString(const std::string& str, Position& pos);
std::string toString(const Position& pos);

inline void fromString(const std::string& str, Square& sq)
{
    if (str != "-")
    {
        int file = std::tolower(str[0]) - 'a';
        int rank = std::stoi(str.substr(1)) - 1;
        sq = static_cast<Square>(rank * FILE_NB + file);
    }
    else sq = OFFBOARD;
}

inline void fromString(const std::string& str, GameSetup& setup) {
    setup = (str == "classic") ? Classic : Modern;
}

inline void fromString(const std::string& str, Color& color) noexcept {
    color = COLOR_TABLE[std::tolower(str[0]) - 'a'];
}

inline void fromString(const std::string& str, Piece& piece) noexcept {
    piece = PIECE_TABLE[std::tolower(str[0]) - 'a'];
}

inline void fromString(const std::string& str, PieceClass& pc)
{
    auto piece = PIECE_TABLE[std::tolower(str[1]) - 'a'];
    auto color = COLOR_TABLE[std::tolower(str[0]) - 'a'];
    pc = PieceClass(piece, color);
}

inline void fromString(const std::string& str, BitBoard& castle, GameSetup setup, Side side)
{
    for (auto color: COLORS)
        if (str[color] == '1') castle |= RIGHTS[setup][color][side];
}

inline void fromString(const std::string& str, int& clock) {
    clock = std::stoi(str);
}

inline void fromString(const std::string& str, ndarray<Square, COLOR_NB - 1>& enpass)
{
    auto tokens = tokenize(str, ',');
    for (auto color: COLORS)
        fromString(tokens[color], enpass[color]);
}

inline std::string toString(Piece piece)
{
    switch (piece)
    {
        case Pawn:   return "p";
        case Knight: return "n";
        case Bishop: return "b";
        case Rook:   return "r";
        case Queen:  return "q";
        case King:   return "k";
        case Empty:  return " ";
        case Stone:  return "x";
        default:     return "?";
    }
}

inline std::string toString(Color color)
{
    switch (color)
    {
        case Red:    return "r";
        case Blue:   return "b";
        case Yellow: return "y";
        case Green:  return "g";
        case None:   return " ";
        default:     return "?";
    }
}

inline std::string toString(PieceClass pc)
{
    std::string str = "";
    str += toString(pc.color());
    str += toString(pc.piece());
    return str;
}

inline std::string toString(Square sq)
{
    if (sq == OFFBOARD) return "-";
    return std::string(1, 'a' + fileSQ(sq)) + std::to_string(rankSQ(sq) + 1);
}

inline std::string toString(Move move)
{
    std::string str = "";
    str += toString(move.source());
    str += toString(move.target());
    if (move.nature() == Evolve) str += toString(move.evolve().piece());
    return str;
}

inline std::string toString(int clock) noexcept {
    return std::to_string(clock);
}

inline std::string toString(GameSetup setup) noexcept {
    return (setup == Classic) ? "classic" : "modern";
}

inline std::string toString(const BitBoard& castle, GameSetup setup, Side side) noexcept
{
    std::string str = "";
    for (auto color: COLORS)
        str += (hasCastle(castle, RIGHTS[setup][color][side]) ? "1" : "0");
    return str;
}

inline std::string toString(const ndarray<Square, COLOR_NB - 1>& enpass) noexcept
{
    std::string str = "";
    for (auto color: COLORS)
        str += toString(enpass[color]) + (color != Green ? "," : "");
    return str;
}

inline void print(const BitBoard& bb)
{
    std::cout << "\n    ";
    for (int f = 0; f < 16; ++f)
        std::cout << static_cast<char>('a' + f) << ' ';
    std::cout << "\n";

    for (int r = 15; r >= 0; --r)
    {
        std::cout << (r + 1 < 10 ? " " : "") << r + 1 << "  ";
        for (int f = 0; f < 16; ++f)
        {
            Square sq = makeSQ(r, f);
            bool valid = isValidSquare(r, f);
            bool set = bb.checkSQ(sq);
            if (set) std::cout << (valid ? "\033[34mX\033[0m" : "\033[90mX\033[0m");
            else     std::cout << (valid ? "." : "\033[90m.\033[0m");
            if (f < 15) std::cout << ' ';
        }
        std::cout << "  " << (r + 1 < 10 ? " " : "") << r + 1 << '\n';
    }

    std::cout << "    ";
    for (int f = 0; f < 16; ++f)
        std::cout << static_cast<char>('a' + f) << ' ';
    std::cout << "\n\n";
}

inline void print(const Position& pos, bool print_fen, bool use_ascii_pieces)
{
    const GameState& gs = pos.states.back();
    constexpr int BOARD_SIZE = 16;

    std::cout << "\n     ";
    for (int file = 0; file < BOARD_SIZE; ++file)
        std::cout << static_cast<char>('a' + file) << "  ";

    std::cout << "\n   +" << std::string(BOARD_SIZE * 3 + 1, '-') << "+\n";

    for (int rank = BOARD_SIZE - 1; rank >= 0; --rank) {
        std::cout << ((rank + 1) < 10 ? " " : "") << (rank + 1) << " | ";

        for (int file = 0; file < BOARD_SIZE; ++file) {
            Square sq = makeSQ(rank, file);
            PieceClass pc = pos.board[sq];
            Piece piece = pc.piece();
            Color color = pc.color();

            std::string sqStr;

            switch (piece) {
                case Piece::Empty:
                    sqStr = "\033[2m.\033[0m"; // Dim dot always
                    break;
                case Piece::Stone:
                    sqStr = "x";
                    break;
                case Piece::Pawn:
                    sqStr = use_ascii_pieces ? "P" : "\u2659"; // ♙
                    break;
                case Piece::Knight:
                    sqStr = use_ascii_pieces ? "N" : "\u2658"; // ♘
                    break;
                case Piece::Bishop:
                    sqStr = use_ascii_pieces ? "B" : "\u2657"; // ♗
                    break;
                case Piece::Rook:
                    sqStr = use_ascii_pieces ? "R" : "\u2656"; // ♖
                    break;
                case Piece::Queen:
                    sqStr = use_ascii_pieces ? "Q" : "\u2655"; // ♕
                    break;
                case Piece::King:
                    sqStr = use_ascii_pieces ? "K" : "\u2654"; // ♔
                    break;
            }

            std::string colorCode;
            switch (color) {
                case Color::Red:    colorCode = "\033[31m"; break;
                case Color::Blue:   colorCode = "\033[34m"; break;
                case Color::Yellow: colorCode = "\033[33m"; break;
                case Color::Green:  colorCode = "\033[32m"; break;
                default:            colorCode = "";         break;
            }

            std::cout << colorCode << sqStr << "\033[0m  ";
        }

        std::cout << "| " << (rank + 1) << "\n";
    }

    std::cout << "   +" << std::string(BOARD_SIZE * 3 + 1, '-') << "+\n";

    std::cout << "     ";
    for (int file = 0; file < BOARD_SIZE; ++file)
        std::cout << static_cast<char>('a' + file) << "  ";
    std::cout << std::endl;

    constexpr int KEY_WIDTH = 12;
    std::cout << std::endl;

    // Turn
    std::cout << std::left << std::setw(KEY_WIDTH) << "Turn:";
    switch (gs.turn) {
        case Red:    std::cout << "\033[1;31mRed\033[0m"; break;
        case Blue:   std::cout << "\033[1;34mBlue\033[0m"; break;
        case Yellow: std::cout << "\033[1;33mYellow\033[0m"; break;
        case Green:  std::cout << "\033[1;32mGreen\033[0m"; break;
        default:     std::cout << "unknown"; break;
    }
    std::cout << std::endl;

    // Setup
    std::cout << std::left << std::setw(KEY_WIDTH) << "Setup:" << toString(pos.setup) << std::endl;

    // Clock
    std::cout << std::left << std::setw(KEY_WIDTH) << "Clock:" << toString(gs.clock) << std::endl;

    // Castling
    std::cout << std::left << std::setw(KEY_WIDTH) << "KingSide:" << toString(gs.castle, pos.setup, KingSide) << std::endl;
    std::cout << std::left << std::setw(KEY_WIDTH) << "QueenSide:" << toString(gs.castle, pos.setup, QueenSide) << std::endl;

    // En Passant
    std::cout << std::left << std::setw(KEY_WIDTH) << "Enpassant:";
    for (auto color : COLORS) {
        std::cout << toString(gs.enpass[color]);
        if (color != Green) std::cout << ",";
    }
    std::cout << std::endl;

    // FEN
    if (print_fen)
        std::cout << std::left << std::setw(KEY_WIDTH) << "FEN:" << toString(pos) << std::endl << std::endl;
}


} // namespace athena

#endif // #ifndef UTILITY.H