#include "core.hpp"
#include "max_cycle.hpp"
#include "strongly_connected_components.hpp"
#include <vector>

namespace cycleFinder
{
MaxCycle::MaxCycle(const core::multiGraph& multiGraph, unsigned int k)
    : multiGraph(multiGraph), k(k), stronglyConnectedComponentsFinder(multiGraph) {
    this->multiGraph = multiGraph.KGraph(k);
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
    auto cycles = std::vector<std::vector<vertex>>();
    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();

    while (stronglyConnectedComponents.empty() == false) {
        leastVertex = stronglyConnectedComponents.front()[0];
        processStronglyConnectedComponent(stronglyConnectedComponents.front());
        multiGraph.removeAllEdges(leastVertex);
        stronglyConnectedComponentsFinder = StronglyConnectedComponents(multiGraph);
        stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();
    }

    filterMaxCycles();

    return maxCycles;
}

void MaxCycle::processStronglyConnectedComponent(const std::vector<vertex>& scc) {
    blocked.clear();
    blockedMap.clear();
    stack.clear();
    auto G = multiGraph.inducedSubgraph(scc);
    processVertex(leastVertex, G);
}

bool MaxCycle::processVertex(vertex v, const core::multiGraph& multiGraph) {
    bool foundCycle = false;

    if (stack.empty() == false && v == leastVertex) {
        std::vector<vertex> cycle = std::vector<vertex>(stack.size() + 1);
        std::copy(stack.begin(), stack.begin(), cycle.begin());
        cycle[cycle.size() - 1] = v;
        cycles.push_back(cycle);
        foundCycle = true;
    } else {
        stack.push_back(v);
        blocked.insert(v);

        for (auto neigbhour : multiGraph.getNeighbours(v)) {
            if (!blocked.contains(neigbhour)) foundCycle |= processVertex(neigbhour, multiGraph);
        }
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
}

void MaxCycle::filterMaxCycles() {
    std::size_t maxSize = 0;
    for (const auto& cycle : cycles)
        maxSize = maxSize > cycle.size() ? maxSize : cycle.size();

    for (const auto& cycle : cycles) {
        if (cycle.size() == maxSize) maxCycles.push_back(cycle);
    }
}

} // namespace cycleFinder