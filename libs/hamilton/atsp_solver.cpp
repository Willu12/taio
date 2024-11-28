#include "include/atsp_solver.hpp"
#include <cstddef>
#include <iostream>
#include <utility>

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

    // dp[mask][i] stores the minimal cost to visit nodes in `mask` ending at `i`
    std::vector<std::vector<std::size_t>> dp(1U << n_, std::vector<std::size_t>(n_, INF));

    // for path reconstruction
    std::vector<std::vector<int>> parent(1U << n_, std::vector<int>(n_, -1));

    // start from the first node
    dp[1][0] = 0;

    // fill the table
    for (std::size_t mask = 1; mask < (1U << n_); ++mask) {
        for (std::size_t u = 0; u < n_; ++u) {
            if (!(mask & (1U << u)) || dp[mask][u] == INF)
                continue;

            for (std::size_t v = 0; v < n_; ++v) {
                if (mask & (1U << v) || cost_matrix_[u][v] == INF)
                    continue;

                std::size_t new_mask = mask | (1U << v);
                std::size_t new_cost = dp[mask][u] + cost_matrix_[u][v];

                if (new_cost < dp[new_mask][v]) {
                    dp[new_mask][v] = new_cost;
                    parent[new_mask][v] = static_cast<int>(u);
                }
            }
        }
    }

    // find the optimal end node
    std::size_t optimal_cost = INF;
    int last_node = -1;

    for (std::size_t i = 1; i < n_; ++i) {
        std::size_t cost_to_complete = dp[(1U << n_) - 1][i] + cost_matrix_[i][0];
        if (cost_to_complete < optimal_cost) {
            optimal_cost = cost_to_complete;
            last_node = static_cast<int>(i);
        }
    }

    if (optimal_cost == INF) {
        throw std::runtime_error("No valid ATSP cycle exists");
    }

    // reconstruct the path
    std::vector<std::size_t> path;
    std::size_t mask = (1U << n_) - 1;
    int current_node = last_node;

    while (current_node != -1) {
        path.push_back(static_cast<std::size_t>(current_node));
        int prev_node = parent[mask][current_node];
        mask &= ~(1U << current_node);
        current_node = prev_node;
    }

    std::reverse(path.begin(), path.end());
    return reconstruct_cycle(path);
}


ATSPSolver::Matrix ATSPSolver::approximate() {
    // nearest-neighbor heuristic
    std::vector<std::size_t> path;
    std::vector<bool> visited(n_, false);
    std::size_t current_node = 0;
    path.push_back(current_node);
    visited[current_node] = true;

    for (std::size_t step = 1; step < n_; ++step) {
        std::size_t nearest_neighbor = -1;
        std::size_t min_cost = std::numeric_limits<std::size_t>::max();
        for (std::size_t neighbor = 0; neighbor < n_; ++neighbor) {
            if (!visited[neighbor] && cost_matrix_[current_node][neighbor] < min_cost) {
                nearest_neighbor = neighbor;
                min_cost = cost_matrix_[current_node][neighbor];
            }
        }
        if (nearest_neighbor == -1) {
            throw std::runtime_error("No valid ATSP cycle exists");
        }
        current_node = nearest_neighbor;
        path.push_back(current_node);
        visited[current_node] = true;
    }
    path.push_back(0);

    // 2-Opt heuristic
    std::size_t numImpr = 0;
    std::size_t costForward, costReverse;
    bool improvement = true;
    while (numImpr < n_ && improvement) {
        improvement = false;
        for (std::size_t i = 1; i < path.size() - 2; ++i) {
            costForward = 0;
            costReverse = 0;

            for (std::size_t j = i + 1; j < path.size() - 1; ++j) {
                std::size_t cost_before = cost_matrix_[path[i - 1]][path[i]] +
                                          costForward +
                                          cost_matrix_[path[j]][path[j + 1]];
                std::size_t cost_after = cost_matrix_[path[i - 1]][path[j]] +
                                         costReverse +
                                         cost_matrix_[path[i]][path[j + 1]];
                if (cost_after < cost_before) {
                    improvement = true;
                    std::reverse(path.begin() + i, path.begin() + j + 1);
                    std::swap(costForward, costReverse);
                }
                costForward += cost_matrix_[path[j]][path[j+1]];
                costReverse += cost_matrix_[path[j+1]][path[j]];
            }
        }
        numImpr++;
    }

    return reconstruct_cycle(path);
}

} // namespace hamilton
