#pragma once
#include "core.hpp"
#include "strongly_connected_components.hpp"
#include <stack>
#include <unordered_map>
#include <vector>
namespace cycleFinder
{
class MaxCycle {
  private:
    std::vector<std::vector<vertex>> cycles;
    std::stack<vertex> stack;
    std::set<vertex> blocked;
    std::unordered_map<vertex, vertex> blockedMap;

    core::multiGraph multigraph;
    StronglyConnectedComponents stronglyConnectedComponentsFinder;
    unsigned int k;
    std::vector<std::vector<vertex>> processStronglyConnectedComponent(
        const std::vector<vertex>& stronglyConnectedComponent);

  public:
    MaxCycle(const core::multiGraph& multiGraph, unsigned int k);
    std::vector<std::vector<vertex>> solve();
};
} // namespace cycleFinder