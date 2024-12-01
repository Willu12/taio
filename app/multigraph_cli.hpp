#ifndef MULTIGRAPHCLI_HPP
#define MULTIGRAPHCLI_HPP

#include <cstddef>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "core.hpp"
#include <CLI/CLI.hpp>

using AdjacencyMatrix = std::vector<std::vector<std::size_t>>;

struct Multigraph {
    std::string filepath;
    std::size_t index;
    core::Multigraph multiGraph;
};

class MultigraphCLI {
  public:
    MultigraphCLI();
    void parse(int argc, char** argv);
    int exit(const CLI::ParseError& e);
    void run() const;

  private:
    CLI::App app_{"TAiO: Multigraph Comparator"};
    Multigraph input1_{"", 0, core::Multigraph(0)};
    Multigraph input2_{"", 0, core::Multigraph(0)};
    Multigraph input0_{"", 0, core::Multigraph(0)};
    std::size_t k_{1};
    std::size_t max_print_{10};
    bool approx_{false};
    bool countSort_{false};

    void init_compare_command();
    void init_find_hamiltonian_extension_command();
    void init_find_max_cycles_command();

    void execute_compare() const;
    void execute_find_hamiltonian_extension() const;
    void execute_find_max_cycles() const;

    static std::vector<AdjacencyMatrix> parse_all_multigraphs(std::istream& input);
    static std::vector<AdjacencyMatrix> load_multigraphs(const std::string& filepath);
    static Multigraph get_multigraph(const Multigraph& input, const std::vector<AdjacencyMatrix>& multigraphs);
    static void print_multigraph(const Multigraph& multigraph);
    void print_cycles(const std::vector<std::vector<vertex>>& cycles, const core::Multigraph& multigraph) const;
    static void print_cycle_in_multigraph(const std::vector<vertex>& cycle, const core::Multigraph& multigraph);
    static void print_cycle(const std::vector<vertex>& cycle);
};

#endif // MULTIGRAPHCLI_HPP
