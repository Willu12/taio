#include "include/hamilton.hpp"
#include "include/atsp_solver.hpp"

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
} // namespace hamilton
