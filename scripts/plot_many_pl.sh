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

"$script_path" "Czas obliczeń metod dokładnych" "n" \
  benchmark_results/ExactMetric.csv "Metryka" \
  benchmark_results/hamiltonian_extension_exact_n_changing.csv "k-roszerzenie Hamiltona" \
  benchmark_results/max_cycle_exact_changing_n.csv "Maksymalne cykle" \
  -o "$output_dir/exact_methods" --log-y

"$script_path" "Porównanie heurystyk metryk z liniowym stopniem" "n" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeLinear.csv "Sortowanie przez porównanie" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeLinear.csv "Sortowanie przez zliczanie" \
  -o "$output_dir/metric_heuristic_linear" --log-y

"$script_path" "Porównanie heurystyk metryk z stałym stopniem" "n" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeConstant.csv "Sortowanie przez porównanie" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeConstant.csv "Sortowanie przez zliczanie" \
  -o "$output_dir/metric_heuristic_constant" --log-y

"$script_path" "Zmiana stałej k w różnych algorytmach" "k" \
  benchmark_results/hamiltonian_extension_approximation_k_changing.csv "k-rozszerzenie hamiltona - aproksymacja" \
  benchmark_results/hamiltonian_extension_exact_k_changing.csv "k-rozszerzenie Hamiltona - rozwiązanie dokładne"  \
  benchmark_results/hamilton_k_cycles_approximation_changing_k.csv "Liczba k-cykli Hamiltona - aproksymacja" \
  benchmark_results/max_cycle_exact_changing_k.csv "Maksymalne cykle - rozwiązanie dokładne" \
  -o "$output_dir/changing-k" --log-y

"$script_path" "Czas obliczeń metod aproksymacyjnych" "n" \
  benchmark_results/hamiltonian_extension_approximation_n_changing.csv "k-rozszerzenie Hamiltona" \
  benchmark_results/max_cycle_approximation_changing_n.csv "Maksymalne cykle" \
  -o "$output_dir/approximation_methods" --log-y

"$script_path" "Porównanie metod znajdujących k-rozszerzenie Hamiltona" "n" \
  benchmark_results/hamiltonian_extension_exact_n_changing.csv "Rozwiązanie dokładne" \
  benchmark_results/hamiltonian_extension_approximation_n_changing.csv "Aproksymacja" \
  -o "$output_dir/hamiltonian_extension_methods" --log-y --log-x

"$script_path" "Porównanie metod znajdujących maksymalne cykle" "n" \
  benchmark_results/hamiltonian_extension_exact_n_changing.csv "Rozwiązanie dokładne" \
  benchmark_results/hamiltonian_extension_approximation_n_changing.csv "Aproksymacja" \
  -o "$output_dir/max_cycle_methods" --log-y --log-x

"$script_path" "Porównanie metod obliczania metryki" "n" \
  benchmark_results/ExactMetric.csv "Rozwiązanie dokładne" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeLinear.csv "Heurystyka z sortowaniem przez porównanie i stopniem liniowym" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeLinear.csv "Heurystyka z sortowaniem przez zliczanie i stopniem liniowym" \
  benchmark_results/HeuristicMetric_ComparisonSort_DegreeConstant.csv "Heurystyka z sortowaniem przez porównanie i stałym stopniem" \
  benchmark_results/HeuristicMetric_CountingSort_DegreeConstant.csv "Heurystyka z sortowaniem przez zliczanie i stałym stopniem" \
  -o "$output_dir/metric_methods" --log-y --log-x

