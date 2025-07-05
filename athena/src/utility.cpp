#include "utility.h"
#include "zobrist.h"

namespace athena
{

std::string toString(const Position& pos)
{
    const GameState& gs = pos.states.back();

    std::string tn = ""; 
    tn += toString(pos.setup) + " ";
    tn += toString(gs.turn)   + " ";
    tn += toString(gs.clock)  + " "; 
    tn += toString(gs.castle, pos.setup, KingSide)  + " ";
    tn += toString(gs.castle, pos.setup, QueenSide) + " ";
    tn += toString(gs.enpass) + " ";

    int counter = 0;
    for (auto sq: VALID_SQUARES)
    {
        const auto& pc = pos.board[sq];
        if (pc == EMPTY)
        {
            ++counter;
            continue;
        }
        
        if (counter > 0)
        {
            tn += std::to_string(counter) + ",";
            counter = 0;
        }

        else tn += toString(pc) + ",";
    }

    if (counter > 0)
        tn += std::to_string(counter);

    if (tn.back() == ',')
        tn.pop_back();

    return tn;
}

void fromString(const std::string& str, Position& pos)
{
    pos.states.clear();
    pos.states.reserve(256);

    // <GameSetup> <Turn> <Clock> <KingSide> <QueenSide> <Enpassant> <Board>
    auto tokens = tokenize(str, ' ');

    // Parsing <GameSetup>
    fromString(tokens[0], pos.setup);

    // Parsing <Turn>
    Color turn;
    fromString(tokens[1], turn);

    // Parsing <Clock>
    int clock;
    fromString(tokens[2], clock);

    // Parsing <KingSide> <QueenSide>
    BitBoard castle;
    fromString(tokens[3], castle, pos.setup, KingSide);
    fromString(tokens[4], castle, pos.setup, QueenSide);

    // Parsing <Enpassant>
    ndarray<Square, COLOR_NB - 1> enpass;
    fromString(tokens[5], enpass);

    // Parsing <Board>
    auto board = tokenize(tokens[6], ',');
    pos.board.clear();
    int idx = 0;
    for (auto sq : board)
    {
        if (!std::isdigit(sq[0]))
        {
            PieceClass pc;
            fromString(sq, pc);
            pos.board.setSQ(VALID_SQUARES[idx++], pc);
        }
        // else idx += std::stoi(sq);
        else {
            int skip = std::stoi(sq);
            for (int i = 0; i < skip; ++i)
                pos.board.setSQ(VALID_SQUARES[idx++], EMPTY);
        }
    }

    pos.states.emplace_back(clock, turn, BB(), castle, EMPTY, enpass);
}

} // namespace athena