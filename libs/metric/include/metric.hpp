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
  public:
    std::size_t operator()(const core::multiGraph& G, const core::multiGraph& H) const override;
};

} // namespace metric
