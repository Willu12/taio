#include "core.hpp"
#include <vector>
namespace core
{

Multigraph::Multigraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix) {
    this->adjacencyMatrix = adjacencyMatrix;
}

Multigraph::Multigraph(const core::Multigraph& multiGraph) {
    this->adjacencyMatrix = multiGraph.adjacencyMatrix;
}
std::size_t Multigraph::vertexCount() const {
    return this->adjacencyMatrix.size();
}

std::size_t Multigraph::size() const {
    std::size_t size = 0;
    for (const auto& row : adjacencyMatrix) {
        for (const auto& edges : row) {
            size += edges;
        }
    }
    return size;
}

std::vector<vertex> Multigraph::getNeighbours(vertex v) const {
    std::vector<vertex> neighbours;
    for (int i = 0; i < this->adjacencyMatrix[v].size(); i++) {
        if (this->adjacencyMatrix[v][i] > 0) neighbours.push_back(i);
    }
    return neighbours;
}

Multigraph::Multigraph(std::size_t size) : adjacencyMatrix(size, std::vector<vertex>(size, 0)) {
}

void Multigraph::addEdge(vertex u, vertex v) {
    this->adjacencyMatrix[u][v] = this->adjacencyMatrix[u][v] + 1;
}

bool Multigraph::hasEdge(vertex u, vertex v) const {
    return this->adjacencyMatrix[u][v] > 0;
}

Multigraph Multigraph::inducedSubgraph(const std::vector<vertex>& vertices) const {
    auto G = Multigraph(vertices.size());
    for (vertex v = 0; v < vertices.size(); v++) {
        for (vertex u = 0; u < vertices.size(); u++) {
            for (int i = 0; i < adjacencyMatrix[vertices[v]][vertices[u]]; i++) {
                G.addEdge(v, u);
            }
        }
    }
    return G;
}

void Multigraph::removeAllEdges(vertex v) {
    for (vertex u = 0; u < this->vertexCount(); u++) {
        adjacencyMatrix[v][u] = adjacencyMatrix[u][v] = 0;
    }
}

Multigraph Multigraph::kGraph(unsigned int k) const {

    Multigraph G = Multigraph(*this);
    for (vertex v = 0; v < G.vertexCount(); v++) {
        for (vertex u = 0; u < G.vertexCount(); u++) {
            if (G.adjacencyMatrix[v][u] < k) G.adjacencyMatrix[v][u] = 0;
        }
    }
    return G;
}

std::vector<std::vector<std::size_t>> Multigraph::getAdjacencyMatrix() const {
    return this->adjacencyMatrix;
}

} // namespace core