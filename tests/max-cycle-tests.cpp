#include "benchmark/benchmark.h"
#include "core.hpp"
#include "max_cycle_solver.hpp"

static void Setup(const benchmark::State& state) {
    srand(100);
}

static void BM_max_cycle_exact(benchmark::State& state) {
    auto solver = cycleFinder::MaxCycleSolver();
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto dist = solver.solve(G, 1);
    }
}

static void BM_max_cycle_approximation(benchmark::State& state) {
    auto solver = cycleFinder::MaxCycleSolver();
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto dist = solver.approximate(G, 1);
    }
}

BENCHMARK(BM_max_cycle_exact)->DenseRange(2, 11)->Setup(Setup);
BENCHMARK(BM_max_cycle_approximation)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
