#include <fstream>
#include <cstring>
#include "weights.h"

namespace athena::nnue {

bool Weights::load(const std::string& path, Network& net)
{
    std::ifstream file(path, std::ios::binary);

    if (!file)
        return false;

    Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (!file || std::memcmp(&header, &HEADER, sizeof(Header)) != 0)
        return false;

    file.read(reinterpret_cast<char*>(&net), sizeof(Network));

    return file.good();
}

bool Weights::save(const std::string& path, const Network& net)
{
    std::ofstream file(path, std::ios::binary);

    if (!file)
        return false;

    file.write(reinterpret_cast<const char*>(&HEADER), sizeof(HEADER));

    if (!file)
        return false;

    file.write(reinterpret_cast<const char*>(&net), sizeof(Network));

    return file.good();
}

} // namespace athena
