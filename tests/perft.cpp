#include "perft.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

using namespace athena;

int main(int argc, char** argv)
{
    const auto classic = chess::Castle::Setup::Classic;
    const auto modern  = chess::Castle::Setup::Modern ;

    chess::Position pos;
    chess::Castle::Setup setup = modern;
    std::string fen = chess::Position::startpos(setup);;
    int depth = 5;
    bool split = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--classic") { setup = classic; fen = chess::Position::startpos(setup); } else
        if (arg == "--modern" ) { setup = modern ; fen = chess::Position::startpos(setup); } else
        if (arg == "--setup"  ) { setup = strcmp(argv[++i], "modern") == 0 ? modern : classic; } else
        if (arg == "--depth"  ) { depth = std::stoi(argv[++i]); } else
        if (arg == "--fen"    ) { fen = argv[++i]; } else
        if (arg == "--split"  ) { split = true; }
    }

    pos.set_setup(setup);
    pos.init(fen);
    uint64_t nodes = tests::run_perft_tests(pos, depth, split);

    return EXIT_SUCCESS;
}