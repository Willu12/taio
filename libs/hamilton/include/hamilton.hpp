#ifndef HAMILTON_HPP
#define HAMILTON_HPP

#include <vector>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include "core.hpp"

namespace hamilton
{
std::vector<std::vector<std::size_t>> findKHamiltonianExtension(std::size_t k, core::Multigraph multiGraph,
                                                                bool approx);

void divideVertices(std::vector<std::vector<std::size_t>>& inputGraph, std::vector<std::size_t>& X,
                    std::vector<std::size_t>& Y, std::vector<std::vector<std::size_t>>& extensionMatrix);

std::size_t findAllHamiltonianCycles(std::vector<std::vector<std::size_t>>& inputGraph,
                                     std::vector<std::vector<std::size_t>>& extensionMatrix, std::size_t k);
}

#endif