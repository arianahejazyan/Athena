#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include "chess/position.h"
#include "chess/castle.h"
#include "chess/perft.h"

using namespace athena;

struct PerftCase {
    std::string id;
    std::string setupName;
    int depth = 0;
    uint64_t expected = 0;
    std::string fen;
    chess::Castle::Setup setup;
};

std::ostream& operator<<(std::ostream& os, const PerftCase& tc) {
    
    return os
        << tc.id
        << " setup=" << tc.setupName
        << " depth=" << tc.depth;
}

// ─────────────────────────────────────────────
// Loader
// ─────────────────────────────────────────────

std::vector<PerftCase> LoadCases(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("Cannot open: " + path);

    std::vector<PerftCase> cases;
    std::string line;

    // int maxDepth = INT_MAX;
    // if (const char* env = std::getenv("MAX_DEPTH"))
    //     maxDepth = std::stoi(env);

    int index = 0;

    while (std::getline(file, line))
    {
        // strip inline comments
        if (auto pos = line.find('#'); pos != std::string::npos)
            line = line.substr(0, pos);

        // skip empty lines
        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
            continue;

        std::istringstream ss(line);

        PerftCase tc;
        tc.id = "tc_" + std::to_string(index++);

        // STRICT PARSING (single pass)
        if (!(ss >> tc.setupName >> tc.depth >> tc.expected >> tc.fen))
        {
            throw std::runtime_error(
                "Invalid perft line (expected: setup depth expected fen)\nLine: " + line
            );
        }

        // setup mapping
        if (tc.setupName == "modern")
            tc.setup = chess::Castle::Setup::Modern;
        else if (tc.setupName == "classic")
            tc.setup = chess::Castle::Setup::Classic;
        else
            throw std::runtime_error("Unknown setup: " + tc.setupName);

        // // depth filter
        // if (tc.depth > maxDepth)
        //     continue;

        cases.push_back(tc);
    }

    return cases;
}

// ─────────────────────────────────────────────────────────────
// Test fixture
// ─────────────────────────────────────────────────────────────

class PerftTest : public testing::TestWithParam<PerftCase> {};

// ─────────────────────────────────────────────────────────────
// Test
// ─────────────────────────────────────────────────────────────

TEST_P(PerftTest, NodeCount) {
    const auto& tc = GetParam();

    int maxDepth = INT_MAX;
    if (const char* env = std::getenv("MAX_DEPTH"))
        maxDepth = std::stoi(env);

    if (tc.depth > maxDepth)
        GTEST_SKIP() << "Filtered by MAX_DEPTH=" << maxDepth;

    chess::Position pos;
    pos.set_setup(tc.setup);
    pos.init(tc.fen);

    uint64_t actual = chess::perft(pos, tc.depth);

    EXPECT_EQ(actual, tc.expected)
        << "setup=" << tc.setupName << " "
        << "depth=" << tc.depth << " "
        << "fen="   << tc.fen;
}

// ─────────────────────────────────────────────────────────────
// Instantiation
// ─────────────────────────────────────────────────────────────

INSTANTIATE_TEST_SUITE_P(
    Perft,
    PerftTest,
    testing::ValuesIn(LoadCases(PERFT_DATA_DIR "/perft.txt")),
    [](const testing::TestParamInfo<PerftCase>& info) {
        return info.param.id;
    }
);