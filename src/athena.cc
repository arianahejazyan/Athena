#include "uci/uci.h"

using namespace athena;

int main(int argc, char* argv[]) {

    uci::UCIEngine engine;
    engine.run(argc, argv);

    return 0;
}