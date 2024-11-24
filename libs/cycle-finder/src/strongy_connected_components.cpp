#include "core.hpp"
#include "strongly_connected_components.hpp"
#include <algorithm>
#include <vector>

namespace cycleFinder
{
StronglyConnectedComponents::StronglyConnectedComponents(const core::multiGraph& multiGraph) : multiGraph(multiGraph) {
    this->low = std::vector<vertex>(multiGraph.size());
    this->visited = std::vector<bool>(multiGraph.size());
    this->onStack = std::set<vertex>();
    this->stack = std::stack<vertex>();
    this->visitedTime = std::vector<unsigned int>(multiGraph.size());
    this->time = 0;
}

std::vector<std::vector<vertex>> StronglyConnectedComponents::solve() {
    for (vertex v = 0; v < multiGraph.vertexCount(); v++) {
        if (!visited[v]) processVertex(v);
    }
    return stronglyConnectedComponents;
}

void StronglyConnectedComponents::processVertex(vertex v) {
    visited[v] = true;
    visitedTime[v] = time;

    low[v] = time++;
    stack.push(v);
    onStack.insert(v);

    for (vertex child : multiGraph.getNeighbours(v)) {
        if (visited[child] == false) {
            processVertex(child);
            low[v] = std::min(low[v], low[child]);
        } else if (onStack.contains(child)) {
            low[v] = std::min(low[v], low[child]);
        }
    }
    if (visitedTime[v] != low[v]) return;
    auto scc = std::vector<vertex>();

    vertex u;
    u = stack.top();
    while (low[u] == low[v] && stack.size() > 0) {
        u = stack.top();
        stack.pop();
        onStack.erase(u);
        scc.push_back(u);
    };

    stronglyConnectedComponents.push_back(scc);
}

} // namespace cycleFinder