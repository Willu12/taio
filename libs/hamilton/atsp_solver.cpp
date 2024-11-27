#include "include/atsp_solver.hpp"

namespace hamilton
{
ATSPSolver::ATSPSolver(const Matrix& cost_matrix) : cost_matrix_(cost_matrix), n_(cost_matrix.size()) {
    if (n_ == 0 || cost_matrix_.size() != cost_matrix_[0].size()) {
        throw std::invalid_argument("Cost matrix must be square and non-empty");
    }
}

ATSPSolver::Matrix ATSPSolver::reconstruct_cycle(const std::vector<std::size_t>& path) {
    Matrix result(n_, std::vector<std::size_t>(n_, 0));
    for (std::size_t i = 0; i < path.size() - 1; ++i) {
        result[path[i]][path[i + 1]] = cost_matrix_[path[i]][path[i + 1]];
    }
    result[path.back()][path.front()] = cost_matrix_[path.back()][path.front()];
    return result;
}

ATSPSolver::Matrix ATSPSolver::solve() {
    const std::size_t INF = std::numeric_limits<std::size_t>::max();

    // DP table: dp[mask][i] stores the minimal cost to visit nodes in `mask` ending at `i`
    std::vector<std::vector<std::size_t>> dp(1 << n_, std::vector<std::size_t>(n_, INF));

    // parent table for path reconstruction
    std::vector<std::vector<std::size_t>> parent(1 << n_, std::vector<std::size_t>(n_, -1));

    // base case: Starting at node 0 with only node 0 visited
    dp[1][0] = 0;

    // iterative DP to fill the table
    for (std::size_t mask = 1; mask < (1U << n_); ++mask) {
        for (std::size_t u = 0; u < n_; ++u) {
            if (!(mask & (1U << u))) continue; // Skip if `u` is not in the current mask

            for (std::size_t v = 0; v < n_; ++v) {
                if ((mask & (1U << v)) || cost_matrix_[u][v] == INF)
                    continue; // Skip if `v` is already visited or invalid

                std::size_t new_mask = mask | (1U << v);
                std::size_t new_cost = dp[mask][u] + cost_matrix_[u][v];

                if (new_cost < dp[new_mask][v]) {
                    dp[new_mask][v] = new_cost;
                    parent[new_mask][v] = u;
                }
            }
        }
    }

    // find the optimal end node for a full cycle
    std::size_t optimal_cost = INF;
    std::size_t last_node = -1;
    for (std::size_t i = 1; i < n_; ++i) {
        if (dp[(1U << n_) - 1][i] + cost_matrix_[i][0] < optimal_cost) {
            optimal_cost = dp[(1U << n_) - 1][i] + cost_matrix_[i][0];
            last_node = i;
        }
    }

    if (optimal_cost >= INF) {
        throw std::runtime_error("No valid ATSP cycle exists");
    }

    // reconstruct the path
    std::vector<std::size_t> path;
    std::size_t mask = (1U << n_) - 1;
    std::size_t current_node = last_node;

    while (current_node != -1) {
        path.push_back(current_node);
        std::size_t prev_node = parent[mask][current_node];
        mask &= ~(1U << current_node);
        current_node = prev_node;
    }

    std::reverse(path.begin(), path.end());
    return reconstruct_cycle(path);
}
} // namespace hamilton