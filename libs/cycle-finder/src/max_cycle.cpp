#include "core.hpp"
#include "max_cycle.hpp"
#include "strongly_connected_components.hpp"
#include <algorithm>
#include <vector>

namespace cycleFinder
{
MaxCycle::MaxCycle(const core::multiGraph& multiGraph, unsigned int k)
    : multiGraph(multiGraph), k(k), stronglyConnectedComponentsFinder(multiGraph) {
    this->multiGraph = multiGraph.KGraph(k);
    this->maxCycleSize = 0;
}

std::vector<std::vector<vertex>> MaxCycle::approximate() {
    auto cycles = std::vector<std::vector<vertex>>();
    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();

    std::size_t maxSize = 0;
    for (const auto& stronglyConnectedComponent : stronglyConnectedComponents)
        maxSize = maxSize > stronglyConnectedComponent.size() ? maxSize : stronglyConnectedComponent.size();

    for (const auto& stronglyConnectedComponent : stronglyConnectedComponents)
        if (stronglyConnectedComponent.size() == maxSize) cycles.push_back(stronglyConnectedComponent);
    return cycles;
}

std::vector<std::vector<vertex>> MaxCycle::solve() {

    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();
    auto filteredSCC = std::vector<std::vector<vertex>>();
    for (const auto& scc : stronglyConnectedComponents) {
        if (scc.size() > 1) filteredSCC.push_back(scc);
    }
    std::sort(stronglyConnectedComponents.begin(), stronglyConnectedComponents.end(),
              [](const std::vector<vertex>& a, const std::vector<vertex>& b) { return a.size() > b.size(); });

    while (filteredSCC.empty() == false) {
        auto firstSCC = filteredSCC.front();
        if (firstSCC.size() <= maxCycleSize) break;
        std::sort(firstSCC.begin(), firstSCC.end());
        processStronglyConnectedComponent(firstSCC);
        leastVertex = firstSCC[0];
        multiGraph.removeAllEdges(leastVertex);
        stronglyConnectedComponentsFinder = StronglyConnectedComponents(multiGraph);
        stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();
        filteredSCC.clear();
        for (const auto& scc : stronglyConnectedComponents) {
            if (scc.size() > 1) filteredSCC.push_back(scc);
        }
    }

    filterMaxCycles();
    return maxCycles;
}

void MaxCycle::processStronglyConnectedComponent(const std::vector<vertex>& scc) {
    blocked.clear();
    blockedMap.clear();
    stack.clear();

    auto G = multiGraph.inducedSubgraph(scc);
    leastVertex = 0;
    processVertex(leastVertex, G, scc);
}

bool MaxCycle::processVertex(vertex v, const core::multiGraph& multiGraph, const std::vector<vertex>& scc) {
    bool foundCycle = false;
    stack.push_back(v);
    blocked.insert(v);

    for (auto neigbhour : multiGraph.getNeighbours(v)) {
        if (neigbhour == leastVertex) {
            std::vector<vertex> cycle = std::vector<vertex>(stack.size() + 1);
            for (auto v = 0; v < stack.size(); v++) {
                cycle[v] = scc[stack[v]];
            }
            cycle[cycle.size() - 1] = scc[leastVertex];
            if (cycle.size() > maxCycleSize) maxCycleSize = cycle.size();

            cycles.push_back(cycle);
            foundCycle = true;

        } else if (!blocked.contains(neigbhour))
            foundCycle |= processVertex(neigbhour, multiGraph, scc);
    }

    if (foundCycle) {
        unblockVertex(v);
    }

    else {
        for (auto neighbour : multiGraph.getNeighbours(v)) {
            // add them to blocked map;
            auto currentNeighbourBlockings =
                blockedMap.contains(neighbour) ? blockedMap[neighbour] : std::vector<vertex>();
            currentNeighbourBlockings.push_back(v);
            blockedMap.insert({neighbour, currentNeighbourBlockings});
        }
    }
    stack.pop_back();
    return foundCycle;
}

void MaxCycle::unblockVertex(vertex v) {
    blocked.erase(v);
    for (auto u : blockedMap[v]) {
        if (blocked.contains(u)) {
            unblockVertex(u);
        }
    }
    blockedMap.erase(v);
}

void MaxCycle::filterMaxCycles() {

    for (const auto& cycle : cycles) {
        if (cycle.size() == maxCycleSize) maxCycles.push_back(cycle);
    }
}

} // namespace cycleFinder