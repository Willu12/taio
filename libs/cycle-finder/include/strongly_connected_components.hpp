#pragma once
#include "core.hpp"
#include <stack>
#include <vector>
#include <set>
namespace cycleFinder
{
class StronglyConnectedComponents {
  private:
    std::vector<vertex> low;
    std::set<vertex> onStack;
    std::stack<vertex> stack;
    std::vector<bool> visited;
    std::vector<unsigned int> visitedTime;
    std::vector<std::vector<vertex>> stronglyConnectedComponents;
    core::multiGraph multiGraph;
    unsigned int time;

    void processVertex(vertex v);

  public:
    StronglyConnectedComponents(const core::multiGraph& multiGraph);
    std::vector<std::vector<vertex>> solve();
};
} // namespace cycleFinder