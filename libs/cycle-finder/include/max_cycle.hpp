#pragma once
#include "core.hpp"
#include "strongly_connected_components.hpp"
#include <unordered_map>
#include <vector>
namespace cycleFinder
{
class MaxCycle {
  private:
    std::vector<std::vector<vertex>> cycles;
    std::vector<std::vector<vertex>> maxCycles;
    std::vector<vertex> stack;
    std::set<vertex> blocked;
    std::unordered_map<vertex, std::vector<vertex>> blockedMap;

    core::multiGraph multiGraph;
    StronglyConnectedComponents stronglyConnectedComponentsFinder;
    vertex leastVertex;
    unsigned int k;
    void processStronglyConnectedComponent(const std::vector<vertex>& stronglyConnectedComponent);

    void unblockVertex(vertex v);
    bool processVertex(vertex v, const core::multiGraph& multiGraph);
    void filterMaxCycles();

  public:
    MaxCycle(const core::multiGraph& multiGraph, unsigned int k);
    std::vector<std::vector<vertex>> solve();
    std::vector<std::vector<vertex>> approximate();
};
} // namespace cycleFinder