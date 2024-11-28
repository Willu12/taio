#ifndef ATSP_HPP
#define ATSP_HPP

#include <vector>
#include <cstddef>
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace hamilton
{

// Asymmetric Traveling Salesman Problem solver using Held-Karp algorithm (dynamic programming)
class ATSPSolver {
  public:
    using Matrix = std::vector<std::vector<std::size_t>>;

    ATSPSolver(const Matrix& cost_matrix);
    Matrix solve();
    Matrix approximate();

  private:
    Matrix cost_matrix_;
    std::size_t n_;

    // Helper function to compute the Minimum Spanning Tree (MST)
    std::vector<std::vector<std::size_t>> computeMST();

    // Helper function to compute an improved matching (compared to Christofides' greedy approach)
    std::vector<std::pair<std::size_t, std::size_t>> computeImprovedMatching(
        const std::vector<std::size_t>& odd_vertices);

    // Helper function to convert Eulerian Circuit into Hamiltonian Cycle
    std::vector<std::size_t> eulerianToHamiltonian(const std::vector<std::size_t>& eulerian_circuit);

    std::size_t tsp(std::size_t pos, std::size_t mask, std::vector<std::vector<std::size_t>>& dp,
                    std::vector<std::vector<std::size_t>>& parent);
    Matrix reconstruct_cycle(const std::vector<std::size_t>& path);
};

} // namespace hamilton
#endif // ATSP_HPP