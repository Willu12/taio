#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "include/flowGraph.hpp"

namespace hamilton
{
    FlowGraph::FlowGraph(std::size_t n)
        : vertexCount(n), capacity(n, std::vector<std::size_t>(n, 0)), flow(n, std::vector<std::size_t>(n, 0)) {
    }

    void FlowGraph::addEdge(std::size_t u, std::size_t v, std::size_t cap) {
        capacity[u][v] += cap;
    }

    bool FlowGraph::bfs(std::size_t source, std::size_t sink, std::vector<std::size_t>& parent) {
        std::vector<bool> visited(vertexCount, false);
        std::queue<std::size_t> queue;
        queue.push(source);
        visited[source] = true;

        while (!queue.empty()) {
            std::size_t u = queue.front();
            queue.pop();

            for (std::size_t v = 0; v < vertexCount; ++v) {
                if (!visited[v] && capacity[u][v] - flow[u][v] > 0) {
                    queue.push(v);
                    visited[v] = true;
                    parent[v] = u;
                    if (v == sink) return true;
                }
            }
        }
        return false;
    }

    std::size_t FlowGraph::edmondsKarp(std::size_t source, std::size_t sink) {
        std::vector<std::size_t> parent(vertexCount);
        std::size_t maxFlow = 0;

        while (bfs(source, sink, parent)) {
            std::size_t pathFlow = INF;
            for (std::size_t v = sink; v != source; v = parent[v]) {
                std::size_t u = parent[v];
                pathFlow = std::min(pathFlow, capacity[u][v] - flow[u][v]);
            }

            for (std::size_t v = sink; v != source; v = parent[v]) {
                std::size_t u = parent[v];
                flow[u][v] += pathFlow;
                flow[v][u] -= pathFlow;
            }

            maxFlow += pathFlow;
        }
        return maxFlow;
    }
} // namespace hamilton