#ifndef MULTIGRAPHCLI_HPP
#define MULTIGRAPHCLI_HPP

#include "core.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <filesystem>
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

    static std::vector<AdjacencyMatrix> parse_all_multigraphs(std::istream& input);
    static std::vector<AdjacencyMatrix> load_multigraphs(const std::string& filepath);
    static Multigraph get_multigraph(const Multigraph& input, const std::vector<AdjacencyMatrix>& multigraphs);
    static void print_multigraph(const Multigraph& multigraph);
    static void print_cycles(const std::vector<std::vector<vertex>>& cycles);
};

#endif // MULTIGRAPHCLI_HPP
