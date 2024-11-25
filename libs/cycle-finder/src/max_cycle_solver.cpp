#include "max_cycle_solver.hpp"
#include "core.hpp"
#include "max_cycle.hpp"

#include <vector>

std::vector<std::vector<vertex>> cycleFinder::MaxCycleSolver::solve(const core::Multigraph& multiGraph,
                                                                    const unsigned int k) {
    auto solver = cycleFinder::MaxCycle(multiGraph, k);
    return solver.solve();
}

std::vector<std::vector<vertex>> cycleFinder::MaxCycleSolver::approximate(const core::Multigraph& multiGraph,
                                                                          const unsigned int k) {
    auto solver = cycleFinder::MaxCycle(multiGraph, k);
    return solver.approximate();
}