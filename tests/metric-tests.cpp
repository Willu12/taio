#include "benchmark/benchmark.h"
#include "core.hpp"
#include "metric.hpp"

static void Setup(const benchmark::State& state) {
    srand(100);
}

static void BM_ExactMetric(benchmark::State& state) {
    metric::ExactMetric distFun;
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);
    const auto H = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto _ = distFun(G, H);
    }
}

static void BM_HeuristicMetric_ComparisonSort_DegreeLinear(benchmark::State& state) {
    metric::HeuristicMetric distFun(false);
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);
    const auto H = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto _ = distFun(G, H);
    }
}

static void BM_HeuristicMetric_CountingSort_DegreeLinear(benchmark::State& state) {
    metric::HeuristicMetric distFun(true);
    const auto n = state.range(0);
    const auto G = core::Multigraph::random(n, n * n);
    const auto H = core::Multigraph::random(n, n * n);

    for (auto _ : state) {
        auto _ = distFun(G, H);
    }
}

static void BM_HeuristicMetric_ComparisonSort_DegreeConstant(benchmark::State& state) {
    metric::HeuristicMetric distFun(false);
    const auto n = state.range(0);
    const auto G = core::DegreeTrackingGraph(core::Multigraph::random(n, n * n));
    const auto H = core::DegreeTrackingGraph(core::Multigraph::random(n, n * n));

    for (auto _ : state) {
        auto _ = distFun(G, H);
    }
}

static void BM_HeuristicMetric_CountingSort_DegreeConstant(benchmark::State& state) {
    metric::HeuristicMetric distFun(true);
    const auto n = state.range(0);
    const auto G = core::DegreeTrackingGraph(core::Multigraph::random(n, n * n));
    const auto H = core::DegreeTrackingGraph(core::Multigraph::random(n, n * n));

    for (auto _ : state) {
        auto _ = distFun(G, H);
    }
}

BENCHMARK(BM_ExactMetric)->DenseRange(2, 11)->Setup(Setup);
BENCHMARK(BM_HeuristicMetric_ComparisonSort_DegreeLinear)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_HeuristicMetric_ComparisonSort_DegreeConstant)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_HeuristicMetric_CountingSort_DegreeLinear)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
BENCHMARK(BM_HeuristicMetric_CountingSort_DegreeConstant)->RangeMultiplier(2)->Range(8, 4096)->Setup(Setup);
