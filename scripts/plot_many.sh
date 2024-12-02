#!/usr/bin/env bash

function show_help {
    echo "Usage: $0 [OPTIONS] <output_directory>"
    echo
    echo "Options:"
    echo "  -e <path>      Specify the path to the plot_benchmark_results.py script."
    echo "  -o <dir>       Specify the output directory for generated files."
    echo "  --help         Display this help message."
    echo
    echo "Positional arguments:"
    echo "  output_directory   Directory to store the output files."
}

script_path=""
output_dir=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -e)
            script_path="$2"
            shift 2
            ;;
        -o)
            output_dir="$2"
            shift 2
            ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

if [[ -z "$script_path" || -z "$output_dir" ]]; then
    echo "Error: Missing required arguments."
    show_help
    exit 1
fi

mkdir -p "$output_dir"

"$script_path" "Exact methods computation time" "n" \
  benchmark_results/ExactMetric.csv "Metric" \
  benchmark_results/hamiltonian_extension_exact_n_changing.csv "Hamiltonian k-extension" \
  benchmark_results/max_cycle_exact_changing_n.csv "Max cycles" \
  -o "$output_dir/exact_methods" --log-y

"$script_path" "Comparison of metric heuristics with linear degree" "n" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeLinear.csv "Comparison sort" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeLinear.csv "Counting sort" \
  -o "$output_dir/metric_heuristic_linear" --log-y

"$script_path" "Comparison of metric heuristics with constant degree" "n" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeConstant.csv "Comparison sort" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeConstant.csv "Counting sort" \
  -o "$output_dir/metric_heuristic_constant" --log-y

"$script_path" "Changing k constant in various algorithms" "k" \
  benchmark_results/hamiltonian_extension_approximation_k_changing.csv "Hamiltonian k-extension - approximation" \
  benchmark_results/hamiltonian_extension_exact_k_changing.csv "Hamiltonian k-extension - exact solution" \
  benchmark_results/hamilton_k_cycles_approximation_changing_k.csv "Hamiltonian k-cycles count - approximation" \
  benchmark_results/max_cycle_exact_changing_k.csv "Max cycles - exact solution" \
  benchmark_results/max_cycle_approximation_changing_k.csv "Max cycles - approximation" \
  -o "$output_dir/changing-k" --log-y

"$script_path" "Approximation methods computation time" "n" \
  benchmark_results/hamiltonian_extension_approximation_n_changing.csv "Hamiltonian k-extension" \
  benchmark_results/max_cycle_approximation_changing_n.csv "Max cycles" \
  -o "$output_dir/approximation_methods" --log-y

"$script_path" "Comparison of Hamiltonian k-extension finding methods" "n" \
  benchmark_results/hamiltonian_extension_exact_n_changing.csv "Exact solution" \
  benchmark_results/hamiltonian_extension_approximation_n_changing.csv "Approximation" \
  -o "$output_dir/hamiltonian_extension_methods" --log-y --log-x

"$script_path" "Comparison of max cycle" "n" \
  benchmark_results/hamiltonian_extension_exact_n_changing.csv "Exact solution" \
  benchmark_results/hamiltonian_extension_approximation_n_changing.csv "Approximation" \
  -o "$output_dir/max_cycle_methods" --log-y --log-x

"$script_path" "Comparison of metric calculation methods" "n" \
  benchmark_results/ExactMetric.csv "Exact solution" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeLinear.csv "Heuristic comparison sort with linear degree" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeLinear.csv "Heuristic counting sort with linear degree" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeConstant.csv "Heuristic comparison sort with constant degree" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeConstant.csv "Heuristic counting sort with constant degree" \
  -o "$output_dir/metric_methods" --log-y --log-x


