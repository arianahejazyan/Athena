#include "utility.h"

namespace athena
{

// int tokenize(std::string& line, char* argv[], int max_args, const char* delim)
// {
//     int argc = 0;
//     argv[argc++] = const_cast<char*>("athena");

//     char* token = std::strtok(line.data(), delim);
//     while (token && argc < max_args) {
//         if (*token != '\0') argv[argc++] = token; token = std::strtok(nullptr, delim);
//     }

//     return argc;
// }

std::vector<std::string_view> tokenize(std::string_view input, char delimiter)
{
    // if (input.empty()) return {};
    
    std::vector<std::string_view> tokens;
    std::size_t start = 0;

    while (true)
    {
        std::size_t pos = input.find(delimiter, start);
        if (pos == std::string_view::npos) {
            tokens.emplace_back(input.substr(start));
            break;
        }
        tokens.emplace_back(input.substr(start, pos - start));
        start = pos + 1;
    }

    return tokens;
}

} // namespace athena