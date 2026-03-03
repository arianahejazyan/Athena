#include <iostream>
#include <stdexcept>
#include <chrono>
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
#include "cli.h"
#include "utility.h"
#include "movegen.h"
#include "uci.h"
#include "perft.h"
#include "attacks.h"
#include "move_parser.h"

namespace athena
{

namespace
{
bool stdin_is_tty() noexcept
{
#if defined(_WIN32)
    return _isatty(_fileno(stdin)) != 0;
#else
    return isatty(fileno(stdin)) != 0;
#endif
}
} // namespace

// CommandLineInterface::CommandLineInterface()
// {
//     auto cmdUCI        = app.add_subcommand("uci"       )->callback([this] { handleUCI();        });
//     auto cmdIsReady    = app.add_subcommand("isready"   )->callback([this] { handleIsReady();    });
//     auto cmdSetOption  = app.add_subcommand("setoption" )->callback([this] { handleSetOption();  });
//     auto cmdUCINewGame = app.add_subcommand("ucinewgame")->callback([this] { handleUCINewGame(); });
//     auto cmdMakemove   = app.add_subcommand("makemove"  )->callback([this] { handleMakemove();   });
//     auto cmdUndomove   = app.add_subcommand("undomove"  )->callback([this] { handleUndomove();   });
//     auto cmdPos        = app.add_subcommand("pos"       )->callback([this] { handlePos();        });
//     auto cmdFEN        = app.add_subcommand("fen"       )->callback([this] { handleFEN();        });
//     auto cmdGo         = app.add_subcommand("go"        )->callback([this] { handleGo();         });
//     auto cmdStop       = app.add_subcommand("stop"      )->callback([this] { handleStop();       });
//     auto cmdQuit       = app.add_subcommand("quit"      )->callback([this] { handleQuit();       });
//     auto cmdPerft      = app.add_subcommand("perft"     )->callback([this] { handlePerft();      });
//     auto cmdPrint      = app.add_subcommand("print"     )->callback([this] { handlePrint();      });

//     ///
//     cmdPos->add_option("type", pos_mode)->required()->check(CLI::IsMember({"startpos", "fen"}));
//     cmdPos->allow_extras();

//     ///
//     cmdPrint->add_flag("-d,--debug", debug);

//     // Init starting position
//     pos.init(STARTPOS[setup = Setup::Modern]);
// }

CLI::CLI()
{
    pos_.init(STARTPOS);
}

void CLI::launch()
{
    const bool interactive = stdin_is_tty();

    if (interactive)
    {
        std::cout
            << "Athena ready. Type 'help' for commands." << std::endl;
    }

    while (true)
    {
        if (interactive)
        {
            std::cout << "> " << std::flush;
        }

        if (!std::getline(std::cin, line_))
        {
            break;
        }

        tokens_ = tokenize(std::string_view(line_), ' ');
        execute();
    }
}

void CLI::execute()
{
    try
    {
        if (tokens_.empty())
        {
            std::cout << "info string empty command" << std::endl;
            return;
        }

        auto name = tokens_.front();

        if (name == "uci"       ) handleUCI();        else 
        if (name == "isready"   ) handleIsReady();    else 
        if (name == "setoption" ) handleSetOption();  else 
        if (name == "ucinewgame") handleUCINewGame(); else 
        if (name == "pos"       ) handlePos();        else 
        if (name == "move"      ) handleMove();       else
        if (name == "go"        ) handleGo();         else 
        if (name == "stop"      ) handleStop();       else 
        if (name == "quit"      ) handleQuit();       else 
        if (name == "perft"     ) handlePerft();      else 
        if (name == "bench"     ) handleBench();      else
        if (name == "print"     ) handlePrint();      else
        if (name == "board"     ) handlePrint();      else
        if (name == "help"      ) handleHelp();       else
        if (name == "fen"       ) handleFen();        else
        if (name == "movegen"   ) handleMoveGen();    else
        {
            std::cout << "info string unknown command '" << name << "'" << std::endl;
            return;
        }
    }

    catch (const std::exception& e) {
        std::cout << "info string " << e.what() << std::endl;
    }
}

// void CommandLineInt erface::launch()
// {
//     std::string line;
//     while (std::getline(std::cin, line)) if (!line.empty()) execute(line);
// }

// void CommandLineInterface::execute(std::string& line)
// {
//     try 
//     {
//         char* argv[64];
//         int argc = tokenize(line, argv, 64);

//         // Reset configurations to default values
//         debug = false;

//         // Parse and execute command
//         app.clear();
//         app.parse(argc, argv);
//     }

//     catch (const CLI::ParseError& e) { std::cout << "info error " << e.what() << std::endl; }
//     catch (const std::exception & e) { std::cout << "info error " << e.what() << std::endl; }
// }

void CLI::handleUCI()
{
    std::cout 
    << "id name Athena" << '\n'
    << "id author Ariana Hejazyan" << '\n' 
    << "uciok" << std::endl;
}

void CLI::handleIsReady()
{
    std::cout << "readyok" << std::endl;
}

void CLI::handleSetOption()
{
}

void CLI::handleUCINewGame()
{
}

void CLI::handlePos()
{   
    if (tokens_.size() < 2 || tokens_[1] == "startpos")
    {
        pos_.init(STARTPOS);
        return;
    }

    if (tokens_[1] == "fen")
    {
        if (tokens_.size() < 3)
        {
            throw std::runtime_error("usage: pos [startpos | fen <FEN>]");
        }

        pos_.init(FEN(tokens_[2]));
        return;
    }

    throw std::runtime_error("usage: pos [startpos | fen <FEN>]");
}

void CLI::handleGo()
{
}

void CLI::handleMove()
{
    if (tokens_.size() < 2)
    {
        throw std::runtime_error("usage: move <uci>");
    }

    Move move;
    std::string error;
    if (!parse_move_uci(pos_, tokens_[1], move, &error))
    {
        throw std::runtime_error(error);
    }

    pos_.makemove(move);
}

void CLI::handleStop()
{
}

void CLI::handleQuit()
{
    std::exit(0);
}

void CLI::handlePerft()
{
    if (tokens_.size() < 2)
    {
        throw std::runtime_error("usage: perft <depth>");
    }

    Perft perft;

    int depth = std::stoi(std::string(tokens_[1]));
    // index++;

    // bool split   = false;
    // bool tabular = false;
    // if (tokens_.size() > index)
    // {
    //     if (tokens_[index] == "-s" || tokens_[index] == "--split")
    //     {
    //         split = true;
    //     }

    //     if (tokens_[index] == "-t" || tokens_[index] == "--tabular")
    //     {
    //         tabular = true;
    //     }
    // }

    std::cout << "running perft at depth " << depth << std::endl;

    Perft::Options options;
    options.depth = depth;

    perft.run(pos_, options);
    
    std::cout << "perft completed" << std::endl;
}

void CLI::handlePrint()
{
    pos_.print();
}

void CLI::handleBench()
{
    if (tokens_.size() < 2 || tokens_[1] != "attacks")
    {
        throw std::runtime_error("usage: bench attacks [iterations]");
    }

    uint64_t iters = 1000000;
    if (tokens_.size() >= 3)
    {
        iters = std::stoull(std::string(tokens_[2]));
        if (iters == 0)
        {
            throw std::runtime_error("iterations must be > 0");
        }
    }

    static const std::vector<Square> playableSquares = []() {
        std::vector<Square> squares;
        squares.reserve(196);
        for (int rank = 1; rank <= 14; ++rank)
        {
            for (int file = 1; file <= 14; ++file)
            {
                Square sq(rank, file);
                if (!sq.stone()) squares.push_back(sq);
            }
        }
        return squares;
    }();

    Bitboard occupied(0, 0, 0, 0);
    uint64_t checksum = 0;

    const auto start = std::chrono::high_resolution_clock::now();

    for (uint64_t i = 0; i < iters; ++i)
    {
        const Square sq1 = playableSquares[i % playableSquares.size()];
        const Square sq2 = playableSquares[(i * 37 + 11) % playableSquares.size()];
        occupied.set(sq2);

        const Bitboard b = bishop_attacks(sq1, occupied);
        const Bitboard r = rook_attacks(sq1, occupied);
        checksum ^= b.combine();
        checksum ^= (r.combine() << 1);

        occupied.pop(sq2);
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    const double seconds = elapsed.count();
    const auto nps = static_cast<uint64_t>(static_cast<double>(iters) / seconds);

    std::cout << "bench attacks iterations: " << iters << std::endl;
    std::cout << "bench attacks seconds: " << seconds << std::endl;
    std::cout << "bench attacks nps: " << nps << std::endl;
    std::cout << "bench attacks checksum: " << checksum << std::endl;
}

void CLI::handleHelp()
{
    std::cout
        << "commands: uci, isready, pos [startpos|fen <FEN>], move <uci>, fen, print, board, perft <depth>, bench attacks [iterations], quit"
        << std::endl;
}

void CLI::handleFen()
{
    std::cout << pos_.fen() << std::endl;
}

void CLI::handleMoveGen()
{

    // Move moves[MOVE_NB];
    // std::size_t num = genAllPseudoMoves(pos_, moves);
    // for (std::size_t i = 0; i < num; i++)
    // {
    //     std::cout << move2UCI(moves[i]) << std::endl;
    // }
    // std::cout << "total pseudo legal moves: " << num << std::endl;
}

} // namespace athena
