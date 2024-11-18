#include "cycle_finder.hpp"
#include "core.hpp"
#include <vector>

enum Visited {
    unvisited,
    partially,
    completely
};

namespace cycleFinder
{

std::vector<std::vector<vertex>> cycleFinder::CycleFinder::findLargestK_Cycles(const core::multiGraph& multiGraph,
                                                                               unsigned int k) const {
    auto cycles = std::vector<std::vector<vertex>>();
    return cycles;
}

} // namespace cycleFinder