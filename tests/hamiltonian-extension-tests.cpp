#include "benchmark/benchmark.h"
#include "core.hpp"
#include "hamilton.hpp"

static void Setup(const benchmark::State& state) {
    srand(100);
}

static void BM_hamiltonian_extension_exact_k_changing(benchmark::State& state) {
    const auto k = state.range(0);
    const std::size_t n = 15;
    const auto G = core::Multigraph::random(n, n);

    for (auto _ : state) {
        auto extension = hamilton::findKHamiltonianExtension(k, G, false);
    }
}

static void BM_hamiltonian_extension_exact_n_changing(benchmark::State& state) {
    const auto n = state.range(0);
    const std::size_t k = n + 2;
    const auto G = core::Multigraph::random(n, n);

    for (auto _ : state) {
        auto extension = hamilton::findKHamiltonianExtension(k, G, false);
    }
}

static void BM_hamiltonian_extension_approximation_k_changing(benchmark::State& state) {
    const auto k = state.range(0);
    const std::size_t n = 1024;
    const auto G = core::Multigraph::random(n, n);

    for (auto _ : state) {
        auto extension = hamilton::findKHamiltonianExtension(k, G, true);
    }
}

static void BM_hamiltonian_extension_approximation_n_changing(benchmark::State& state) {
    const auto n = state.range(0);
    const std::size_t k = n + 2;
    const auto G = core::Multigraph::random(n, n);

    for (auto _ : state) {
        auto extension = hamilton::findKHamiltonianExtension(k, G, true);
    }
}

BENCHMARK(BM_hamiltonian_extension_exact_k_changing)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_hamiltonian_extension_approximation_k_changing)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_hamiltonian_extension_exact_n_changing)->DenseRange(2, 24, 2)->Setup(Setup);
BENCHMARK(BM_hamiltonian_extension_approximation_n_changing)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
