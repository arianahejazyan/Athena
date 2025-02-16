#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <sstream>
#include <array>
#include "bitboard.hpp"
#include "gamestate.hpp"

using namespace std;

namespace ELSA
{

static constexpr array<PieceType, 256> ASCII2PIECE = []
{
    array<PieceType, 256> arr = {};

    arr['P'] = WPawn  ; arr['K'] = WKing; arr['N'] = WKnight;
    arr['B'] = WBishop; arr['R'] = WRook; arr['Q'] = WQueen;
    arr['p'] = BPawn  ; arr['k'] = BKing; arr['n'] = BKnight;
    arr['b'] = BBishop; arr['r'] = BRook; arr['q'] = BQueen;

    return arr;
}();

static constexpr array<char, 13> PIECE2ASCII = [] 
{
    array<char, 13> arr = {};

    arr[WPawn]   = 'P'; arr[WKing] = 'K'; arr[WKnight] = 'N';
    arr[WBishop] = 'B'; arr[WRook] = 'R'; arr[WQueen]  = 'Q';
    arr[BPawn]   = 'p'; arr[BKing] = 'k'; arr[BKnight] = 'n';
    arr[BBishop] = 'b'; arr[BRook] = 'r'; arr[BQueen]  = 'q'; arr[none]  = ' ';

    return arr;
}();

static void initBoard(GameState& state, const string board)
{
    for (int sq = a1; sq <= h8; ++sq) state.mailbox[sq] = none;

    state.occupied[WPawn];   state.occupied[BPawn];
    state.occupied[WKnight]; state.occupied[BKnight];
    state.occupied[WBishop]; state.occupied[BBishop];
    state.occupied[WRook];   state.occupied[BRook];
    state.occupied[WQueen];  state.occupied[BQueen];
    state.occupied[WKing];   state.occupied[BKing];

    int rank = 7, file = 0, size = 0;

    for (const auto s: board)
    {
        if (size > BOARD_SIZE) throw invalid_argument("[BOARD]: overflow");

        if (s == '/') 
        {
            rank--; 
            file = 0;
        }

        else if ('1' <= s && s <= '8')
        {
            file += (s - '0');
            size += (s - '0');
        }

        else if (string("pknbrqPKNBRQK").find(s) != string::npos) 
        {
            Square sq = makeSquare(rank, file++);
            PieceType type = ASCII2PIECE[s];
            state.mailbox[sq] = type;
            state.occupied[type] = sq;
            size++;
        }

        else throw invalid_argument("[BOARD]: unknown piece");
    }
    
    if (size < BOARD_SIZE) throw invalid_argument("[BOARD]: underflow");
}

void initGameState(GameState& state, const string fen)
{
    stringstream ss(fen);
    string token;

    GameState newState;

    ss >> token; initBoard(newState,    token);

    state = newState;
}

void initGameState(GameState& state)
{
    initGameState(state, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void printGameState(const GameState& state)
{
    cout << "    a b c d e f g h\n";
    cout << "  +-----------------+\n";

    for (int rank = 7; rank >= 0; --rank)
    {
        cout << rank + 1 << " | ";

        for (int file = 0; file < 8; ++file)
            cout << PIECE2ASCII[state.mailbox[makeSquare(rank, file)]] << " ";

        cout << "| " << rank + 1 << "\n";
    }

    cout << "  +-----------------+\n";
    cout << "    a b c d e f g h\n\n";
}

}; // namespace

#endif