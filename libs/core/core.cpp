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

std::size_t multiGraph::vertexCount() const {
    return adjacencyMatrix.size();
}

std::size_t multiGraph::edgeCount(std::size_t from, std::size_t to) const {
    if (from >= vertexCount() || to >= vertexCount()) return 0;
    return adjacencyMatrix[from][to];
}

std::size_t multiGraph::outDegree(std::size_t vertex) const {
    if (vertex >= vertexCount()) return 0;
    return std::accumulate(adjacencyMatrix[vertex].begin(), adjacencyMatrix[vertex].end(), (std::size_t)0);
}

} // namespace core
