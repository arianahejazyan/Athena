#include <sstream>
#include "cli/cli.h"
#include "misc.h"

namespace athena::cli {

CLI::CLI() {
    registerCommands();
}

void CLI::registerCommands() {
    commands_["uci"]        = [this](std::istream& is) { uci(is); };
    commands_["isready"]    = [this](std::istream& is) { isready(is); };
    commands_["setoption"]  = [this](std::istream& is) { setoption(is); };
    commands_["ucinewgame"] = [this](std::istream& is) { ucinewgame(is); };
    commands_["pos"]        = [this](std::istream& is) { pos(is); };
    commands_["go"]         = [this](std::istream& is) { go(is); };
    commands_["stop"]       = [this](std::istream& is) { stop(is); };
    commands_["quit"]       = [this](std::istream& is) { quit(is); };
    commands_["perft"]      = [this](std::istream& is) { perft(is); };
    commands_["print"]      = [this](std::istream& is) { print(is); };
}

int CLI::run(int argc, char** argv) {

    // Handle command-line arguments
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            std::cout
                << "Athena " << misc::version() << "\n"
                << "A free uci-compatible 4-player chess engine.\n"
                << "For full documentation, read the README.md file or visit the GitHub page: "
                << misc::github_url() 
                << std::endl;
            return EXIT_SUCCESS;
        }
        else if (arg == "--version" || arg == "-v") {
            std::cout
                << "Athena " << misc::version()
                << std::endl;
            return EXIT_SUCCESS;
        }
        else if (arg == "--license") {
            std::cout 
                << "Athena is free and open source software licensed under the MIT License.\n"
                << "See the LICENSE file for details."
                << std::endl;
            return EXIT_SUCCESS;
        }
        else {
            std::cerr << "info string unknown command line argument '" << arg << "'" << std::endl;
            return EXIT_FAILURE;
        }
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
            std::cout << "info string unknown command '" << line << "'\n";
        }
    }
}

void CLI::uci(std::istream&) {
    std::cout << "id name Athena " << misc::version() << "\n";
    std::cout << "id author " << misc::author() << "\n";
    std::cout << "option name Setup type combo default modern var modern var classic\n";
    std::cout << "uciok" << std::endl;
}

void CLI::isready(std::istream&) {
    std::cout << "readyok" << std::endl;
}

void CLI::setoption(std::istream& args) {

    std::string token, name, value;

    for (int i=0; i<4; i++) {
        if (!(args >> token)) {
            std::cout << "info string expected format: setoption name <name> value <value>\n";
            return;
        }
        if (i == 1) name  = token;
        if (i == 3) value = token;
    }

    engine_.setOption(name, value);
}

void CLI::ucinewgame(std::istream&) {
}

void CLI::pos(std::istream& args) {

    std::string token;
    args >> token;

    std::string fen;
    if (token == "startpos") {
        if (args >> token) {
            fen = token == "modern" ? 
            chess::Position::startpos(chess::Castle::Setup::Modern ):
            chess::Position::startpos(chess::Castle::Setup::Classic);
            args >> token;
        } else {
            std::cout << "info string expected modern or classic after 'startpos' command\n";
        }
    } else if (token == "fen") {
        if (args >> token) {
            fen = token;
            args >> token;
        } else {
            std::cout << "info string expected FEN string after 'fen' command\n";
        }
    } else {
        std::cout << "info string position expected startpos or fen\n";
        return;
    }

    engine_.setPosition(fen);

    if (token == "moves") {
        while (args >> token) engine_.applyMove(token);
    }
}

void CLI::go(std::istream&) {
}

void CLI::stop(std::istream&) {
}

void CLI::quit(std::istream&) {
    std::exit(EXIT_SUCCESS);
}

void CLI::perft(std::istream&) {
}

void CLI::print(std::istream&) {
}

} // namespace athena