#include <iostream>
#include "cli/cli.h"

using namespace athena;

void version() {
    std::cout << "v0.0.0" << std::endl;
}

void help() {
    std::cout << "UCI commands:\n"
              << "  uci           - identify engine\n"
              << "  isready       - ping engine\n"
              << "  setoption     - set engine option\n"
              << "  ucinewgame    - reset for new game\n"
              << "  position      - set board position\n"
              << "  go            - start searching\n"
              << "  stop          - stop searching\n"
              << "  quit          - exit\n";
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string name = argv[1];
        if (name == "--version") { version(); return EXIT_SUCCESS; } else
        if (name == "--help"   ) { help();    return EXIT_SUCCESS; } else {
            std::cerr << "info string unknown command line argument: " << name << std::endl;
            return EXIT_FAILURE;
        }  
    }

    cli::CLI cli;
    cli.run(argc, argv);

    return EXIT_SUCCESS;
}