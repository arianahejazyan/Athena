#pragma once

#include <string>
#include "nnue/network.h"

namespace athena::nnue {

struct Header {
    char magic[4]; // "NNUE"
    uint32_t version;
};

class Weights {
public:
    static bool load(const std::string& path, Network& net);
    static bool save(const std::string& path, const Network& net);

private:
    static constexpr Header HEADER{{'N', 'N', 'U', 'E'}, 1};
};

// auto weights = std::filesystem::path("bin") / "nnue_weights.bin";

} // namespace athena
