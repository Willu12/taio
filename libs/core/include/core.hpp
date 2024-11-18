#pragma once

#include <cstddef>
#include <vector>

namespace core
{
class multiGraph {
    std::vector<std::vector<std::size_t>> adjacencyMatrix;

    multiGraph(const std::vector<std::vector<std::size_t>> adjacencyMatrix) {
        this->adjacencyMatrix = adjacencyMatrix;
    }
};

} // namespace core
