#include "core.hpp"
#include "max_cycle.hpp"
#include "strongly_connected_components.hpp"
#include <vector>

namespace cycleFinder
{
MaxCycle::MaxCycle(const core::multiGraph& multiGraph, unsigned int k)
    : multigraph(multiGraph), k(k), stronglyConnectedComponentsFinder(multiGraph) {
}

std::vector<std::vector<vertex>> MaxCycle::solve() {
    auto cycles = std::vector<std::vector<vertex>>();
    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();

    while (stronglyConnectedComponents.empty() == false) {
        leastVertex = stronglyConnectedComponents.front()[0];
        auto new_cycles = processStronglyConnectedComponent(stronglyConnectedComponents.front());
        multigraph.removeAllEdges(leastVertex);
        stronglyConnectedComponentsFinder = StronglyConnectedComponents(multigraph);
        stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();

        cycles.insert(cycles.end(), new_cycles.begin(), new_cycles.end());
    }

    return cycles;
}

void MaxCycle::processStronglyConnectedComponent(const std::vector<vertex>& scc) {
    blocked.clear();
    blockedMap.clear();
    stack.clear();
    auto G = multigraph.inducedSubgraph(scc);
}

bool MaxCycle::processVertex(vertex v, const core::multiGraph& multiGraph) {
    bool foundCycle = false;

    if (stack.empty() == false && v == leastVertex) {
        std::vector<vertex> cycle = std::vector<vertex>(stack.size() + 1);
        cycle.insert(cycle.begin(), stack.begin(), stack.end());
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

std::vector<vertex> getCycleFromStack(const std::stack<vertex>& stack) {
    // get all items from stack
}

} // namespace cycleFinder