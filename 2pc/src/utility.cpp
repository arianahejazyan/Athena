#include "utility.h"

namespace Talia
{

std::string formatNumber(uint64_t num)
{
    std::string numStr = std::to_string(num);

    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }

    return numStr;
}

std::vector<std::string> splitString(const std::string& input)
{
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) 
        result.push_back(token);

    return result;
}

void printPosition(const Position& pos)
{
    const GameState& state = pos.state();

    std::cout << "    a b c d e f g h\n";
    std::cout << "  +-----------------+\n";

    for (int rank = 7; rank >= 0; --rank)
    {
        std::cout << rank + 1 << " | ";

        for (int file = 0; file < 8; ++file)
            std::cout << toASCII(pos.board.at(makeSquare(rank, file))) << " ";
            
        std::cout << "| " << rank + 1 << "\n";
    }

    std::cout << "  +-----------------+\n";
    std::cout << "    a b c d e f g h\n\n";

    std::cout << "turn: " << (pos.turn == WHITE ? "white" : "black") << std::endl;

    if (state.castle != NO_CASTLE)
    {
        std::cout << "castle: ";

        if (hasCastlingRight(state.castle, WHITESIDE, KINGSIDE))  std::cout << 'K';
        if (hasCastlingRight(state.castle, WHITESIDE, QUEENSIDE)) std::cout << "Q";
        if (hasCastlingRight(state.castle, BLACKSIDE, KINGSIDE))  std::cout << "k";
        if (hasCastlingRight(state.castle, BLACKSIDE, QUEENSIDE)) std::cout << "q";

        std::cout << std::endl;
    }

    else std::cout << "castle: -" << std::endl; 

    if (state.epsq == OFFBOARD)
         std::cout << "epsq: -" << std::endl;
    else std::cout << "epsq: " << (char)(fileSQ(state.epsq) + 'a') << (char)(rankSQ(state.epsq) + '1') << std::endl;

    std::cout << "halfmove: " << state.halfmove << std::endl;
    std::cout << "fullmove: " << state.fullmove << std::endl;
    std::cout << "fen: " << fenFromPosition(pos) << std::endl;
}

} // namespace