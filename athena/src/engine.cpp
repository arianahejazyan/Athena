#include "engine.h"
#include "utility.h"
#include "movegen.h"
#include "perft.h"

namespace athena
{

Engine::Engine() : pos()
{
    fromString(FEN_MODERN, pos);

    auto* uciCommand = app.add_subcommand("uci", "[UCI] Start UCI protocol and identify the engine")
    ->callback([this]() { handleUCI(); });

    auto* isreadyCommand = app.add_subcommand("isready", "Ensure engine is fully initialized before continuing")
    ->callback([this]() { handleIsReady(); });

    auto* setOptionCommand = app.add_subcommand("setoption", "Set an engine option in UCI format")
    ->callback([this]() { handleSetOption(); });

    setOptionCommand->allow_extras();

    auto* ucinewgameCommand = app.add_subcommand("ucinewgame", "Start a new game")
    ->callback([this]() { handleUCINewGame(); });

    auto* positionCommand = app.add_subcommand("position", "Position setup and display")
    ->callback([this]() { handlePosition(); });
    
    positionCommand->add_option("mode", position_mode, "Position setup mode")
    ->required()
    ->check(CLI::IsMember({"classic", "modern", "fen"}));
    
    positionCommand->allow_extras();
    
    auto* goCommand = app.add_subcommand("go", "COMPLETE")
    ->callback([this]() { handleGo(); });

    auto* stopCommand = app.add_subcommand("stop", "COMPLETE")
    ->callback([this]() { handleStop(); });

    auto* exitCommand = app.add_subcommand("quit", "Quit the engine")
    ->callback([this]() { handleQuit(); });

    auto* perftCommand = app.add_subcommand("perft", "Run perft to given depth")
    ->callback([this]() { handlePerft(); });

    perftCommand->add_option("depth", perft_depth, "Depth to run perft")
    ->required()
    ->check(CLI::PositiveNumber);

    perftCommand->add_flag("-f,--full", perft_full, "Show full detailed report");
    perftCommand->add_flag("-s,--split", perft_split, "Show perft per move (split node counts)");
    perftCommand->add_flag("-c,--cumulative", perft_cumulative, "Show cumulative totals at each depth");

    auto* printCommand = app.add_subcommand("print", "Print current position")
    ->callback([this]() { handlePrint(); });

    printCommand->add_flag("-c,--config", print_config, "Show current engine configuration");
    printCommand->add_flag("-f,--fen", print_fen, "Print position in FEN format");
    printCommand->add_flag("-a,--ascii", print_ascii_pieces, "Print board as ASCII layout");
}

void Engine::launch()
{
    std::string line;
    while (true)
    {
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::vector<std::string> args = tokenize(line);
        args.insert(args.begin(), "athena");
        
        std::vector<const char*> argv;
        for (const auto& arg : args) argv.push_back(arg.c_str());
        int argc = static_cast<int>(argv.size());
        
        execute(argc, argv.data());
    }
}

void Engine::execute(int argc, const char* argv[])
{
    try // execute command //
    {
        app.clear();

        // 
        perft_full = false;
        perft_split = false;
        perft_cumulative = false;

        //
        print_config = false;
        print_fen = false;
        print_ascii_pieces = false;
        
        app.parse(argc, argv);
    } 
    catch (const CLI::ParseError& e) { std::cerr << "info string cli invalid command" << std::endl; } 
    catch (const std::exception&  e) { std::cout << "info string " << e.what() << std::endl; }
}

void Engine::handleUCI()
{
    std::cout << "id name Athena" << std::endl;
    std::cout << "id author Ariana Hejazyan" << std::endl;
    std::cout << "uciok" << std::endl << std::flush;
}

void Engine::handleIsReady()
{
    std::cout << "readyok" << std::endl << std::flush;
}

void Engine::handleSetOption()
{
    const auto& extras = app.get_subcommand("setoption")->remaining();

    if (extras.size() != 4 || extras[0] != "name" || extras[2] != "value") 
        throw std::invalid_argument("expected format: setoption name <name> value <value>");

    const std::string& name  = extras[1];
    const std::string& value = extras[3];

    if (name == "debug")
    {
             if (value == "on" ) debug = true ;
        else if (value == "off") debug = false;
        else throw std::invalid_argument("invalid debug value: " + value);
    }

    else throw std::invalid_argument("unknown option name: " + name);
}

void Engine::handleUCINewGame()
{

}

void Engine::handlePosition()
{
    auto* positionCommand = app.get_subcommand("position");
    std::vector<std::string> extras = positionCommand->remaining();
    
    std::string fen;
    if (position_mode == "fen") 
    {
        int numTokens = 7;
        if (extras.size() < numTokens)
            throw std::invalid_argument(
                "FEN requires " + std::to_string(numTokens) + " parameters, but received " + std::to_string(extras.size()));

        fen = concatenate(extras, 0, numTokens, ' ');
        extras.erase(extras.begin(), extras.begin() + numTokens);
    }

    else if (position_mode == "modern" ) fen = FEN_MODERN ;
    else if (position_mode == "classic") fen = FEN_CLASSIC;
    
    // 
    fromString(fen, pos);

    //
    if (extras.empty()) return;

    // 
    if (extras[0] == "moves")
    {
        Move moves[MAX_MOVES];
        int size = 0;

        extras.erase(extras.begin());
        for (const auto& move : extras)
        {
            size = 0;
            size += genAllNoisyMoves(pos, moves + size);
            size += genAllQuietMoves(pos, moves + size);

            for (int i = 0; i < size; ++i)
            {
               if (toString(moves[i]) == move)
                {
                    pos.makemove(moves[i]);
                    break;
                }                
            }      
        }
    } 
    
    else { throw std::invalid_argument("expected 'moves' keyword"); }
}

void Engine::handleGo()
{
    std::cout << "bestmove e2e4" << std::endl << std::flush;
}

void Engine::handleStop()
{

}

void Engine::handleQuit() {
    std::exit(0);
}

void Engine::handlePerft() {
    runPerftTests(pos, perft_depth, perft_full, perft_split, perft_cumulative);
}

void Engine::handlePrint()
{
    print(pos, print_fen, print_ascii_pieces);

    if (print_config)
    {
        std::cout << std::endl;
        std::cout << "configurations: "  << std::endl;
        std::cout << "debug " << (debug ? "on" : "off") << std::endl;
    }
}

} // namespace athena