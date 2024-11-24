#pragma once
#include <vector>
#include <cstddef>
#include <algorithm>
#include <iostream>

namespace hamilton
{
std::vector<std::vector<std::size_t>> findKHamiltonianExtension(std::size_t k, std::vector<std::vector<std::size_t>> multiGraph);
}
