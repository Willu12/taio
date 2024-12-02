#ifndef FLOW_GRAPH_HPP
#define FLOW_GRAPH_HPP

#include <vector>
#include <cstddef>
#include <queue>
#include <limits>

namespace hamilton
{

const std::size_t INF = std::numeric_limits<std::size_t>::max();

class FlowGraph {
  public:
    std::vector<std::vector<std::size_t>> capacity;
    std::vector<std::vector<std::size_t>> flow;
    std::size_t vertexCount;

    FlowGraph(std::size_t n);

    void addEdge(std::size_t u, std::size_t v, std::size_t cap);

    bool bfs(std::size_t source, std::size_t sink, std::vector<std::size_t>& parent);

    std::size_t edmondsKarp(std::size_t source, std::size_t sink);
};

} // namespace hamilton

#endif // FLOW_GRAPH_HPP
