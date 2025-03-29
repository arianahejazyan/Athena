#include "uci.h"

namespace Talia
{

void handleUCI()
{
    cout << "id name Talia-2PC" << endl;
    cout << "id author Ariana Hejazyan" << endl;
    cout << "uciok" << endl;
}

void handleIsReady()
{
    cout << "readyok" << endl;
}

void handlePosition(Position& pos, const std::vector<std::string> args)
{
    if (args.empty())
        throw invalid_argument("[POSITION]: missing args, expected 'startpos' or 'fen'");

    if (args[0] != "startpos" && args[0] != "fen")
        throw std::invalid_argument("[POSITION]: invalid args '" + args[0] + "', expected 'startpos' or 'fen'");

    if (args[0] == "fen")
    {
        if (args.size() < 7)
            throw std::invalid_argument("[FEN]: insufficient args for complete FEN");

        std::string fen = "";

        fen += args[1] + " ";
        fen += args[2] + " ";
        fen += args[3] + " ";
        fen += args[4] + " ";
        fen += args[5] + " ";
        fen += args[6];

        if (!fenIsValid(fen))
            throw std::invalid_argument("[FEN]: invalid args for FEN");

        else pos = fenToPosition(fen);
    }

    else pos = fenToPosition(startpos);

//     // if (token == "moves")
//     //     while (!ss.eof()) handleMakemove(temp, ss, token);

//     // size_t movesIndex = (token == "fen") ? 7 : 1;
//     // if (args.size() > movesIndex && args[movesIndex] == "moves") {
//     //     for (size_t i = movesIndex + 1; i < args.size(); i++) {
//     //         // Pass each move directly to handleMakemove
//     //         handleMakemove(temp, args[i]);
//     //     }
//     // }
}

[[nodiscard]] std::string moveToUCI(const Move move) noexcept
{
    std::string uci_move;

    const auto source = move.source();
    const auto target = move.target();
    const auto nature = move.nature();

    uci_move += ('a' + (fileSQ(source)));
    uci_move += ('1' + (rankSQ(source)));
    uci_move += ('a' + (fileSQ(target)));
    uci_move += ('1' + (rankSQ(target)));

    if (nature == FLAG_QUEEN)  uci_move += 'q';
    if (nature == FLAG_ROOK)   uci_move += 'r';
    if (nature == FLAG_BISHOP) uci_move += 'b';
    if (nature == FLAG_KNIGHT) uci_move += 'n';

    return uci_move;
}

} // namespace