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
    multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix);
    std::size_t size() const;
};

} // namespace core
