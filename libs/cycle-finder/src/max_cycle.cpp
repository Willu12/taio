#include "core.hpp"
#include "max_cycle.hpp"
#include "strongly_connected_components.hpp"
#include <algorithm>
#include <vector>

namespace cycleFinder
{
MaxCycle::MaxCycle(const core::Multigraph& multiGraph, unsigned int k)
    : multiGraph_(multiGraph.kGraph(k)), k_(k), stronglyConnectedComponentsFinder_(multiGraph) {
}

std::vector<std::vector<vertex>> MaxCycle::approximate() {
    auto cycles = std::vector<std::vector<vertex>>();
    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder_.solve();

    std::size_t maxSize = 0;
    for (const auto& stronglyConnectedComponent : stronglyConnectedComponents)
        maxSize = maxSize > stronglyConnectedComponent.size() ? maxSize : stronglyConnectedComponent.size();

    for (const auto& stronglyConnectedComponent : stronglyConnectedComponents)
        if (stronglyConnectedComponent.size() == maxSize) cycles.push_back(stronglyConnectedComponent);
    return cycles;
}

std::vector<std::vector<vertex>> MaxCycle::solve() {

    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder_.solve();
    auto filteredSCC = std::vector<std::vector<vertex>>();
    for (const auto& scc : stronglyConnectedComponents) {
        if (scc.size() > 1) filteredSCC.push_back(scc);
    }
    std::sort(stronglyConnectedComponents.begin(), stronglyConnectedComponents.end(),
              [](const std::vector<vertex>& a, const std::vector<vertex>& b) { return a.size() > b.size(); });

    while (filteredSCC.empty() == false) {
        auto firstSCC = filteredSCC.front();
        if (firstSCC.size() <= maxCycleSize_) break;
        std::sort(firstSCC.begin(), firstSCC.end());
        processStronglyConnectedComponent(firstSCC);
        leastVertex_ = firstSCC[0];
        multiGraph_.removeAllEdges(leastVertex_);
        stronglyConnectedComponentsFinder_ = StronglyConnectedComponents(multiGraph_);
        stronglyConnectedComponents = stronglyConnectedComponentsFinder_.solve();
        filteredSCC.clear();
        for (const auto& scc : stronglyConnectedComponents) {
            if (scc.size() > 1) filteredSCC.push_back(scc);
        }
    }

    filterMaxCycles();
    return maxCycles_;
}

void MaxCycle::processStronglyConnectedComponent(const std::vector<vertex>& scc) {
    blocked_.clear();
    blockedMap_.clear();
    stack_.clear();

    auto G = multiGraph_.inducedSubgraph(scc);
    leastVertex_ = 0;
    processVertex(leastVertex_, G, scc);
}

bool MaxCycle::processVertex(vertex v, const core::Multigraph& multiGraph, const std::vector<vertex>& scc) {
    bool foundCycle = false;
    stack_.push_back(v);
    blocked_.insert(v);

    for (auto neighbour : multiGraph.getNeighbours(v)) {
        if (neighbour == 0) {
            foundCycle = true;

            std::vector<vertex> cycle = std::vector<vertex>(stack_.size() + 1);
            for (auto v = 0; v < stack_.size(); v++) {
                cycle[v] = scc[stack_[v]];
            }
            cycle[cycle.size() - 1] = scc[0];
            if (cycle.size() >= maxCycleSize_) {
                maxCycleSize_ = cycle.size();
                cycles_.push_back(cycle);
            }

        } else if (!blocked_.contains(neighbour))
            foundCycle |= processVertex(neighbour, multiGraph, scc);
    }

    if (foundCycle) {
        unblockVertex(v);
    }

    else {
        for (auto neighbour : multiGraph.getNeighbours(v)) {
            // add them to blocked map;
            auto currentNeighbourBlockings =
                blockedMap_.contains(neighbour) ? blockedMap_[neighbour] : std::vector<vertex>();
            currentNeighbourBlockings.push_back(v);
            blockedMap_.insert({neighbour, currentNeighbourBlockings});
        }
    }
    stack_.pop_back();
    return foundCycle;
}

void MaxCycle::unblockVertex(vertex v) {
    blocked_.erase(v);
    for (auto u : blockedMap_[v]) {
        if (blocked_.contains(u)) {
            unblockVertex(u);
        }
    }
    blockedMap_.erase(v);
}

void MaxCycle::filterMaxCycles() {

    for (const auto& cycle : cycles_) {
        if (cycle.size() == maxCycleSize_) maxCycles_.push_back(cycle);
    }
}

} // namespace cycleFinder