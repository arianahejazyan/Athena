#include <sstream>
#include "parser.h"
#include <iostream>

namespace parser
{

string remove_char(string str, char x)
{
    string result;

    for (char ch : str) 
    {
        if (ch != x) 
        {
            result += ch;
        }
    }

    return result;
}

char lower(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return ch + ('a' - 'A');
    }

    return ch;
}

string substring(string str, char start, char end)
{
    size_t l = str.find(start);
    size_t r = str.rfind(end);

    if (l != string::npos && r != string::npos)
    {
        return str.substr(l + 1, r - l -1);
    }

    return "";
}

vector<string> split_string(string str, char delimiter)
{
    vector<string> result;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter))
    {
        result.push_back(token);
    }

    if (!str.empty() && str.back() == delimiter)
    {
        result.push_back("");
    }

    return result;
}

vector<string> split_board(string board)
{
    vector<string> rows = split_string(board, '/'), output;

    for (auto row = rows.rbegin(); row != rows.rend(); ++row)
    {
        vector<string> cols = split_string(*row, ',');

        for (string col: cols)
        {
            output.push_back(col);
        }
    }

    return output;
}

vector<string> split_status(string status, char open, char close, char delimiter)
{
    vector<string> result;
    string slice;

    int depth = 0;
    for (char ch: status)
    {
        if (ch == open)
        {
            slice += ch;
            ++depth;
        }

        else if (ch == close)
        {
            slice += ch;
            --depth;
        }

        else if (ch == delimiter && depth == 0)
        {
            result.push_back(slice);
            slice.clear();
        }

        else slice += ch;
    }

    result.push_back(slice);

    return result;
}

bool parse_digit(int& digit, char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        digit = ch - '0';
    }

    else return false;

    return true;
}

bool parse_integer(int& integer, string str)
{
    if (str.empty())
    {
        return false;
    }

    int sign = +1, idx = 0;

    if (str[0] == '+') 
    {
        if (str.size() == 1)
        {
            return false;
        }

        else ++idx;
    } 

    else if (str[0] == '-') 
    {
        if (str.size() == 1)
        {
            return false;
        }
        
        else 
        {
            sign = -1;
            ++idx; 
        }
    }
  
    integer = 0;
    for (; idx< str.size(); ++idx)
    {
        int digit;

        if (parse_digit(digit, str[idx]))
        {
            integer = integer * 10 + digit;
        }

        else return false;
    }

    integer *= sign;

    return true;
}

bool parse_square(Square& sq, string loc)
{
    int row, col;

    if (loc.size() < 2)
    {
        return false;
    }

    char c = tolower(loc[0]);

    if (c >= 'a' && c <= 'n')
    {
        col = c - 'a';
    }

    else return false;

    if (parse_integer(row, loc.substr(1)))
    {
        row--;
    }

    else return false;

    if (valid_square(row, col))
    {
        sq = spot(row, col);
    }

    else return false;

    return true;
}

bool parse_piece(Piece& piece, char ch)
{
    switch (lower(ch))
    {
        case 'p': piece = Pawn;   break;
        case 'n': piece = Knight; break;
        case 'b': piece = Bishop; break;
        case 'r': piece = Rook;   break;
        case 'q': piece = Queen;  break;
        case 'k': piece = King;   break;

        default: return false;  
    }

    return true;
}

bool parse_player(Player& player, char ch)
{
    switch (lower(ch))
    {
        case 'r': player = Red;    break;
        case 'b': player = Blue;   break;
        case 'y': player = Yellow; break;
        case 'g': player = Green;  break;

        default: return false;
    }

    return true;
}

bool parse_turn(Position& pos, string turn)
{
    if (turn.length() == 0)
    {
        return true;
    } 
    
    switch (lower(turn[0]))
    {
        case 'r': pos.turn = Red;    break;
        case 'b': pos.turn = Blue;   break;
        case 'y': pos.turn = Yellow; break;
        case 'g': pos.turn = Green;  break;
        
        default: return false;
    }

    return true;
}

bool parse_state(Position& pos, string state)
{
    int player = 0;
    for (string part: split_string(state, ','))
    {
        if (player >= player_size) 
        {
            break;
        }

        if (part.size() == 0)
        {
            player++;

            continue;
        }

        switch (part[0])
        {
            case '1': pos.states[player++] = Alive; break;
            case '0': pos.states[player++] = Dead;  break;
        
            default: return false;
        }
    }

    return true;
}

bool parse_right(Position& pos, string right, Side side)
{
    int player = 0;
    for (string part: split_string(right, ','))
    {
        if (player >= player_size) 
        {
            break;
        }

        if (part.size() == 0)
        {
            player++;

            continue;
        }

        switch (part[0])
        {
            case '1': pos.rights[player++][side] = true;  break;
            case '0': pos.rights[player++][side] = false; break;
        
            default: return false;
        }
    }

    return true;
}

bool parse_score(Position& pos, string score)
{
    int player = 0;
    for (string part: split_string(score, ','))
    {
        if (player >= player_size) 
        {
            break;
        }

        if (part.size() == 0)
        {
            player++;

            continue;
        }

        int player_score;

        if (parse_integer(player_score, part))
        {
            pos.scores[player++] = player_score;
        }

        else return false;
    }

    return true;
}

bool parse_fifty_rule(Position& pos, string fifty_rule)
{
    int rule;

    if (parse_integer(rule, fifty_rule))
    {
        if (rule < 0)
        {
            return false;
        }

        else
        {
            pos.fifty_rule = rule;

            return true;            
        }
    }

    else return false;
}

bool parse_royal(Position& pos, string royal)
{
    int player = 0;

    for (string part: split_string(royal, ','))
    {
        if (player >= player_size) 
        {
            break;
        }

        if (parse_square(pos.royals[player], part))
        {
            player++;
        }

        else return false;
    }

    return true;
}

bool parse_enpassant(Position& pos, string enpassant)
{
    int player = 0;

    for (string part: split_string(enpassant, ','))
    {
        if (player >= player_size) 
        {
            break;
        }

        vector<string> components = split_string(part, ':');

        if (components.size() == 2)
        {
            bool r1 = parse_square(pos.marked[player], components[0]);
            bool r2 = parse_square(pos.target[player], components[1]);

            player++;

            if (!(r1 && r2)) return false;
        }

        else return false;
    }

    return true;




    /*
    vector<string> locations = split_string(substring(enpassant, '(', ')'), ',');

    if (locations.size() != 4)
    {
        return false;
    } 

    for (Player p = Red; p <= Green; ++p)
    //for (int p = 0; p <= 3; ++p)
    {
        vector<string> parts = split_string(substring(locations[p], '\'', '\''), ':');

        if (parts.empty())
        {
            pos.marked[p] = offboard;
            pos.target[p] = offboard;    
        }

        else if (parts.size() == 2)
        {
            bool r1 = parse_square(pos.marked[p], parts[0]);
            bool r2 = parse_square(pos.target[p], parts[1]);

            if (!(r1 && r2)) return false;
        }

        else return false;
    }

    return true;*/
}

bool parse_status(Position& pos, string status)
{
    for (char ch: {'{', '}', '\''})
    {
        status = remove_char(status, ch);
    }

    for (string statusx: split_status(status, '(', ')', ','))
    {
        vector<string> components = split_status(statusx, '(', ')', ':');

        for (char ch: {'(', ')'})
        {
            components[1] = remove_char(components[1], ch);
        }

        // EXPECT_TRUE(parse_status(pos, "r:(g9,,),e:(,c4:c5,,,)"));

        if (components.size() != 0)
        {
            switch (lower(components[0][0]))
            {
                case 'r':
                {
                    if (!parse_royal(pos, components[1]))
                    {
                        return false;
                    }

                    break;
                }

                case 'e':
                {
                    if (!parse_enpassant(pos, components[1]))
                    {
                        return false;
                    }

                    break;
                }

            
                default: return false;
            }
        }

        else return false;
    }

    return true;
}



bool parse_board(Position& pos, string board)
{
    vector<string> parts = split_board(board);

    unsigned int count = 0;
    for (const string& part: parts)
    {
        int val;

        if (parse_integer(val, part))
        {
            count += val;
        }

        else if (part.size() >= 2)
        {
            Piece piece;
            Player player;

            if (!parse_player(player, part[0])) continue;
            if (!parse_piece(piece, part[1])) continue;
    
            pos.place(count++, piece, player);
        }

        if (count >= board_size) break;
    }

    return count == board_size;
}

bool parse_fen(Position& pos, string fen)
{
    vector<string> parts = split_string(fen, '-');

    if (parts.size() != 7 && parts.size() != 8)
    {
        return false;
    } 

    if (!parse_turn(pos, parts[0]))
    {
        return false;
    }

    if (!parse_state(pos, parts[1]))
    {
        return false;
    }

    if (!parse_right(pos, parts[2], KingSide))
    {
        return false;
    }

    if (!parse_right(pos, parts[3], QueenSide))
    {
        return false;
    }

    if (!parse_score(pos, parts[4]))
    {
        return false;
    }

    if (!parse_fifty_rule(pos, parts[5]))
    {
        return false;
    }

    if (parts.size() == 8)
    {   
        if (!parse_status(pos, parts[6]))
        {
            return false;
        }
    }

    if (!parse_board(pos, parts.back()))
    {
        return false;
    }

    return true;
}

}; // namespace