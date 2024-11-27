#ifndef HAMILTON_HPP
#define HAMILTON_HPP

#include <vector>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include "core.hpp"

namespace hamilton
{
std::vector<std::vector<std::size_t>> findKHamiltonianExtension(std::size_t k, core::Multigraph multiGraph, bool approx);
}

#endif