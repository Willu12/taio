#include "core.hpp"
#include "max_cycle.hpp"
#include "strongly_connected_components.hpp"
#include <vector>

namespace cycleFinder
{
MaxCycle::MaxCycle(const core::multiGraph& multiGraph, unsigned int k)
    : multigraph(multiGraph), k(k), stronglyConnectedComponentsFinder(multiGraph) {
}

std::vector<std::vector<vertex>> MaxCycle::solve() {
    auto cycles = std::vector<std::vector<vertex>>();
    auto stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();

    while (stronglyConnectedComponents.empty() == false) {
        auto leastVertex = stronglyConnectedComponents.front()[0];
        auto new_cycles = processStronglyConnectedComponent(stronglyConnectedComponents.front());
        multigraph.removeAllEdges(leastVertex);
        stronglyConnectedComponentsFinder = StronglyConnectedComponents(multigraph);
        stronglyConnectedComponents = stronglyConnectedComponentsFinder.solve();

        cycles.insert(cycles.end(), new_cycles.begin(), new_cycles.end());
    }

    return cycles;
}

std::vector<std::vector<vertex>> MaxCycle::processStronglyConnectedComponent(const std::vector<vertex>& scc) {
    auto G = multigraph.inducedSubgraph(scc);
}

} // namespace cycleFinder