#pragma once
#include "core.hpp"
#include <vector>
namespace cycleFinder
{
class CycleFinder {
  public:
    std::vector<std::vector<vertex>> findLargestK_Cycles(const core::multiGraph& multiGraph, unsigned int k) const;
};
} // namespace cycleFinder