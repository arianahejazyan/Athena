#include "fen.h"

namespace Talia
{

[[nodiscard]] char toASCII(const PieceType type)
{
    switch (type)
    {
        case NO_PIECE_TYPE: return ' ';

        case W_PAWN:   return 'P'; case B_PAWN:   return 'p';
        case W_KING:   return 'K'; case B_KING:   return 'k';
        case W_KNIGHT: return 'N'; case B_KNIGHT: return 'n';
        case W_BISHOP: return 'B'; case B_BISHOP: return 'b';
        case W_ROOK:   return 'R'; case B_ROOK:   return 'r';
        case W_QUEEN:  return 'Q'; case B_QUEEN:  return 'q';

        default: throw invalid_argument("invalid piece type");
    }
}

[[nodiscard]] bool fenIsValid(const string fen)
{
    stringstream ss(fen);

    string board;    if (!(ss >> board))    return false;
    string turn;     if (!(ss >> turn))     return false;
    string castle;   if (!(ss >> castle))   return false;
    string enpass;   if (!(ss >> enpass))   return false;
    string halfmove; if (!(ss >> halfmove)) return false;
    string fullmove; if (!(ss >> fullmove)) return false;

    string token;
    if (ss >> token) return false;

    int rank = 7;
    int file = 0;
    for (const auto c : board)
    {            
        if ('1' <= c && c <= '8')
        {
            file += (c - '0');
        }
    
        else if (c == '/')
        {
            if (file != 8) return false;

            rank--;
            file = 0;
        }

        else if (ASCII_TO_PIECE[c] == NO_PIECE_TYPE) return false;

        else file++;

        if (file > 8) return false;
    }

    if (rank != 0 || file != 8) return false;

    bool cond1 = turn == "w";
    bool cond2 = turn == "b";

    if (!(cond1 || cond2)) return false;

    if (castle != "-") 
        for (const auto c : castle)
            if (ASCII_TO_CASTLING[c] == NO_CASTLE) return false;

    if (enpass != "-")
    {
        bool cond1 = enpass.size() == 2;
        bool cond2 = 'a' <= enpass[0] && enpass[0] <= 'h';
        bool cond3 = '1' <= enpass[1] && enpass[1] <= '8';

        if (!(cond1 && cond2 && cond3)) return false;
    }

    for (const auto c : halfmove)
        if (!isdigit(c)) return false;

    for (const auto c : fullmove)
        if (!isdigit(c)) return false;

    return true;
}

Position fenToPosition(const string fen)
{
    assert(fenIsValid(fen));

    stringstream ss(fen);

    string board;    ss >> board;
    string turn;     ss >> turn;
    string castle;   ss >> castle;
    string enpass;   ss >> enpass;
    string halfmove; ss >> halfmove;
    string fullmove; ss >> fullmove;

    Position pos;
    GameState& state = pos.state();

    pos.board.setAllZero();

    int rank = 7;
    int file = 0;
    for (const auto c: board)
    {
        if ('1' <= c && c <= '8') 
        {
            file += (c - '0');
        }

        else if (c == '/')
        {
            rank--;
            file = 0;
        }

        else pos.board.setSQ(makeSquare(rank, file++), ASCII_TO_PIECE[c]);
    }

    pos.turn = turn == "w" ? WHITE : BLACK;

    state.castle = NO_CASTLE;
    for (const auto c: castle)
        setbb(state.castle, ASCII_TO_CASTLING[c]);

    if (enpass != "-" )
            state.epsq = makeSquare(enpass[1] - '1', enpass[0] - 'a');
    else state.epsq = OFFBOARD;

    state.halfmove = 0;
    for (const auto d: halfmove)
        state.halfmove = state.halfmove * 10 + (d - '0');

    state.fullmove = 0;
    for (const auto d: fullmove)
        state.fullmove = state.fullmove * 10 + (d - '0');

    return pos;
}

string fenFromPosition(const Position& pos)
{
    string fen = "";

    return fen;
}

} // namespace