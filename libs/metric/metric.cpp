#include "metric.hpp"

std::size_t metric::ExactMetric::operator()(const core::multiGraph& G, const core::multiGraph& H) const {
    return 1;
}

std::size_t metric::HeuristicMetric::operator()(const core::multiGraph& G, const core::multiGraph& H) const {
    return 2;
}
