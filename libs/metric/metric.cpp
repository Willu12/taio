#include "metric.hpp"

#include <numeric>
#include <algorithm>
#include <vector>
#include <ranges>

namespace metric
{

inline static std::size_t absDiff(std::size_t a, std::size_t b) {
    return a > b ? a - b : b - a;
}

std::size_t ExactMetric::operator()(const core::Multigraph& G, const core::Multigraph& H) const {
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

HeuristicMetric::HeuristicMetric(bool useCountingSort) : _useCountingSort(useCountingSort) {
}

std::size_t HeuristicMetric::operator()(const core::Multigraph& G, const core::Multigraph& H) const {
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

    if (_useCountingSort) return n - m + edgeDifferenceCount(degG, degH);

    return n - m + edgeDifferenceCompare(degG, degH);
}

std::size_t HeuristicMetric::edgeDifferenceCompare(std::vector<std::size_t>& degG,
                                                   std::vector<std::size_t>& degH) const {
    std::sort(degG.begin(), degG.end(), std::greater<>());
    std::sort(degH.begin(), degH.end(), std::greater<>());

    std::size_t edgeDifference = 0;
    for (std::size_t v = 0; v < degG.size(); v++) {
        edgeDifference += absDiff(degG[v], degH[v]);
    }
    return edgeDifference;
}

std::size_t HeuristicMetric::edgeDifferenceCount(const std::vector<std::size_t>& degG,
                                                 const std::vector<std::size_t>& degH) const {
    auto maxDegG = *std::max_element(degG.begin(), degG.end());
    auto maxDegH = *std::max_element(degH.begin(), degH.end());
    std::vector<std::size_t> degreeCounterG(maxDegG + 1, 0);
    std::vector<std::size_t> degreeCounterH(maxDegH + 1, 0);

    for (std::size_t v = 0; v < degG.size(); v++) {
        degreeCounterG[degG[v]]++;
        degreeCounterH[degH[v]]++;
    }

    std::size_t edgeDifference = 0;
    for (std::size_t degIterG = 0, degIterH = 0; degIterG <= maxDegG && degIterH <= maxDegH;
         degIterG += (degreeCounterG[degIterG] == 0), degIterH += (degreeCounterH[degIterH] == 0)) {

        if (degreeCounterG[degIterG] && degreeCounterH[degIterH]) {
            edgeDifference += absDiff(degIterG, degIterH);

            degreeCounterG[degIterG]--;
            degreeCounterH[degIterH]--;
        }
    }
    return edgeDifference;
}

} // namespace metric