#include <iostream>
#include <sstream>
#include <array>
#include "bitboards.h"
#include "gamestate.h"
#include "utility.h"
#include "zobrist.h"

using namespace std;

namespace ELSA
{

static constexpr array<Piece, 256> ASCII_2_PIECE = []
{
    array<Piece, 256> arr = {};

    arr['P'] = W_PAWN  ; arr['K'] = W_KING; arr['N'] = W_KNIGHT;
    arr['B'] = W_BISHOP; arr['R'] = W_ROOK; arr['Q'] = W_QUEEN;
    arr['p'] = B_PAWN  ; arr['k'] = B_KING; arr['n'] = B_KNIGHT;
    arr['b'] = B_BISHOP; arr['r'] = B_ROOK; arr['q'] = B_QUEEN;

    return arr;
}();

static constexpr array<char, 13> PIECE_2_ASCII = [] 
{
    array<char, 13> arr = {};

    arr[W_PAWN]   = 'P'; arr[W_KING] = 'K'; arr[W_KNIGHT] = 'N';
    arr[W_BISHOP] = 'B'; arr[W_ROOK] = 'R'; arr[W_QUEEN]  = 'Q';
    arr[B_PAWN]   = 'p'; arr[B_KING] = 'k'; arr[B_KNIGHT] = 'n';
    arr[B_BISHOP] = 'b'; arr[B_ROOK] = 'r'; arr[B_QUEEN]  = 'q'; arr[NO_PIECE] = ' ';

    return arr;
}();

static void initStateHash(GameState& state, Info& si)
{
    for (int s = A1; s <= H8; s++)
        if (state.at(static_cast<Square>(s)) != NO_PIECE) si.hash ^= Zobrist[s][state.at(static_cast<Square>(s))];

    si.hash ^= ZobristCastle[si.castle];

    if (si.epsq != OFFBOARD) si.hash ^= ZobristEnpass[getFile(si.epsq)];
    if (si.turn == BLACK)    si.hash ^= ZobristTurn;
}

static void initSquares(GameState& state, const string board)
{
    int rank = 7, file = 0;

    for (auto s: board)
    {
        if (s == '/') rank--, file = 0;

        else if ('1' <= s && s <= '8') file += (s - '0');

        else state.setPiece(makeSquare(rank, file++), ASCII_2_PIECE[s]);
    }
}

static void initTurn(Info& si, const string turn)
{
    si.turn = turn == "w" ? WHITE : BLACK;
}

static void initCastle(Info& si, const string castle)
{
    for (char c: castle)
    {
        switch (c)
        {
            case 'K': si.castle = (CastlingRights)(si.castle | (WHITESIDE & KINGSIDE));  break;
            case 'Q': si.castle = (CastlingRights)(si.castle | (WHITESIDE & QUEENSIDE)); break;
            case 'k': si.castle = (CastlingRights)(si.castle | (BLACKSIDE & KINGSIDE));  break;
            case 'q': si.castle = (CastlingRights)(si.castle | (BLACKSIDE & QUEENSIDE)); break;
        }
    }
}

static void initEpsq(Info& si, const string epsq)
{
    if (epsq != "-" )
        si.epsq = makeSquare(epsq[1] - '1', epsq[0] - 'a');

    else si.epsq = OFFBOARD;
}

static void initHalfmove(Info& si, const string halfmove)
{
    for (char d: halfmove)
            si.halfmove = si.halfmove * 10 + (d - '0');
}

static void initFullmove(Info& si, const string fullmove)
{
    for (char d: fullmove)
            si.fullmove = si.fullmove * 10 + (d - '0');
}

void initGameState(GameState& state, const string fen)
{
    state.resetGameState();

    stringstream ss(fen);
    string token;

    Info& si = state.stateInfo();

    ss >> token; initSquares(state, token);
    ss >> token; initTurn(si, token);
    ss >> token; initCastle(si, token);
    ss >> token; initEpsq(si, token);
    ss >> token; initHalfmove(si, token);
    ss >> token; initFullmove(si, token);

    initStateHash(state, si);
}

void printGameState(const GameState& state)
{
    Info& si = state.stateInfo();

    cout << "    a b c d e f g h\n";
    cout << "  +-----------------+\n";

    for (int rank = 7; rank >= 0; --rank)
    {
        cout << rank + 1 << " | ";

        for (int file = 0; file < 8; ++file)
            cout << PIECE_2_ASCII[state.at(makeSquare(rank, file))] << " ";

        cout << "| " << rank + 1 << "\n";
    }

    cout << "  +-----------------+\n";
    cout << "    a b c d e f g h\n\n";

    cout << "turn: " << (si.turn == WHITE ? "white" : "black") << endl;

    if (si.castle != 0)
    {
        std::cout << "castle: ";

        if ((si.castle & (WHITESIDE & KINGSIDE)) ) std::cout << 'K';
        if ((si.castle & (WHITESIDE & QUEENSIDE))) std::cout << "Q";
        if ((si.castle & (BLACKSIDE & KINGSIDE)) ) std::cout << "k";
        if ((si.castle & (BLACKSIDE & QUEENSIDE))) std::cout << "q";

        std::cout << std::endl;
    }

    else std::cout << "castle: -" << std::endl;

    if (si.epsq == OFFBOARD)
        std::cout << "epsq: -" << std::endl;

    else std::cout << "epsq: " << (char)(getFile(si.epsq) + 'a') << (char)(getRank(si.epsq) + '1') << std::endl;

    std::cout << "halfmove: " << si.halfmove << std::endl;
    std::cout << "fullmove: " << si.fullmove << std::endl;
}

}; // namespace