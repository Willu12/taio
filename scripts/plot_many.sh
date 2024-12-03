#!/usr/bin/env bash

function show_help {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Options:"
    echo "  -e <path>      Specify the path to the plot_benchmark_results.py script."
    echo "  -o <dir>       Specify the output directory for generated files."
    echo "  -b <dir>       Specify the directory containing benchmark result files (default: benchmark_results)."
    echo "  --pl           Enable Polish translation for the script."
    echo "  --help         Display this help message."
    echo
}

script_path=""
output_dir=""
benchmark_dir="benchmark_results"
use_polish=false

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
        -b)
            benchmark_dir="$2"
            shift 2
            ;;
        --pl)
            use_polish=true
            shift
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

if $use_polish; then
    # Polish commands
    "$script_path" "Czas obliczeń metod dokładnych" "n" \
      "$benchmark_dir/ExactMetric.csv" "Metryka" \
      "$benchmark_dir/hamiltonian_extension_exact_n_changing.csv" "k-roszerzenie Hamiltona" \
      "$benchmark_dir/max_cycle_exact_changing_n.csv" "Maksymalne cykle" \
      -o "$output_dir/exact_methods" --log-y --pl

    "$script_path" "Porównanie heurystyk metryk z liniowym stopniem" "n" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeLinear.csv" "Sortowanie przez porównanie" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeLinear.csv" "Sortowanie przez zliczanie" \
      -o "$output_dir/metric_heuristic_linear" --log-y --pl

    "$script_path" "Porównanie heurystyk metryk z stałym stopniem" "n" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeConstant.csv" "Sortowanie przez porównanie" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeConstant.csv" "Sortowanie przez zliczanie" \
      -o "$output_dir/metric_heuristic_constant" --log-y --pl

    "$script_path" "Zmiana stałej k w różnych algorytmach" "k" \
      "$benchmark_dir/hamiltonian_extension_approximation_k_changing.csv" "k-rozszerzenie Hamiltona - aproksymacja" \
      "$benchmark_dir/hamiltonian_extension_exact_k_changing.csv" "k-rozszerzenie Hamiltona - rozwiązanie dokładne" \
      "$benchmark_dir/hamilton_k_cycles_approximation_changing_k.csv" "Liczba k-cykli Hamiltona - aproksymacja" \
      "$benchmark_dir/max_cycle_exact_changing_k.csv" "Maksymalne cykle - rozwiązanie dokładne" \
      "$benchmark_dir/max_cycle_approximation_changing_k.csv" "Maksymalne cykle - aproksymacja" \
      -o "$output_dir/changing-k" --log-y --log-x --height 4 --legend-position "below" --pl

    "$script_path" "Czas obliczeń metod aproksymacyjnych" "n" \
      "$benchmark_dir/hamiltonian_extension_approximation_n_changing.csv" "k-rozszerzenie Hamiltona" \
      "$benchmark_dir/max_cycle_approximation_changing_n.csv" "Maksymalne cykle" \
      -o "$output_dir/approximation_methods" --log-y --pl

    "$script_path" "Porównanie metod znajdujących k-rozszerzenie Hamiltona" "n" \
      "$benchmark_dir/hamiltonian_extension_exact_n_changing.csv" "Rozwiązanie dokładne" \
      "$benchmark_dir/hamiltonian_extension_approximation_n_changing.csv" "Aproksymacja" \
      -o "$output_dir/hamiltonian_extension_methods" --log-y --log-x --pl

    "$script_path" "Porównanie metod znajdujących maksymalne cykle" "n" \
      "$benchmark_dir/hamiltonian_extension_exact_n_changing.csv" "Rozwiązanie dokładne" \
      "$benchmark_dir/hamiltonian_extension_approximation_n_changing.csv" "Aproksymacja" \
      -o "$output_dir/max_cycle_methods" --log-y --log-x --pl

    "$script_path" "Porównanie metod obliczania metryki" "n" \
      "$benchmark_dir/ExactMetric.csv" "Rozwiązanie dokładne" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeLinear.csv" "Heurystyka z sortowaniem przez porównanie i stopniem liniowym" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeLinear.csv" "Heurystyka z sortowaniem przez zliczanie i stopniem liniowym" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeConstant.csv" "Heurystyka z sortowaniem przez porównanie i stałym stopniem" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeConstant.csv" "Heurystyka z sortowaniem przez zliczanie i stałym stopniem" \
      -o "$output_dir/metric_methods" --log-y --log-x --pl
else
    # English commands
    "$script_path" "Exact methods computation time" "n" \
      "$benchmark_dir/ExactMetric.csv" "Metric" \
      "$benchmark_dir/hamiltonian_extension_exact_n_changing.csv" "Hamiltonian k-extension" \
      "$benchmark_dir/max_cycle_exact_changing_n.csv" "Max cycles" \
      -o "$output_dir/exact_methods" --log-y

    "$script_path" "Comparison of metric heuristics with linear degree" "n" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeLinear.csv" "Comparison sort" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeLinear.csv" "Counting sort" \
      -o "$output_dir/metric_heuristic_linear" --log-y

    "$script_path" "Comparison of metric heuristics with constant degree" "n" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeConstant.csv" "Comparison sort" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeConstant.csv" "Counting sort" \
      -o "$output_dir/metric_heuristic_constant" --log-y

    "$script_path" "Changing k constant in various algorithms" "k" \
      "$benchmark_dir/hamiltonian_extension_approximation_k_changing.csv" "Hamiltonian k-extension - approximation" \
      "$benchmark_dir/hamiltonian_extension_exact_k_changing.csv" "Hamiltonian k-extension - exact solution" \
      "$benchmark_dir/hamilton_k_cycles_approximation_changing_k.csv" "Hamiltonian k-cycles count - approximation" \
      "$benchmark_dir/max_cycle_exact_changing_k.csv" "Max cycles - exact solution" \
      "$benchmark_dir/max_cycle_approximation_changing_k.csv" "Max cycles - approximation" \
      -o "$output_dir/changing-k" --log-y --log-x --height 4 --legend-position "below"

    "$script_path" "Approximation methods computation time" "n" \
      "$benchmark_dir/hamiltonian_extension_approximation_n_changing.csv" "Hamiltonian k-extension" \
      "$benchmark_dir/max_cycle_approximation_changing_n.csv" "Max cycles" \
      -o "$output_dir/approximation_methods" --log-y

    "$script_path" "Comparison of Hamiltonian k-extension finding methods" "n" \
      "$benchmark_dir/hamiltonian_extension_exact_n_changing.csv" "Exact solution" \
      "$benchmark_dir/hamiltonian_extension_approximation_n_changing.csv" "Approximation" \
      -o "$output_dir/hamiltonian_extension_methods" --log-y --log-x

    "$script_path" "Comparison of max cycle" "n" \
      "$benchmark_dir/hamiltonian_extension_exact_n_changing.csv" "Exact solution" \
      "$benchmark_dir/hamiltonian_extension_approximation_n_changing.csv" "Approximation" \
      -o "$output_dir/max_cycle_methods" --log-y --log-x

    "$script_path" "Comparison of metric calculation methods" "n" \
      "$benchmark_dir/ExactMetric.csv" "Exact solution" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeLinear.csv" "Heuristic comparison sort with linear degree" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeLinear.csv" "Heuristic counting sort with linear degree" \
      "$benchmark_dir/HeuristicMetric_ComparisonSort_DegreeConstant.csv" "Heuristic comparison sort with constant degree" \
      "$benchmark_dir/HeuristicMetric_CountingSort_DegreeConstant.csv" "Heuristic counting sort with constant degree" \
      -o "$output_dir/metric_methods" --log-y --log-x
fi

