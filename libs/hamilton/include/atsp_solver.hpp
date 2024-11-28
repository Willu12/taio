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

    Matrix reconstruct_cycle(const std::vector<std::size_t>& path);
};

} // namespace hamilton
#endif // ATSP_HPP