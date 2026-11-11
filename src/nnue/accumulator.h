#pragma once

#include <cstring>
#include <cstdint>
#include "chess/color.h"
#include "chess/constants.h"
#include "nnue/feature.h"
#include "nnue/network.h"
#include "nnue/layers.h"

namespace athena::nnue {

class Accumulator {
public:
    void reset(const Network& net) noexcept {
        std::memcpy(data_, net.b1, sizeof(data_));
    }

    void add_feature(const Network& net, Feature feature) noexcept { add<ARCH[L1][COL]>(data_, net.w1[feature.id()]); }
    void sub_feature(const Network& net, Feature feature) noexcept { sub<ARCH[L1][COL]>(data_, net.w1[feature.id()]); }

    auto* data() noexcept { return data_; }
    const auto* data() const noexcept { return data_; }

private:
    int16_t data_[ARCH[L1][COL]];
};

class AccumulatorStack {
public:
    void push() noexcept { ++play_; std::memcpy(accumulators_[play_], accumulators_[play_ - 1], sizeof(accumulators_[play_])); }
    void pop()  noexcept { --play_; }

    Accumulator& operator[](chess::Color color) noexcept {
        return accumulators_[play_][color.id()];
    }

    const Accumulator& self(chess::Color turn) const noexcept { return accumulators_[play_][turn.self().id()]; }
    const Accumulator& next(chess::Color turn) const noexcept { return accumulators_[play_][turn.next().id()]; }
    const Accumulator& ally(chess::Color turn) const noexcept { return accumulators_[play_][turn.ally().id()]; }
    const Accumulator& prev(chess::Color turn) const noexcept { return accumulators_[play_][turn.prev().id()]; }

private:
    int play_ = 0; // chess::Position::Play
    alignas(64) Accumulator accumulators_[chess::PLAY_NB][chess::COLOR_NB];
};

} // namespace athena
