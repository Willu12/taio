#include "core.hpp"
#include <vector>
namespace core
{

multiGraph::multiGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix) {
    this->adjacencyMatrix = adjacencyMatrix;
}

multiGraph::multiGraph(const core::multiGraph& multiGraph) {
    this->adjacencyMatrix = multiGraph.adjacencyMatrix;
}
std::size_t multiGraph::vertexCount() const {
    return this->adjacencyMatrix.size();
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
    for (int i = 0; i < this->adjacencyMatrix[v].size(); i++) {
        if (this->adjacencyMatrix[v][i] > 0) neighbours.push_back(i);
    }
    return neighbours;
}

multiGraph::multiGraph(std::size_t size) {
    auto rows = std::vector<std::vector<vertex>>(size);
    for (int i = 0; i < rows.size(); i++) {
        auto row = std::vector<vertex>(size);
        for (int j = 0; j < row.size(); j++) {
            row[j] = 0;
        }
        rows[i] = row;
    }
    adjacencyMatrix = rows;
}

void multiGraph::addEdge(vertex u, vertex v) {
    this->adjacencyMatrix[u][v] = this->adjacencyMatrix[u][v] + 1;
}

bool multiGraph::hasEdge(vertex u, vertex v) const {
    return this->adjacencyMatrix[u][v] > 0;
}

multiGraph multiGraph::inducedSubgraph(const std::vector<vertex>& vertices) const {
    auto G = multiGraph(vertices.size());
    for (vertex v = 0; v < vertices.size(); v++) {
        for (vertex u = 0; u < vertices.size(); u++) {
            for (int i = 0; i < adjacencyMatrix[vertices[v]][vertices[u]]; i++) {
                G.addEdge(v, u);
            }
        }
    }
    return G;
}

void multiGraph::removeAllEdges(vertex v) {
    for (vertex u = 0; u < this->vertexCount(); u++) {
        adjacencyMatrix[v][u] = adjacencyMatrix[u][v] = 0;
    }
}

multiGraph multiGraph::KGraph(unsigned int k) const {

    multiGraph G = multiGraph(*this);
    for (vertex v = 0; v < G.vertexCount(); v++) {
        for (vertex u = 0; u < G.vertexCount(); u++) {
            if (G.adjacencyMatrix[v][u] < k) G.adjacencyMatrix[v][u] = 0;
        }
    }
    return G;
}

std::vector<std::vector<std::size_t>> multiGraph::getAdjacencyMatrix() const {
    return this->adjacencyMatrix;
}

} // namespace core