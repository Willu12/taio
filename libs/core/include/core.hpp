#pragma once

#include <cstddef>
#include <vector>

typedef std::size_t vertex;

namespace core
{
class Multigraph {
  private:
    std::vector<std::vector<std::size_t>> adjacencyMatrix;

  public:
    Multigraph(std::size_t size);
    Multigraph(const core::Multigraph& Multigraph);
    Multigraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix);
    std::size_t size() const;
    bool hasEdge(vertex u, vertex v) const;
    void addEdge(vertex u, vertex v);
    void removeAllEdges(vertex v);
    std::vector<vertex> getNeighbours(vertex v) const;
    Multigraph inducedSubgraph(const std::vector<vertex>& vertices) const;
    Multigraph kGraph(unsigned int k) const;
    std::vector<std::vector<std::size_t>> getAdjacencyMatrix() const;
    std::size_t vertexCount() const;
};

} // namespace core
