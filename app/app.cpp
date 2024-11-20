#include "multigraph_cli.hpp"


int main(int argc, char** argv) {
    MultigraphCLI cli;
    try {
        cli.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return cli.exit(e);
    }
    cli.run();
    return 0;
}

