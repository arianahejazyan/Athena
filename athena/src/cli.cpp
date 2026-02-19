#include <iostream>
#include "cli.h"
#include "utility.h"
#include "movegen.h"
#include "uci.h"
#include "perft.h"

namespace athena
{

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
    while (std::getline(std::cin, line_)) 
    {
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
        if (name == "go"        ) handleGo();         else 
        if (name == "stop"      ) handleStop();       else 
        if (name == "quit"      ) handleQuit();       else 
        if (name == "perft"     ) handlePerft();      else 
        if (name == "print"     ) handlePrint();      else
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
    std::size_t index = 1;
    FEN fen;

    if (tokens_[index] == "fen")
    {
        index++;
        fen = tokens_[index];
    }

    else fen = STARTPOS;

    pos_.init(fen);
    // index++;
    // index++;

    // while (index < tokens_.size())
    // {
    //     pos_.makemove(Move(tokens_[index], pos_));
    //     index++;
    // }
}

void CLI::handleGo()
{
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
    std::size_t index = 1;
    Perft perft;

    int depth = std::stoi(std::string(tokens_[index]));
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