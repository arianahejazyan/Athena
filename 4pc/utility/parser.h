#include "definitions.h"

using namespace definitions;

namespace parser
{
    char lower(char ch);

    string substring(string str, char start, char end);

    vector<string> split_string(string fen, char delimiter);

    vector<string> split_board(string board);

    vector<string> split_status(string status, char open, char close, char delimiter);

    bool parse_digit(int& digit, char ch);

    bool parse_integer(int& integer, string str);

    bool parse_square(Square& sq, string loc);

    bool parse_piece(Piece& piece, char ch);

    bool parse_player(Player& player, char ch);

    bool parse_turn(Position& pos, string turn);

    bool parse_state(Position& pos, string state);

    bool parse_right(Position& pos, string right, Side side);

    bool parse_score(Position& pos, string score);

    bool parse_fifty_rule(Position& pos, string fifty_rule);

    bool parse_royal(Position& pos, string royal);

    bool parse_enpassant(Position& pos, string enpassant);

    bool parse_status(Position& pos, string status);
    
    string remove_char(string str, char x);

    bool parse_board(Position& pos, string board);

    bool parse_fen(Position& pos, string fen);
};