#include <sstream>
#include "cli/cli.h"

namespace athena::cli {

void CLI::run(int argc, char *argv[]) {

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty()) continue;
        std::istringstream is(line);
        std::string name;
        is >> name;
        if (name.empty()) continue;
        if (name == "uci"       ) uci();         else 
        if (name == "isready"   ) isready();     else 
        if (name == "setoption" ) setoption(is); else 
        if (name == "ucinewgame") ucinewGame();  else 
        if (name == "pos"       ) pos(is);       else 
        if (name == "go"        ) go(is);        else 
        if (name == "stop"      ) stop();        else 
        if (name == "quit"      ) quit();        else {
            std::cout << "info string" << "unknown command: '" << line << std::endl;
        }
    }
}

void CLI::uci() {
    std::cout << "id name Athena\n";
    std::cout << "id author Ariana Hejazyan\n";
    std::cout << "option name Setup type combo default modern var modern var classic\n";
    std::cout << "uciok" << std::endl;
}

void CLI::isready() {
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

void CLI::ucinewGame() {
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

void CLI::go(std::istream& args) {
}

void CLI::stop() {
}

void CLI::quit() {
    std::exit(EXIT_SUCCESS);
}

} // namespace athena