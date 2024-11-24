#pragma once

#include <cstddef>
#include <vector>

namespace core
{
class multiGraph {
  private:
    std::vector<std::vector<std::size_t>> adjacencyMatrix;

  public:
    multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix);

    std::size_t vertexCount() const;
    std::size_t edgeCount(std::size_t from, std::size_t to) const;
    std::size_t outDegree(std::size_t vertex) const;

    std::size_t size() const;
};

} // namespace core
