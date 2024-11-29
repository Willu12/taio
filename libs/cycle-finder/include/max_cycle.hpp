#pragma once
#include "core.hpp"
#include "strongly_connected_components.hpp"
#include <unordered_map>
#include <vector>
namespace cycleFinder
{
class MaxCycle {
  private:
    std::vector<std::vector<vertex>> cycles_;
    std::vector<std::vector<vertex>> maxCycles_;
    std::vector<std::vector<vertex>> maxCyclesExact_;
    std::vector<vertex> stack_;
    std::set<vertex> blocked_;
    std::unordered_map<vertex, std::vector<vertex>> blockedMap_;

    core::Size maxCycleSizeExact_ = core::Size{0, 0, 0};
    const core::Multigraph baseMultiGraph_;
    core::Multigraph multiGraph_;
    StronglyConnectedComponents stronglyConnectedComponentsFinder_;
    vertex leastVertex_;
    unsigned int k_;
    std::size_t maxCycleSize_ = 0;
    void processStronglyConnectedComponent(const std::vector<vertex>& stronglyConnectedComponent);

    void unblockVertex(vertex v);
    bool processVertex(vertex v, const core::Multigraph& multiGraph, const std::vector<vertex>& scc);
    void filterMaxCycles();
    void filterMaxCyclesExact();

  public:
    std::vector<std::vector<vertex>> getMaxVertexCycle();
    MaxCycle(const core::Multigraph& multiGraph, unsigned int k);
    std::vector<std::vector<vertex>> solve();
    std::vector<std::vector<vertex>> approximate();
    core::Size getMaxSize();
};
} // namespace cycleFinder