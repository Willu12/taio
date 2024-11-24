#pragma once

#include "core.hpp"

namespace metric
{

class Metric {
  public:
    virtual std::size_t operator()(const core::multiGraph& G, const core::multiGraph& H) const = 0;
};

class ExactMetric : public Metric {
  public:
    std::size_t operator()(const core::multiGraph& G, const core::multiGraph& H) const override;
};

class HeuristicMetric : public Metric {
  private:
    bool _useCountingSort;
    std::size_t edgeDifferenceCompare(std::vector<std::size_t>& degG, std::vector<std::size_t>& degH) const;
    std::size_t edgeDifferenceCount(const std::vector<std::size_t>& degG, const std::vector<std::size_t>& degH) const;

  public:
    HeuristicMetric(bool useCountingSort);
    std::size_t operator()(const core::multiGraph& G, const core::multiGraph& H) const override;
};

} // namespace metric
