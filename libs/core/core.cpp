#include "core.hpp"
#include <vector>

namespace core
{

multiGraph::multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix) {
    this->adjacencyMatrix = adjacencyMatrix;
}
} // namespace core