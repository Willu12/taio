#include "multigraph_cli.hpp"

MultigraphCLI::MultigraphCLI() {
    app_.description("CLI tool for working with multigraphs.");

    // "compare" subcommand
    auto* compare_cmd = app_.add_subcommand("compare", "Compares multigraphs from files.");
    compare_cmd->add_option("file1", input1_.filepath, "Path to the first multigraph file")
        ->required()
        ->check(CLI::ExistingFile);
    compare_cmd->add_option("-i,--index1", input1_.index, "Index of the multigraph in the first file (default: 0)")
        ->default_val(0);
    compare_cmd->add_option("file2", input2_.filepath, "Path to the second multigraph file")
        ->required()
        ->check(CLI::ExistingFile);
    compare_cmd->add_option("-j,--index2", input2_.index, "Index of the multigraph in the second file (default: 0)")
        ->default_val(0);

    // "find_hamiltonian_extension" subcommand
    auto* find_hamiltonian_cmd = app_.add_subcommand("find_hamiltonian_extension", "Finds k-Hamiltonian extension for a graph.");
    find_hamiltonian_cmd->add_option("filepath", input1_.filepath, "Path to the multigraph file")
        ->required()
        ->check(CLI::ExistingFile);
    find_hamiltonian_cmd->add_option("-i,--index", input1_.index, "Index of the multigraph in the file (default: 0)")
        ->default_val(0);
    find_hamiltonian_cmd->add_option("-k", k_, "Value for k in findHamiltonianKExtension (default: 1)")
        ->default_val(1);

    app_.footer("Example:\n"
                "  ./multigraph_cli compare file1.txt file2.txt -i 0 -j 1\n"
                "  ./multigraph_cli find_hamiltonian_extension graph.txt -i 0 -k 2");
}

void MultigraphCLI::parse(int argc, char** argv) {
    app_.parse(argc, argv);
}

int MultigraphCLI::exit(const CLI::ParseError& e) {
    return app_.exit(e);
}

void MultigraphCLI::run() const {
    try {
        if (app_.got_subcommand("compare")) {
            // Handle the "compare" subcommand
            const auto multigraphs1 = load_multigraphs(input1_.filepath);
            const auto multigraph1 = get_multigraph(input1_, multigraphs1);

            const auto multigraph2 = (input1_.filepath == input2_.filepath)
                                         ? get_multigraph(input2_, multigraphs1)
                                         : get_multigraph(input2_, load_multigraphs(input2_.filepath));

            print_multigraph(multigraph1);
            print_multigraph(multigraph2);

            throw std::runtime_error("Not implemented yet!");
        } else if (app_.got_subcommand("find_hamiltonian_extension")) {
            // Handle the "find_hamiltonian_extension" subcommand
            const auto multigraphs = load_multigraphs(input1_.filepath);
            const auto multigraph = get_multigraph(input1_, multigraphs);

            print_multigraph(multigraph);

            hamilton::findHamiltonianKExtension(k_, multigraph.adjacency_matrix);
        } else {
            throw std::runtime_error("No valid subcommand specified.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
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

    return Multigraph{input.filepath, input.index, multigraphs[input.index]};
}

void MultigraphCLI::print_multigraph(const Multigraph& multigraph) {
    std::cout << "Multigraph from file: " << multigraph.filepath << ", Index: " << multigraph.index << "\n";
    for (const auto& row : multigraph.adjacency_matrix) {
        for (auto value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

