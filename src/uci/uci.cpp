#include "uci/uci.h"
#include <sstream>
#include <vector>
#include "chess/position.h"
#include "uci/uci.h"
#include "misc.h"

namespace athena::uci {

UCIEngine::UCIEngine() {
    register_commands();
}

void UCIEngine::register_commands() {
    commands_["uci"]        = [this](std::istream& is) { uci(is); };
    commands_["isready"]    = [this](std::istream& is) { isready(is); };
    commands_["setoption"]  = [this](std::istream& is) { setoption(is); };
    commands_["ucinewgame"] = [this](std::istream& is) { ucinewgame(is); };
    commands_["position"]   = [this](std::istream& is) { position(is); };
    commands_["go"]         = [this](std::istream& is) { go(is); };
    commands_["stop"]       = [this](std::istream& is) { stop(is); };
    commands_["quit"]       = [this](std::istream& is) { quit(is); };
    commands_["perft"]      = [this](std::istream& is) { perft(is); };
    commands_["print"]      = [this](std::istream& is) { print(is); };
}

void UCIEngine::run(int argc, char** argv) {

    // Handle command-line arguments
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            std::cout
                << "Athena " << misc::version() << "\n"
                << "A free uci-compatible 4-player chess engine.\n"
                << "For full documentation, read the README.md file or visit the GitHub page: "
                << misc::github() 
                << std::endl;
        }
        else if (arg == "--version" || arg == "-v") {
            std::cout
                << "Athena " << misc::version()
                << std::endl;
        }
        else if (arg == "--license") {
            std::cout 
                << "Athena is free and open source software licensed under the MIT License.\n"
                << "See the LICENSE file for details."
                << std::endl;
        }
        return;
    }

    // UCI interactive loop
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream is(line);
        std::string cmd;
        is >> cmd;

        auto it = commands_.find(cmd);
        if (it != commands_.end()) {
            it->second(is);
        } else {
            std::cout << "Unknown command '" << line << "'\n";
        }
    }
}

void UCIEngine::uci(std::istream&) {
    std::cout << "id name Athena " << misc::version() << "\n";
    std::cout << "id author " << misc::author() << "\n";
    std::cout << "uciok" << std::endl;
}

void UCIEngine::isready(std::istream&) {
    std::cout << "readyok" << std::endl;
}

void UCIEngine::setoption(std::istream&) {
}

void UCIEngine::ucinewgame(std::istream&) {
}

void UCIEngine::position(std::istream& is) {

    std::string token, fen;
    is >> token;

    if (token == "fen") {
        is >> token;
        fen = token;
    } else {
        fen = chess::Position::startpos(token);
    }

    std::vector<std::string> moves;
    if (is >> token) {
        while (is >> token) moves.push_back(token);
    }

    engine_.set_position(fen, moves);
}

void UCIEngine::go(std::istream& is) {
    Search::Limits limits = parse_limits(is);
    engine_.go(limits);
}

void UCIEngine::stop(std::istream&) {
    engine_.stop();
}

void UCIEngine::quit(std::istream&) {
    engine_.stop();
}

void UCIEngine::perft(std::istream& is) {
    int depth;
    is >> depth;
    engine_.perft(depth);
}

void UCIEngine::print(std::istream&) {
    engine_.print(false);
}

Search::Limits UCIEngine::parse_limits(std::istream& is) {

    Search::Limits limits;
    std::string token;

    while (is >> token)
        if (token == "depth")
            is >> limits.maxDepth;
 
    return limits;
}

} // namespace athena