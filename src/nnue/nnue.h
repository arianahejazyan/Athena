#pragma once

#include <fstream>
#include "chess/bitboard.h"
#include "chess/constants.h"
#include "chess/square.h"
#include "nnue/feature.h"
#include "nnue/layers.h"
#include "chess/position.h"
#include "score.h"

namespace athena::nnue {

class NNUE {
public:
    bool load(const std::string& filebath);

    void set_accumulator(chess::Square sq, chess::Piece piece, chess::Color color) noexcept;
    void pop_accumulator(chess::Square sq, chess::Piece piece, chess::Color color) noexcept;
    void refresh_accumulator(const chess::Position& pos) noexcept;

    Score evaluate() const noexcept;

private:

    // Network architecture version
    static constexpr uint version = 1;

    // Network architecture
    enum { L1, L2, L3, L4, NNUE_LAYERS };
    enum { W, B, PARAMETERS };
    enum { ROW, COL, DIMENSIONS };

    // Matrix shapes
    static constexpr uint ARCH[NNUE_LAYERS][DIMENSIONS] = {
        {160*6*4, 256}, // Layer1
        {128    , 256}, // Layer2
        {64     , 128}, // Layer3
        {1      , 64 }, // Layer4
    };

    static constexpr uint ARCH_DIMENSIONS[NNUE_LAYERS][PARAMETERS] = {
        {ARCH[L1][ROW] * ARCH[L1][COL], ARCH[L1][COL]},
        {ARCH[L2][ROW] * ARCH[L2][COL], ARCH[L2][ROW]},
        {ARCH[L3][ROW] * ARCH[L3][COL], ARCH[L3][ROW]},
        {ARCH[L4][ROW] * ARCH[L4][COL], ARCH[L4][ROW]},
    };

    struct alignas(64) Network {
        float w1[ ARCH_DIMENSIONS[L1][W] ];
        float b1[ ARCH_DIMENSIONS[L1][B] ];
        float w2[ ARCH_DIMENSIONS[L2][W] ];
        float b2[ ARCH_DIMENSIONS[L2][B] ];
        float w3[ ARCH_DIMENSIONS[L3][W] ];
        float b3[ ARCH_DIMENSIONS[L3][B] ];
        float w4[ ARCH_DIMENSIONS[L4][W] ];
        float b4[ ARCH_DIMENSIONS[L4][B] ];
    };

    Network network_;
    alignas(64) float accumulator_[ARCH[L1][COL]];
};

struct NNUEHeader {
    char magic[4]; // "NNUE"
    uint version;
};

inline bool NNUE::load(const std::string& filepath) {

    std::ifstream file(filepath, std::ios::binary);
    NNUEHeader header;

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&network_), sizeof(Network));

    return !file.good() || header.version != version || std::string(header.magic, 4) != "NNUE";
}

inline void NNUE::set_accumulator(chess::Square sq, chess::Piece piece, chess::Color color) noexcept {
    add<ARCH[L1][COL]>(accumulator_, &network_.w1[make_index(sq, piece, color) * ARCH[L1][COL]]);
}

inline void NNUE::pop_accumulator(chess::Square sq, chess::Piece piece, chess::Color color) noexcept {
    sub<ARCH[L1][COL]>(accumulator_, &network_.w1[make_index(sq, piece, color) * ARCH[L1][COL]]);
}

inline void NNUE::refresh_accumulator(const chess::Position& pos) noexcept {

    // Start with bias
    std::copy(network_.b1, network_.b1 + ARCH[L1][COL], accumulator_);

    // Add all active features
    auto occupied = pos.occupied();
    for (chess::Bitboard::ChunkID chunk_id = 0; chunk_id < chess::CHUNK_NB; chunk_id++) {
        auto& chunk = occupied.chunk(chunk_id);
        while(chunk) {
            const auto square = chess::Bitboard::pop_lsb(chunk, chunk_id).id();
            const auto piece = pos.board(square).piece().id();
            const auto color = pos.board(square).color().id();
            set_accumulator(square, piece, color);
        }
    }
}

inline Score NNUE::evaluate() const noexcept {

    alignas(64) float o1[ARCH[L1][COL]];
    alignas(64) float o2[ARCH[L2][ROW]];
    alignas(64) float o3[ARCH[L3][ROW]];
                float o4[ARCH[L4][ROW]];

    // Layer 1: 3840 -> 256 accumulator
    std::copy(accumulator_, accumulator_ + ARCH[L1][COL], o1);
    clipped_relu<ARCH[L1][COL]>(o1, 0.0f, 1.0f);

    // Layer 2: 256 -> 128
    affine<
        ARCH[L2][COL],
        ARCH[L2][ROW]
    >(
        o1,
        o2,
        network_.w2,
        network_.b2
    );

    clipped_relu<ARCH[L2][ROW]>(o2, 0.0f, 1.0f);

    // Layer 3: 128 -> 64
    affine<
        ARCH[L3][COL],
        ARCH[L3][ROW]
    >(
        o2,
        o3,
        network_.w3,
        network_.b3
    );

    clipped_relu<ARCH[L3][ROW]>(o3, 0.0f, 1.0f);

    // Layer 4: 64 -> 1
    affine<
        ARCH[L4][COL],
        ARCH[L4][ROW]
    >(
        o3,
        o4,
        network_.w4,
        network_.b4
    );

    return Score(o4[0]);
}

} // namespace athena