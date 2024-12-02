#include "benchmark/benchmark.h"
#include "core.hpp"
#include "max_cycle_solver.hpp"

static void Setup(const benchmark::State& state) {
    srand(100);
}

static void BM_max_cycle_exact_changing_n(benchmark::State& state) {
    auto solver = cycleFinder::MaxCycleSolver();
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto dist = solver.solve(G, 1);
    }
}

static void BM_max_cycle_approximation_changing_n(benchmark::State& state) {
    auto solver = cycleFinder::MaxCycleSolver();
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto dist = solver.approximate(G, 1);
    }
}

static void BM_max_cycle_approximation_changing_k(benchmark::State& state) {
    auto solver = cycleFinder::MaxCycleSolver();
    const auto n = 1000;
    const auto k = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto dist = solver.approximate(G, k);
    }
}

static void BM_max_cycle_exact_changing_k(benchmark::State& state) {
    auto solver = cycleFinder::MaxCycleSolver();
    const auto n = 8;
    const auto k = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto dist = solver.solve(G, k);
    }
}

BENCHMARK(BM_max_cycle_approximation_changing_n)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_max_cycle_exact_changing_n)->DenseRange(2, 15)->Setup(Setup);

BENCHMARK(BM_max_cycle_approximation_changing_k)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_max_cycle_exact_changing_k)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
