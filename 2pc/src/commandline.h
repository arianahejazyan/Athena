#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <string>
#include <vector>
#include "position.h"

namespace Talia
{

class CommandLine
{
    private:

        Position pos;

    public:

        CommandLine();
        ~CommandLine();

        void run();

    private:

        void handleMakemove(std::vector<std::string>& args);
        void handleUndomove();
        void handlePerft(std::vector<std::string>& args);
        void handleCommand(const std::string& command);
};

} // namespace

#endif