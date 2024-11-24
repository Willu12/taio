#include "metric.hpp"

#include <numeric>
#include <algorithm>
#include <vector>

inline static std::size_t absDiff(std::size_t a, std::size_t b) {
    return a > b ? a - b : b - a;
}

std::size_t metric::ExactMetric::operator()(const core::multiGraph& G, const core::multiGraph& H) const {
    if (G.vertexCount() < H.vertexCount()) return this->operator()(H, G);

    auto n = G.vertexCount();
    auto m = H.vertexCount();

    std::size_t minEdgeDifference = std::numeric_limits<std::size_t>::max();
    std::vector<std::size_t> vertexMap(n);
    std::iota(vertexMap.begin(), vertexMap.end(), 0);

    do {
        std::size_t edgeDifference = 0;
        for (std::size_t u = 0; u < n; u++) {
            for (std::size_t v = 0; v < n; v++) {
                edgeDifference += vertexMap[u] < m && vertexMap[v] < m
                                      ? absDiff(G.edgeCount(u, v), H.edgeCount(vertexMap[u], vertexMap[v]))
                                      : G.edgeCount(u, v);
            }
        }
        if (edgeDifference < minEdgeDifference) minEdgeDifference = edgeDifference;
    } while (std::next_permutation(vertexMap.begin(), vertexMap.end()));

    return n - m + minEdgeDifference;
}

std::size_t metric::HeuristicMetric::operator()(const core::multiGraph& G, const core::multiGraph& H) const {
    if (G.vertexCount() < H.vertexCount()) return this->operator()(H, G);

    auto n = G.vertexCount();
    auto m = H.vertexCount();

    std::vector<std::size_t> degG(n);
    std::vector<std::size_t> degH(n);

    for (std::size_t v = 0; v < m; v++) {
        degG[v] = G.outDegree(v);
        degH[v] = H.outDegree(v);
    }

    for (std::size_t v = m; v < n; v++) {
        degG[v] = G.outDegree(v);
        degH[v] = 0;
    }

    std::sort(degG.begin(), degG.begin() + n, std::greater<>());
    std::sort(degH.begin(), degH.begin() + m, std::greater<>());

    std::size_t edgeDifference = 0;
    for (std::size_t v = 0; v < n; v++) {
        edgeDifference += absDiff(degG[v], degH[v]);
    }

    return n - m + edgeDifference;
}
