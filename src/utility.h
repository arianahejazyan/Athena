#pragma once

#include <string>
#include <vector>
#include <cstring>

namespace athena
{

// /// @brief
// int tokenize(std::string& line, char* argv[], int max_args, const char* delim = " ");

/// @brief
std::vector<std::string_view> tokenize(std::string_view input, char delimiter = ' ');

} // namespace athena