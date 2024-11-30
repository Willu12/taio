#include "core.hpp"
#include <vector>
#include <numeric>

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

std::size_t Multigraph::edgeCount(std::size_t from, std::size_t to) const {
    return adjacencyMatrix[from][to];
}

std::size_t Multigraph::outDegree(std::size_t vertex) const {
    return std::accumulate(adjacencyMatrix[vertex].begin(), adjacencyMatrix[vertex].end(), (std::size_t)0);
}

DegreeTrackingGraph::DegreeTrackingGraph(std::size_t size) : Multigraph(size), _outDegrees(size) {
    computeOutDegrees();
}
DegreeTrackingGraph::DegreeTrackingGraph(const core::Multigraph& multigraph)
    : Multigraph(multigraph), _outDegrees(multigraph.vertexCount()) {
    computeOutDegrees();
}
DegreeTrackingGraph::DegreeTrackingGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix)
    : Multigraph(adjacencyMatrix), _outDegrees(adjacencyMatrix.size()) {
    computeOutDegrees();
}

void DegreeTrackingGraph::computeOutDegrees() {
    for (std::size_t v = 0; v < vertexCount(); v++) {
        _outDegrees[v] = Multigraph::outDegree(v);
    }
}

void DegreeTrackingGraph::addEdge(vertex u, vertex v) {
    Multigraph::addEdge(u, v);
    _outDegrees[u]++;
}
void DegreeTrackingGraph::removeAllEdges(vertex v) {
    Multigraph::removeAllEdges(v);
    _outDegrees[v] = 0;
}

std::size_t DegreeTrackingGraph::outDegree(std::size_t vertex) const {
    return _outDegrees[vertex];
}

} // namespace core
