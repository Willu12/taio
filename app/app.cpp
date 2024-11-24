#include <iostream>

#include "core.hpp"
#include "metric.hpp"

void interfaceTest(const metric::Metric& metric, const core::multiGraph& G, const core::multiGraph& H) {
    std::cout << "dist(G, H) = " << metric(G, H) << "\n";
}

int main() {
    std::vector<std::vector<std::size_t>> adj1 {
        { 0, 1, 2, 3, 4},
        { 1, 0, 3, 4, 0},
        { 2, 3, 0, 0, 1},
        { 3, 4, 0, 0, 2},
        { 4, 0, 1, 2, 0},
    };

    std::vector<std::vector<std::size_t>> adj2 {
        { 0, 1, 2, 3, 4, 0},
        { 1, 0, 3, 4, 0, 1},
        { 2, 3, 0, 0, 1, 2},
        { 3, 4, 0, 0, 2, 3},
        { 4, 0, 1, 2, 0, 4},
        { 0, 0, 0, 0, 0, 0}
    };
    core::multiGraph G(adj1);
    core::multiGraph H(adj2);
    metric::ExactMetric exactMetric;
    metric::HeuristicMetric heuristicMetric;
    interfaceTest(exactMetric, G, H);
    interfaceTest(heuristicMetric, G, H);
	return 0;
}