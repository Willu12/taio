#include "core.hpp"
#include "strongly_connected_components.hpp"
#include <algorithm>
#include <vector>

namespace cycleFinder
{
StronglyConnectedComponents::StronglyConnectedComponents(const core::Multigraph& multiGraph)
    : multiGraph_(multiGraph), low_(std::vector<vertex>(multiGraph.vertexCount())),
      visited_(std::vector<bool>(multiGraph.vertexCount())),
      visitedTime_(std::vector<unsigned int>(multiGraph.vertexCount())) {
}

std::vector<std::vector<vertex>> StronglyConnectedComponents::solve() {
    for (vertex v = 0; v < multiGraph_.vertexCount(); v++) {
        if (!visited_[v]) processVertex(v);
    }
    return stronglyConnectedComponents_;
}

void StronglyConnectedComponents::processVertex(vertex v) {
    visited_[v] = true;
    visitedTime_[v] = time_;

    low_[v] = time_++;
    stack_.push(v);
    onStack_.insert(v);

    for (vertex child : multiGraph_.getNeighbours(v)) {
        if (visited_[child] == false) {
            processVertex(child);
            low_[v] = std::min(low_[v], low_[child]);
        } else if (onStack_.contains(child)) {
            low_[v] = std::min(low_[v], low_[child]);
        }
    }

    if (visitedTime_[v] != low_[v]) return;
    auto scc = std::vector<vertex>();

    while (stack_.size() > 0 && low_[stack_.top()] == low_[v]) {
        auto u = stack_.top();

        stack_.pop();
        onStack_.erase(u);
        scc.push_back(u);
    };

    stronglyConnectedComponents_.push_back(scc);
}

} // namespace cycleFinder