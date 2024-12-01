#include "benchmark/benchmark.h"
#include "core.hpp"
#include "hamilton.hpp"

static void Setup(const benchmark::State& state) {
    srand(100);
}

static void BM_hamiltonian_extension_exact(benchmark::State& state) {
    const auto n = state.range(0);
    const std::size_t k = n + 2;
    const auto G = core::Multigraph::random(n, n);

    for (auto _ : state) {
        auto extension = hamilton::findKHamiltonianExtension(k, G, false);
    }
}

static void BM_hamiltonian_extension_approximation(benchmark::State& state) {
    const auto n = state.range(0);
    const std::size_t k = n + 2;
    const auto G = core::Multigraph::random(n, n);

    for (auto _ : state) {
        auto extension = hamilton::findKHamiltonianExtension(k, G, true);
    }
}

BENCHMARK(BM_hamiltonian_extension_exact)->DenseRange(2, 24, 2)->Setup(Setup);
BENCHMARK(BM_hamiltonian_extension_approximation)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
