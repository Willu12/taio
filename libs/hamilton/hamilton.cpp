#include "include/hamilton.hpp"
#include "include/atsp_solver.hpp"
#include "include/flowGraph.hpp"
#include <set>

namespace hamilton
{
std::vector<std::vector<std::size_t>> findKHamiltonianExtension(std::size_t k, core::Multigraph multiGraph,
                                                                bool approx) {
    ATSPSolver::Matrix multiGraphAM = multiGraph.getAdjacencyMatrix();
    ATSPSolver::Matrix cost(multiGraphAM);
    for (int i = 0; i < cost.size(); ++i) {
        for (int j = 0; j < cost[0].size(); ++j) {
            if (i == j) continue;
            if (multiGraphAM[i][j] == 0) {
                cost[i][j] = k;
            } else if (k >= multiGraphAM[i][j]) {
                cost[i][j] = k - multiGraphAM[i][j];
            } else {
                cost[i][j] = 0;
            }

            // ATSP algorithm can't handle 0 costs (so everything is +1)
            cost[i][j] += 1;
        }
    }

    ATSPSolver::Matrix cycleMatrix;

    try {
        ATSPSolver solver(cost);
        cycleMatrix = approx ? solver.approximate() : solver.solve();

        for (int i = 0; i < cost.size(); ++i) {
            for (int j = 0; j < cost[0].size(); ++j) {
                if (cycleMatrix[i][j] != 0) {
                    // ATSP algorithm can't handle 0 costs
                    cycleMatrix[i][j] -= 1;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return cycleMatrix;
}

void divideVertices(std::vector<std::vector<std::size_t>>& inputGraph, std::set<std::size_t>& X_in,
                    std::set<std::size_t>& X_out, std::set<std::size_t>& Y,
                    std::vector<std::vector<std::size_t>>& extensionMatrix) {
    for (int i = 0; i < inputGraph.size(); ++i) {
        for (int j = 0; j < inputGraph.size(); j++) {
            if (extensionMatrix[i][j] != 0) {
                X_out.insert(i);
                X_in.insert(j);
            }
        }
    }

    for (int i = 0; i < inputGraph.size(); ++i) {
        if (X_in.find(i) == X_in.end() && X_out.find(i) == X_out.end()) {
            Y.insert(i);
        }
    }
}

void splitRepeatedVertices(std::vector<std::vector<std::size_t>>& inputGraph, std::set<std::size_t>& X_in,
                           std::set<std::size_t>& X_out, std::set<std::size_t>& Y,
                           std::vector<std::vector<std::size_t>>& extensionMatrix) {
    std::size_t verticesNumber = inputGraph.size();
    std::set<std::size_t> verticesToSplit;
    for (std::size_t v : X_in) {
        if (X_out.find(v) != X_out.end()) {
            verticesToSplit.insert(v);
        }
    }

    for (std::size_t oldVertex : verticesToSplit) {
        std::size_t newVertex = inputGraph.size();
        inputGraph.emplace_back(std::vector<std::size_t>(inputGraph.size() + 1, 0));
        for (auto& row : inputGraph) {
            row.push_back(0);
        }

        extensionMatrix.emplace_back(std::vector<std::size_t>(inputGraph.size() + 1, 0));
        for (auto& row : extensionMatrix) {
            row.push_back(0);
        }

        for (std::size_t i = 0; i < verticesNumber; ++i) {
            if (inputGraph[oldVertex][i] > 0) {
                inputGraph[newVertex][i] = inputGraph[oldVertex][i];
                inputGraph[oldVertex][i] = 0;
            }
        }

        X_out.erase(oldVertex);
        X_out.insert(newVertex);
    }
}

std::size_t findAllHamiltonianCycles(std::vector<std::vector<std::size_t>>& inputGraph,
                                     std::vector<std::vector<std::size_t>>& extensionMatrix, std::size_t k) {
    std::vector<std::vector<std::size_t>> cycles;

    std::set<std::size_t> X_in, X_out, Y;
    divideVertices(inputGraph, X_in, X_out, Y, extensionMatrix);
    splitRepeatedVertices(inputGraph, X_in, X_out, Y, extensionMatrix);

    std::size_t n = inputGraph.size();
    hamilton::FlowGraph flowGraph(n + 2);
    std::size_t source = n;
    std::size_t sink = n + 1;

    for (size_t x_i : X_in) {
        flowGraph.addEdge(source, x_i, INF);
    }

    for (size_t x_o : X_out) {
        flowGraph.addEdge(x_o, sink, INF);
    }

    for (size_t x : X_out) {
        for (size_t y : Y) {
            if (inputGraph[y][x] > 0) {
                flowGraph.addEdge(y, x, 1);
            }
        }
    }

    for (size_t x : X_in) {
        for (size_t y : Y) {
            if (inputGraph[x][y] > 0) {
                flowGraph.addEdge(x, y, 1);
            }
        }
    }

    for (size_t y_out : Y) {
        for (size_t y_in : Y) {
            if (inputGraph[y_out][y_in] > 0) {
                flowGraph.addEdge(y_out, y_in, 1);
            }
        }
    }

    std::size_t maxFlow = flowGraph.edmondsKarp(source, sink);

    return maxFlow;
}
} // namespace hamilton
