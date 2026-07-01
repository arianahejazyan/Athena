#include <benchmark/benchmark.h>
#include "chess/castle.h"
#include "chess/position.h"
#include "chess/perft.h"

using namespace athena;

static void BM_Perft_Modern(benchmark::State& state) {
    const int depth = state.range(0);
    const auto setup = chess::Castle::Setup::Modern;

    chess::Position base;
    base.set_setup(setup);
    base.init(chess::Position::startpos(setup));

    // Run one perft to get the node count (it's deterministic)
    chess::Position pos = base;
    const uint64_t nodes = chess::perft(pos, depth);

    for (auto _ : state) {
        chess::Position p = base;           // copy each iteration
        uint64_t n = chess::perft(p, depth);

        benchmark::DoNotOptimize(n);
        benchmark::ClobberMemory();
    }

    // === Counters set AFTER the loop ===
    
    // Nodes = nodes from ONE perft call
    state.counters["Nodes"] = nodes;

    // NPS = nodes_per_call / average_time_per_iteration
    // kIsIterationInvariantRate tells Benchmark that the value is per-iteration
    state.counters["NPS"] = benchmark::Counter(
        static_cast<double>(nodes),
        benchmark::Counter::kIsIterationInvariantRate
    );
}

BENCHMARK(BM_Perft_Modern)
    ->DenseRange(1, 5)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();