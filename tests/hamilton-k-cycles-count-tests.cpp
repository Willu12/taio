#include "benchmark/benchmark.h"
#include "core.hpp"
#include "hamilton.hpp"
#include <cstddef>
#include <vector>

static void Setup(const benchmark::State& state) {
    srand(100);
}

static core::Multigraph generateKCompleteMG(std::size_t n, std::size_t k) {
    std::vector<std::vector<std::size_t>> adjacencyMatrix(n, std::vector<std::size_t>(n, k));

    for (int i = 0; i < n; ++i)
        adjacencyMatrix[i][i] = 0;

    return core::Multigraph(adjacencyMatrix);
}

static void BM_hamilton_k_cycles_approximation_changing_n(benchmark::State& state) {
    const auto n = state.range(0);
    const auto k = n + 2;
    core::Multigraph G = generateKCompleteMG(n, k);
    const std::size_t isolatedVertexId = n / 2; // doesn't matter
    G.removeAllEdges(isolatedVertexId);

    core::Multigraph extensionGraph(n);
    for (int i = 0; i < k; ++i) {
        extensionGraph.addEdge(1, isolatedVertexId);
        extensionGraph.addEdge(isolatedVertexId, n - 1);
    }

    auto GAdjacencyMatrix = G.getAdjacencyMatrix();
    auto ExtensionGraphAdjacencyMatrix = extensionGraph.getAdjacencyMatrix();

    for (auto _ : state) {
        auto dist = hamilton::findAllHamiltonianCycles(GAdjacencyMatrix, ExtensionGraphAdjacencyMatrix, k);
    }
}

static void BM_hamilton_k_cycles_approximation_changing_k(benchmark::State& state) {
    const auto k = state.range(0);
    const auto n = 1024;
    core::Multigraph G = generateKCompleteMG(n, k);
    const std::size_t isolatedVertexId = n / 2; // doesn't matter
    G.removeAllEdges(isolatedVertexId);

    core::Multigraph extensionGraph(n);
    for (int i = 0; i < k; ++i) {
        extensionGraph.addEdge(1, isolatedVertexId);
        extensionGraph.addEdge(isolatedVertexId, n - 1);
    }

    auto GAdjacencyMatrix = G.getAdjacencyMatrix();
    auto ExtensionGraphAdjacencyMatrix = extensionGraph.getAdjacencyMatrix();

    for (auto _ : state) {
        auto dist = hamilton::findAllHamiltonianCycles(GAdjacencyMatrix, ExtensionGraphAdjacencyMatrix, k);
    }
}

BENCHMARK(BM_hamilton_k_cycles_approximation_changing_k)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_hamilton_k_cycles_approximation_changing_n)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
