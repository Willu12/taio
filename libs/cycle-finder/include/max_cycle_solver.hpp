#include "core.hpp"
#include <vector>
namespace cycleFinder
{
class MaxCycleSolver {
  public:
    std::vector<std::vector<vertex>> solve(const core::Multigraph& G, const unsigned int k);
    std::vector<std::vector<vertex>> approximate(const core::Multigraph& G, const unsigned int k);
};
} // namespace cycleFinder