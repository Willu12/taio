#include "multigraph_cli.hpp"
#include "core.hpp"
#include "max_cycle.hpp"
#include "hamilton.hpp"
#include "metric.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <vector>

MultigraphCLI::MultigraphCLI() {
    app_.description("CLI tool for working with multigraphs.");
    init_distance_command();
    init_find_hamiltonian_extension_command();
    init_find_max_cycles_command();
    app_.require_subcommand(1, 1);

    app_.footer("Example:\n"
                "  ./app distance file0.txt file1.txt -i 0 -j 1\n"
                "  ./app find_hamiltonian_extension graph.txt -i 0 -k 2\n"
                "  ./app find_max_cycles graph.txt -i 0 -k 2 -p 10");
}

void MultigraphCLI::parse(int argc, char** argv) {
    app_.parse(argc, argv);
}

int MultigraphCLI::exit(const CLI::ParseError& e) {
    return app_.exit(e);
}

void MultigraphCLI::run() const {
    try {
        if (app_.got_subcommand("distance")) {
            execute_distance();
        } else if (app_.got_subcommand("find_hamiltonian_extension")) {
            execute_find_hamiltonian_extension();
        } else if (app_.got_subcommand("find_max_cycles")) {
            execute_find_max_cycles();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void MultigraphCLI::init_distance_command() {
    auto* cmd = app_.add_subcommand("distance", "Calculate the distance between given multigraphs.");
    cmd->add_option("file0", input1_.filepath, "Path to the first multigraph file")
        ->required()
        ->check(CLI::ExistingFile);
    cmd->add_option("-i,--index0", input1_.index, "Index of the multigraph in the first file")->default_val(0);
    cmd->add_option("file1", input2_.filepath, "Path to the second multigraph file")
        ->required()
        ->check(CLI::ExistingFile);
    cmd->add_option("-j,--index1", input2_.index, "Index of the multigraph in the second file")->default_val(0);
    cmd->add_flag("--approx", approx_, "Use heuristic metric");
    cmd->add_flag("--counting-sort", countSort_, "Use counting sort in heuristic metric");
    cmd->add_flag("--const-out-deg", constantOutDegrees_,
                  "Use constant-time vertex outgoing degree evaluation in heuristic metric");
}

void MultigraphCLI::init_find_hamiltonian_extension_command() {
    auto* cmd = app_.add_subcommand("find_hamiltonian_extension", "Find minimal k-Hamiltonian extension.");
    cmd->add_option("filepath", input0_.filepath, "Path to the multigraph file")->required()->check(CLI::ExistingFile);
    cmd->add_option("-i,--index", input0_.index, "Index of the multigraph in the file")->default_val(0);
    cmd->add_option("-k", k_, "Value for k in findHamiltonianKExtension")->default_val(1);
    cmd->add_flag("--approx", approx_, "Use approximation algorithm");
}

void MultigraphCLI::init_find_max_cycles_command() {
    auto* cmd = app_.add_subcommand("find_max_cycles", "Find all max k-cycles of a multigraph.");
    cmd->add_option("filepath", input0_.filepath, "Path to the multigraph file")->required()->check(CLI::ExistingFile);
    cmd->add_option("-i,--index", input0_.index, "Index of the multigraph in the file")->default_val(0);
    cmd->add_option("-k", k_, "Value for k in max cycle finding")->default_val(1);
    cmd->add_flag("--approx", approx_, "Use approximation algorithm");
    cmd->add_option("-p,--print", max_print_, "Maximum amount of printed cycles")->default_val(10);
}

void MultigraphCLI::execute_distance() const {
    const auto multigraphs0 = load_multigraphs(input1_.filepath);
    const auto multigraph0 = get_multigraph(input1_, multigraphs0);

    const auto multigraph1 = (input1_.filepath == input2_.filepath)
                                 ? get_multigraph(input2_, multigraphs0)
                                 : get_multigraph(input2_, load_multigraphs(input2_.filepath));

    print_multigraph(multigraph0);
    print_multigraph(multigraph1);

    std::unique_ptr<metric::Metric> distFun;
    if (approx_) {
        distFun = std::make_unique<metric::HeuristicMetric>(countSort_);
    } else {
        distFun = std::make_unique<metric::ExactMetric>();
    }

    std::unique_ptr<core::Multigraph> G, H;
    if (constantOutDegrees_) {
        G = std::make_unique<core::DegreeTrackingGraph>(multigraph0.multiGraph);
        H = std::make_unique<core::DegreeTrackingGraph>(multigraph1.multiGraph);
    } else {
        G = std::make_unique<core::Multigraph>(multigraph0.multiGraph);
        H = std::make_unique<core::Multigraph>(multigraph1.multiGraph);
    }

    std::size_t distance = (*distFun)(*G, *H);
    std::cout << "Distance: " << distance << "\n";
}

void MultigraphCLI::execute_find_hamiltonian_extension() const {
    const auto multigraphs = load_multigraphs(input0_.filepath);
    const auto multigraph = get_multigraph(input0_, multigraphs);

    print_multigraph(multigraph);

    core::Multigraph extension = hamilton::findKHamiltonianExtension(k_, multigraph.multiGraph, approx_);

    std::size_t kExtSize = 0;
    auto extMatrix = extension.getAdjacencyMatrix();
    std::cout << "Hamiltonian k-extension: " << std::endl;
    for (int i = 0; i < extMatrix.size(); ++i) {
        for (int j = 0; j < extMatrix.size(); ++j) {
            std::cout << extMatrix[i][j] << " ";
            kExtSize += extMatrix[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "Hamiltonian k-extension size: " << kExtSize << std::endl;

    auto multMatrix = multigraph.multiGraph.getAdjacencyMatrix();

    std::cout << "Extended input multigraph to Hamiltonian k-cycle graph: " << std::endl;
    std::vector<std::vector<std::size_t>> extendedMatrix(multMatrix.size(),
                                                         std::vector<std::size_t>(multMatrix.size()));
    for (int i = 0; i < multMatrix.size(); ++i) {
        for (int j = 0; j < multMatrix.size(); ++j) {
            extendedMatrix[i][j] = multMatrix[i][j] + extMatrix[i][j];
            std::cout << multMatrix[i][j] + extMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    if (approx_) {
        auto modifiedGraph = multigraph.multiGraph.kGraph(k_);
        auto modifiedMatrix = modifiedGraph.getAdjacencyMatrix();
        std::size_t maxFlow = hamilton::findAllHamiltonianCycles(modifiedMatrix, extMatrix, k_);

        std::cout << "Number of Hamilton cycles in the extended graph: " << maxFlow << std::endl;
    } else {
        core::Multigraph extendedMultiGraph(extendedMatrix);
        cycleFinder::MaxCycle maxCycleObj(extendedMultiGraph, k_);
        maxCycleObj.solve();
        auto cycles = maxCycleObj.getMaxVertexCycles();

        std::cout << "Number of Hamilton cycles in the extended graph: " << cycles.size() << std::endl;
    }
}

void MultigraphCLI::execute_find_max_cycles() const {
    const auto multigraphs = load_multigraphs(input0_.filepath);
    const auto multigraph = get_multigraph(input0_, multigraphs);

    print_multigraph(multigraph);
    auto maxCycleFinder = cycleFinder::MaxCycle(multigraph.multiGraph, k_);
    auto cycles = approx_ ? maxCycleFinder.approximate() : maxCycleFinder.solve();
    if (cycles.empty()) {
        std::cout << "Didn't find any cycles in this multigraph.\n";
        return;
    };

    auto maxSize = maxCycleFinder.getMaxSize();
    std::cout << "Found " << cycles.size() << " max cycles of size \n|V| = " << maxSize.vertexCount
              << " \n|E| = " << maxSize.edgeCount << " \nmaxOutDegree = " << maxSize.maxOutDegree << "\n";

    if (approx_) {
        for (const auto& cycle : cycles) {
            print_cycle(cycle);
        }
        return;
    }

    this->print_cycles(cycles, multigraph.multiGraph);
}

std::vector<AdjacencyMatrix> MultigraphCLI::parse_all_multigraphs(std::istream& input) {
    std::vector<AdjacencyMatrix> multigraphs;
    std::string line;

    if (!std::getline(input, line) || line.empty()) {
        throw std::runtime_error("Invalid input format: Missing number of multigraphs.");
    }

    std::size_t num_multigraphs = std::stoul(line);

    while (input) {
        AdjacencyMatrix adjacency_matrix;

        if (!std::getline(input, line) || line.empty()) {
            break;
        }

        std::size_t num_vertices = std::stoul(line);
        adjacency_matrix.reserve(num_vertices);

        for (std::size_t i = 0; i < num_vertices; ++i) {
            if (!std::getline(input, line) || line.empty()) {
                throw std::runtime_error("Invalid input format: Incomplete adjacency matrix.");
            }

            std::istringstream row_stream(line);
            adjacency_matrix.emplace_back(std::istream_iterator<std::size_t>(row_stream),
                                          std::istream_iterator<std::size_t>());

            if (adjacency_matrix.back().size() != num_vertices) {
                throw std::runtime_error("Invalid input format: Row size mismatch.");
            }
        }

        multigraphs.push_back(std::move(adjacency_matrix));

        if (!std::getline(input, line) || !line.empty()) {
            input.putback('\n');
        }
    }

    if (multigraphs.size() != num_multigraphs) {
        throw std::runtime_error("Invalid input format: Number of multigraphs does not match the specified count.");
    }

    return multigraphs;
}

std::vector<AdjacencyMatrix> MultigraphCLI::load_multigraphs(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    return parse_all_multigraphs(file);
}

Multigraph MultigraphCLI::get_multigraph(const Multigraph& input, const std::vector<AdjacencyMatrix>& multigraphs) {
    if (input.index >= multigraphs.size()) {
        throw std::out_of_range("Index " + std::to_string(input.index) + " out of range for file " + input.filepath);
    }

    return Multigraph{input.filepath, input.index, core::Multigraph(multigraphs[input.index])};
}

void MultigraphCLI::print_multigraph(const Multigraph& multigraph) {
    std::cout << "Multigraph from file: " << multigraph.filepath << ", Index: " << multigraph.index << "\n";
    for (const auto& row : multigraph.multiGraph.getAdjacencyMatrix()) {
        for (auto value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void MultigraphCLI::print_cycles(const std::vector<std::vector<vertex>>& cycles,
                                 const core::Multigraph& multigraph) const {
    std::cout << "Found Cycles: \n\n";
    std::size_t i = 1;
    for (const auto& cycle : cycles) {
        if (i > max_print_) return;
        std::cout << "Cycle " << i++ << ":\n";
        print_cycle(cycle);
        print_cycle_in_multigraph(cycle, multigraph);
        std::cout << "\n";
    }
}

void MultigraphCLI::print_cycle(const std::vector<vertex>& cycle) {
    std::cout << "Cycle vertices: \n";
    for (auto vertex : cycle) {
        std::cout << vertex << " ";
    }
    std::cout << "\n";
}

void MultigraphCLI::print_cycle_in_multigraph(const std::vector<vertex>& cycle, const core::Multigraph& multigraph) {
    std::cout << "Cycle represented in multigraph matrix: \n";
    std::vector<std::vector<vertex>> adjacencyMatrix(multigraph.vertexCount(),
                                                     std::vector<vertex>(multigraph.vertexCount(), 0));
    auto multigraphAdjacencyMatrix = multigraph.getAdjacencyMatrix();
    for (int i = 0; i < cycle.size() - 1; i++) {
        adjacencyMatrix[cycle[i]][cycle[i + 1]] = multigraphAdjacencyMatrix[cycle[i]][cycle[i + 1]];
    }
    for (const auto& row : adjacencyMatrix) {
        for (auto value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}
