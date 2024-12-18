#pragma once

#include <cstddef>
#include <vector>

typedef std::size_t vertex;

namespace core
{
struct Size {
  public:
    std::size_t vertexCount;
    std::size_t edgeCount;
    std::size_t maxOutDegree;

    bool operator>(const Size& other) const {
        if (vertexCount != other.vertexCount) {
            return vertexCount > other.vertexCount;
        }
        if (edgeCount != other.edgeCount) {
            return edgeCount > other.edgeCount;
        }
        return maxOutDegree > other.maxOutDegree;
    }

    bool operator==(const Size& other) const {
        return vertexCount == other.vertexCount && edgeCount == other.edgeCount && maxOutDegree == other.maxOutDegree;
    }
};

class Multigraph {
  private:
    std::vector<std::vector<std::size_t>> adjacencyMatrix;

  public:
    Multigraph();
    Multigraph(std::size_t size);
    Multigraph(const core::Multigraph& Multigraph);
    Multigraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix);

    std::size_t vertexCount() const;
    std::size_t edgeCount(std::size_t from, std::size_t to) const;
    virtual std::size_t outDegree(std::size_t vertex) const;

    Size size() const;
    bool hasEdge(vertex u, vertex v) const;
    virtual void addEdge(vertex u, vertex v);
    virtual void removeAllEdges(vertex v);
    std::vector<vertex> getNeighbours(vertex v) const;
    Multigraph inducedSubgraph(const std::vector<vertex>& vertices) const;
    Multigraph cycleGraph(const std::vector<vertex>& vertices) const;
    Multigraph kGraph(unsigned int k) const;
    static Multigraph random(std::size_t vertexCount, std::size_t edgeCount);
    std::vector<std::vector<std::size_t>> getAdjacencyMatrix() const;
};

class DegreeTrackingGraph : public Multigraph {
    const std::size_t DEGREE_UNDEFINED = -1;
    std::vector<std::size_t> _outDegrees;

    void computeOutDegrees();

  public:
    DegreeTrackingGraph(std::size_t size);
    DegreeTrackingGraph(const core::Multigraph& Multigraph);
    DegreeTrackingGraph(const std::vector<std::vector<std::size_t>>& adjacencyMatrix);

    void addEdge(vertex u, vertex v) override;
    void removeAllEdges(vertex v) override;
    std::size_t outDegree(std::size_t vertex) const override;
};

} // namespace core
