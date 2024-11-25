#pragma once
#include "core.hpp"
#include <stack>
#include <vector>
#include <set>
namespace cycleFinder
{
class StronglyConnectedComponents {
  private:
    std::vector<vertex> low_;
    std::set<vertex> onStack_;
    std::stack<vertex> stack_;
    std::vector<bool> visited_;
    std::vector<unsigned int> visitedTime_;
    std::vector<std::vector<vertex>> stronglyConnectedComponents_;
    core::Multigraph multiGraph_;
    unsigned int time_ = 0;

    void processVertex(vertex v);

  public:
    StronglyConnectedComponents(const core::Multigraph& multiGraph_);
    std::vector<std::vector<vertex>> solve();
};
} // namespace cycleFinder