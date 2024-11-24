#include "metric.hpp"

#include <numeric>
#include <algorithm>
#include <vector>
#include <ranges>

std::size_t metric::ExactMetric::operator()(const core::multiGraph& G, const core::multiGraph& H) const {
    if (G.vertexCount() < H.vertexCount()) return this->operator()(H, G);

    std::size_t vertexDifference = G.vertexCount() - H.vertexCount();

    std::size_t minEdgeDifference = -1;
    std::vector<std::size_t> vertexMap(G.vertexCount());
    std::iota(vertexMap.begin(), vertexMap.end(), 0);

    do {
        std::size_t edgeDifference = 0;
        for (std::size_t u = 0; u < G.vertexCount(); u++) {
            for (std::size_t v = 0; v < G.vertexCount(); v++) {
                auto uvEdgesG = G.edgeCount(u, v);
                auto uvEdgesH = H.edgeCount(vertexMap[u], vertexMap[v]);
                edgeDifference += uvEdgesG > uvEdgesH ? uvEdgesG - uvEdgesH : uvEdgesH - uvEdgesG;
            }
        }
        if (edgeDifference < minEdgeDifference) {
            minEdgeDifference = edgeDifference;
        }
    } while (std::next_permutation(vertexMap.begin(), vertexMap.end()));

    return vertexDifference + minEdgeDifference;
}

std::size_t metric::HeuristicMetric::operator()(const core::multiGraph& G, const core::multiGraph& H) const {
    if (G.vertexCount() < H.vertexCount()) return this->operator()(H, G);

    std::size_t n = G.vertexCount();
    std::size_t vertexDifference = G.vertexCount() - H.vertexCount();

    std::vector<std::size_t> degG(n);
    std::vector<std::size_t> degH(n, 0);

    for (std::size_t v = 0; v < n; v++) {
        degG[v] = G.outDegree(v);
        degH[v] = H.outDegree(v);
    }

    std::sort(degG.begin(), degG.end(), std::greater<>());
    std::sort(degH.begin(), degH.end(), std::greater<>());

    std::size_t edgeDifference = 0;
    for (std::size_t v = 0; v < n; v++) {
        edgeDifference += degG[v] > degH[v] ? degG[v] - degH[v] : degH[v] + degG[v];
    }

    return vertexDifference + edgeDifference;
}
