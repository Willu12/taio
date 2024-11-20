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

std::vector<vertex> multiGraph::getNeighbours(vertex v) const {
    std::vector<vertex> neighbours;
    for (auto u : this->adjacencyMatrix[v]) {
        if (u > 0) neighbours.push_back(u);
    }
    return neighbours;
}

multiGraph::multiGraph(std::size_t size) {
    auto rows = std::vector<std::vector<vertex>>(size);
    for (auto row : rows) {
        row = std::vector<vertex>(size);
    }
}

void multiGraph::addEdge(vertex u, vertex v) {
    this->adjacencyMatrix[u][v]++;
}

bool multiGraph::hasEdge(vertex u, vertex v) const {
    return this->adjacencyMatrix[u][v] > 0;
}

multiGraph multiGraph::inducedSubgraph(const std::vector<vertex>& vertices) const {
    auto G = multiGraph(vertices.size());
    for (vertex v : vertices) {
        for (vertex u : vertices) {
            for (int i = 0; i < adjacencyMatrix[v][u]; i++) {
                G.addEdge(v, u);
            }
        }
    }
    return G;
}

void multiGraph::removeAllEdges(vertex v) {
    for (vertex u = 0; u < this->size(); u++) {
        adjacencyMatrix[v][u] = adjacencyMatrix[u][v] = 0;
    }
}

multiGraph multiGraph::KGraph(unsigned int k) const {

    multiGraph G = multiGraph(*this);
    for (vertex v = 0; v < G.size(); v++) {
        for (vertex u = 0; u < G.size(); u++) {
            if (G.adjacencyMatrix[v][u] < k) G.adjacencyMatrix[v][u] = 0;
        }
    }
    return G;
}

} // namespace core