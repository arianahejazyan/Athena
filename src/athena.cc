#include "cli/cli.h"

using namespace athena;

int main(int argc, char* argv[]) {
    cli::CLI cli;
    return cli.run(argc, argv);
}