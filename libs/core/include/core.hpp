#pragma once

#include <cstddef>
#include <vector>

typedef std::size_t vertex;

namespace core
{
class multiGraph {
  private:
    std::vector<std::vector<std::size_t>> adjacencyMatrix;

  public:
    multiGraph(const core::multiGraph& multiGraph);
    multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix);
    std::size_t size() const;
    std::vector<vertex> getNeighbours(vertex v);
};

} // namespace core
