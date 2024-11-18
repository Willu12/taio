#include "core.hpp"
#include <vector>
#include <numeric>
namespace core
{

multiGraph::multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix) {
    this->adjacencyMatrix = adjacencyMatrix;
}

std::size_t multiGraph::size() const {
    std::size_t size = 0;
    for (const auto& row : adjacencyMatrix) {
        for (const auto& edges : row) {
            size += edges;
        }
    }
    return size;
}

} // namespace core