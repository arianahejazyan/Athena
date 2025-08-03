#include "cli.h"
#include "perft.h"

namespace athena
{

#define FEN_MODERN  "R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/x,x,x,8,x,x,x/bR,bP,10,gP,gR/bN,bP,10,gP,gN/bB,bP,10,gP,gB/bQ,bP,10,gP,gK/bK,bP,10,gP,gQ/bB,bP,10,gP,gB/bN,bP,10,gP,gN/bR,bP,10,gP,gR/x,x,x,8,x,x,x/x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x"
#define FEN_CLASSIC "R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/x,x,x,8,x,x,x/bR,bP,10,gP,gR/bN,bP,10,gP,gN/bB,bP,10,gP,gB/bK,bP,10,gP,gQ/bQ,bP,10,gP,gK/bB,bP,10,gP,gB/bN,bP,10,gP,gN/bR,bP,10,gP,gR/x,x,x,8,x,x,x/x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x"

void CommandLine::launch()
{
    // char cmd[] = "pos modern";
    // execute(cmd);

    // static String<1024> line;
    line.write("pos modern");
    line.close();
    execute(line);

    while (true)
    {
        line.size = 0;

        if (!std::cin.getline(line.c_str, sizeof(line)))
            break;

        if (line.c_str[0] == '\0')
            continue;

        execute(line);
    }
}

void CommandLine::execute(String<1024> line)
{
    argc = tokenize(line.c_str, argv, 32);
    if (argc == 0) return;
    
    const char* cmd = argv[0];

    if (strcmp(cmd, "uci"       ) == 0) handleUCI(       ); else
    if (strcmp(cmd, "isready"   ) == 0) handleIsReady(   ); else
    if (strcmp(cmd, "setoption" ) == 0) handleSetOption( ); else
    if (strcmp(cmd, "ucinewgame") == 0) handleUCINewGame(); else
    if (strcmp(cmd, "setup"     ) == 0) handleSetup(     ); else
    if (strcmp(cmd, "pos"       ) == 0) handlePosition(  ); else
    if (strcmp(cmd, "fen"       ) == 0) handleFEN(       ); else
    if (strcmp(cmd, "go"        ) == 0) handleGo(        ); else
    if (strcmp(cmd, "stop"      ) == 0) handleStop(      ); else
    if (strcmp(cmd, "quit"      ) == 0) handleQuit(      ); else
    if (strcmp(cmd, "perft"     ) == 0) handlePerft(     ); else
    if (strcmp(cmd, "print"     ) == 0) handlePrint(     ); else

    std::cout << "info string unknown command " << cmd << std::endl;
}

    // Engine::Engine()
    // {
    //     pos.setup = Modern;
    //     pos.init(FEN_MODERN);

    //     // fromString(FEN_MODERN, pos);

    //     // auto* uciCommand = app.add_subcommand("uci", "[UCI] Start UCI protocol and identify the engine")
    //     // ->callback([this]() { handleUCI(); });

    //     // auto* isreadyCommand = app.add_subcommand("isready", "Ensure engine is fully initialized before continuing")
    //     // ->callback([this]() { handleIsReady(); });

    //     // auto* setOptionCommand = app.add_subcommand("setoption", "Set an engine option in UCI format")
    //     // ->callback([this]() { handleSetOption(); });

    //     // setOptionCommand->allow_extras();

    //     // auto* ucinewgameCommand = app.add_subcommand("ucinewgame", "Start a new game")
    //     // ->callback([this]() { handleUCINewGame(); });

    //     // auto* positionCommand = app.add_subcommand("position", "Position setup and display")
    //     // ->callback([this]() { handlePosition(); });

    //     // positionCommand->add_option("mode", position_mode, "Position setup mode")
    //     // ->required()
    //     // ->check(CLI::IsMember({"classic", "modern", "fen"}));

    //     // positionCommand->allow_extras();

    //     // auto* goCommand = app.add_subcommand("go", "COMPLETE")
    //     // ->callback([this]() { handleGo(); });

    //     // auto* stopCommand = app.add_subcommand("stop", "COMPLETE")
    //     // ->callback([this]() { handleStop(); });

    //     // auto* exitCommand = app.add_subcommand("quit", "Quit the engine")
    //     // ->callback([this]() { handleQuit(); });

    //     auto *perftCommand = app.add_subcommand("perft", "Run perft to given depth")
    //                              ->callback([this]()
    //                                         { handlePerft(); });

    //     perftCommand->add_option("depth", perft_depth, "Depth to run perft")
    //         ->required()
    //         ->check(CLI::PositiveNumber);

    //     perftCommand->add_flag("-f,--full", perft_full, "Show full detailed report");
    //     perftCommand->add_flag("-s,--split", perft_split, "Show perft per move (split node counts)");
    //     perftCommand->add_flag("-c,--cumulative", perft_cumulative, "Show cumulative totals at each depth");

    //     // auto *printCommand = app.add_subcommand("print", "Print current position")
    //     //                          ->callback([this]()
    //     //                                     { handlePrint(); });

    //     // printCommand->add_flag("-c,--config", print_config, "Show current engine configuration");
    //     // printCommand->add_flag("-f,--fen", print_fen, "Print position in FEN format");
    //     // printCommand->add_flag("-a,--ascii", print_ascii_pieces, "Print board as ASCII layout");

    //     auto *printCommand = app.add_subcommand("print", "Print current position")
    //                              ->callback([this]()
    //                                         { handlePrint(); });

    //     printCommand->add_flag("-d,--debug", debug, "Show current engine configuration");
    // }

    // void Engine::launch()
    // {
    //     std::string line;
    //     while (true)
    //     {
    //         if (!std::getline(std::cin, line))
    //             break;
    //         if (line.empty())
    //             continue;

    //         std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    //         std::vector<std::string> args = tokenize(line);
    //         args.insert(args.begin(), "athena");

    //         std::vector<const char *> argv;
    //         for (const auto &arg : args)
    //             argv.push_back(arg.c_str());
    //         int argc = static_cast<int>(argv.size());

    //         execute(argc, argv.data());
    //     }
    // }

    // void Engine::execute(int argc, const char *argv[])
    // {
    //     try // execute command //
    //     {
    //         app.clear();

    //         debug = false;

    //         //
    //         perft_full = false;
    //         perft_split = false;
    //         perft_cumulative = false;

    //         //
    //         print_config = false;
    //         print_fen = false;
    //         print_ascii_pieces = false;

    //         app.parse(argc, argv);
    //     }
    //     catch (const CLI::ParseError &e)
    //     {
    //         std::cerr << "info string cli invalid command" << std::endl;
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cout << "info string " << e.what() << std::endl;
    //     }
    // }

void CommandLine::handleUCI()
{
    std::cout 
    << "id name Athena\n"
    << "id author Ariana Hejazyan\n" 
    << "uciok" 
    << std::endl;
}

void CommandLine::handleIsReady() {
    std::cout << "readyok" << std::endl;
}

void CommandLine::handleSetOption()
{
    // const auto &extras = app.get_subcommand("setoption")->remaining();

    // if (extras.size() != 4 || extras[0] != "name" || extras[2] != "value")
    //     throw std::invalid_argument("expected format: setoption name <name> value <value>");

    // const std::string &name = extras[1];
    // const std::string &value = extras[3];

    // if (name == "debug")
    // {
    //     if (value == "on")
    //         debug = true;
    //     else if (value == "off")
    //         debug = false;
    //     else
    //         throw std::invalid_argument("invalid debug value: " + value);
    // }

    // else
    //     throw std::invalid_argument("unknown option name: " + name);
}

void CommandLine::handleUCINewGame()
{
}

void CommandLine::handleSetup()
{
    if (argc >= 2)
    {
        if (std::strcmp(argv[1], "modern" ) == 0) setup = Modern ; else
        if (std::strcmp(argv[1], "classic") == 0) setup = Classic; else 
        std::cout << "info string invalid setup argument" << std::endl;
    } 
    else std::cout << "info string missing setup argument" << std::endl;
}

void CommandLine::handlePosition()
{
    if (argc >= 2)
    {
        static String<1024> fen;
        fen.size = 0;
        if (std::strcmp(argv[1], "modern" ) == 0) setup = Modern , fen.write(FEN_MODERN ), pos.init(fen.c_str, setup); else
        if (std::strcmp(argv[1], "classic") == 0) setup = Classic, fen.write(FEN_CLASSIC), pos.init(fen.c_str, setup); else
        pos.init(argv[1], setup);
    } 
    else std::cout << "info string missing position argument" << std::endl;
    


    // if (argc >= 2)
    // {
    //     static String<1024> fen;
    //     fen.size = 0;
    //     // static char fen[1024];
    //     if (std::strcmp(argv[1], "modern" ) == 0) setup = Modern , std::strcpy(fen, FEN_MODERN ), pos.init(fen, setup); else
    //     if (std::strcmp(argv[1], "classic") == 0) setup = Classic, std::strcpy(fen, FEN_CLASSIC), pos.init(fen, setup); else 
    //     pos.init(argv[1], setup);
    // } 
    // else std::cout << "info string missing position argument" << std::endl;
    



    // auto *positionCommand = app.get_subcommand("position");
    // std::vector<std::string> extras = positionCommand->remaining();

    // std::string fen;
    // if (position_mode == "fen")
    // {
    //     int numTokens = 7;
    //     if (extras.size() < numTokens)
    //         throw std::invalid_argument(
    //             "FEN requires " + std::to_string(numTokens) + " parameters, but received " + std::to_string(extras.size()));

    //     fen = concatenate(extras, 0, numTokens, ' ');
    //     extras.erase(extras.begin(), extras.begin() + numTokens);
    // }

    // else if (position_mode == "modern")
    //     fen = FEN_MODERN;
    // else if (position_mode == "classic")
    //     fen = FEN_CLASSIC;

    // //
    // fromString(fen, pos);

    // //
    // if (extras.empty())
    //     return;

    // //
    // if (extras[0] == "moves")
    // {
    //     Move moves[MAX_MOVES];
    //     int size = 0;

    //     extras.erase(extras.begin());
    //     for (const auto &move : extras)
    //     {
    //         size = 0;
    //         size += genAllNoisyMoves(pos, moves + size);
    //         size += genAllQuietMoves(pos, moves + size);

    //         for (int i = 0; i < size; ++i)
    //         {
    //             if (toString(moves[i]) == move)
    //             {
    //                 pos.makemove(moves[i]);
    //                 break;
    //             }
    //         }
    //     }
    // }

    // else
    // {
    //     throw std::invalid_argument("expected 'moves' keyword");
    // }
}

void CommandLine::handleGo()
{
}

void CommandLine::handleStop()
{
}

void CommandLine::handleQuit() {
    std::exit(0);
}

void CommandLine::handlePerft()
{
    static Perft perft;
    perft.run(pos, 6, false, false, false);
    // runPerftTests(pos, perft_depth, perft_full, perft_split, perft_cumulative);
}

void CommandLine::handleFEN() {
    std::cout << "fen " << pos.fen() << std::endl;
}

void CommandLine::handlePrint()
{
    bool debug = false;
    if (argc >= 2)
    {
        if (std::strcmp(argv[1], "-d") != 0)
        {
            std::cout << "info string invalid print argument [" << argv[1] << "]" << std::endl;
            return;
        }
        else debug = true;
    }
    pos.print(debug);
    // pos.print(debug);

    // print(pos, print_fen, print_ascii_pieces);

    // if (print_config)
    // {
    //     std::cout << std::endl;
    //     std::cout << "configurations: " << std::endl;
    //     std::cout << "debug " << (debug ? "on" : "off") << std::endl;
    // }
}

} // namespace athena