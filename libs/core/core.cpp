#include "core.hpp"
#include <vector>
#include <numeric>
namespace core
{

multiGraph::multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix) {
    this->adjacencyMatrix = adjacencyMatrix;
}

multiGraph::multiGraph(const core::multiGraph& multiGraph) {
    this->adjacencyMatrix = multiGraph.adjacencyMatrix;
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

std::vector<vertex> multiGraph::getNeighbours(vertex v) {
    std::vector<vertex> neighbours;
    for (auto u : this->adjacencyMatrix[v]) {
        if (u > 0) neighbours.push_back(u);
    }
    return neighbours;
}

} // namespace core