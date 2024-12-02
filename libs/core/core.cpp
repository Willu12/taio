#include "core.hpp"
#include <vector>
#include <numeric>
#include <stdlib.h>

namespace core
{

Multigraph::Multigraph() {
}

Multigraph::Multigraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix) {
    this->adjacencyMatrix = adjacencyMatrix;
}

Multigraph::Multigraph(const core::Multigraph& multiGraph) {
    this->adjacencyMatrix = multiGraph.adjacencyMatrix;
}
std::size_t Multigraph::vertexCount() const {
    return this->adjacencyMatrix.size();
}

Size Multigraph::size() const {
    std::size_t edgeCount = 0;
    std::size_t maxDegree = 0;
    for (const auto& row : adjacencyMatrix) {
        for (const auto& edges : row) {
            edgeCount += edges;
            maxDegree = maxDegree > edges ? maxDegree : edges;
        }
    };
    return Size{adjacencyMatrix.size(), edgeCount, maxDegree};
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

Multigraph Multigraph::cycleGraph(const std::vector<vertex>& vertices) const {
    auto G = Multigraph(vertices.size() - 1);
    for (vertex v = 1; v < vertices.size() - 1; v++) {
        G.adjacencyMatrix[v - 1][v] = adjacencyMatrix[vertices[v - 1]][vertices[v]];
    }

    // add last edge
    G.adjacencyMatrix[vertices.size() - 2][0] = adjacencyMatrix[vertices[vertices.size() - 2]][vertices[0]];
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

Multigraph Multigraph::random(std::size_t vertexCount, std::size_t edgeCount) {
    auto graph = Multigraph(vertexCount);

    for (std::size_t i = 0; i < edgeCount; i++) {
        std::size_t u = rand() % vertexCount;
        std::size_t v = rand() % (vertexCount - 1);
        if (v >= u) v++;

        graph.addEdge(u, v);
    }

    return graph;
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
