#!/usr/bin/env bash

# This script runs benchmarks using the provided executable and saves results in the specified output directory.

# Usage:
# ./benchmark_runner.sh <path_to_tests_exe> <output_directory>
# Arguments:
#   <path_to_tests_exe>    Path to the test executable.
#   <output_directory>     Directory to store the benchmark outputs.

# Check for help flag or incorrect arguments
if [[ "$1" == "--help" || "$1" == "-h" || "$#" -ne 2 ]]; then
    echo "Usage: $0 <path_to_tests_exe> <output_directory>"
    echo
    echo "Arguments:"
    echo "  <path_to_tests_exe>    Path to the test executable."
    echo "  <output_directory>     Directory to store the benchmark outputs."
    exit 1
fi

# Input parameters
TESTS_EXE=$1
OUT_DIRECTORY=$2

# Define filters
FILTERS=(
  "ExactMetric"
  "HeuristicMetric_ComparisonSort_DegreeLinear"
  "HeuristicMetric_ComparisonSort_DegreeConstant"
  "HeuristicMetric_CountingSort_DegreeLinear"
  "HeuristicMetric_CountingSort_DegreeConstant"
  "max_cycle_exact_changing_n"
  "max_cycle_exact_changing_k"
  "max_cycle_approximation_changing_k"
  "max_cycle_approximation_changing_n"
  "hamiltonian_extension_exact_k_changing"
  "hamiltonian_extension_exact_n_changing"
  "hamiltonian_extension_approximation_n_changing"
  "hamiltonian_extension_approximation_k_changing"
  "hamilton_k_cycles_approximation_changing_k"
  "hamilton_k_cycles_approximation_changing_n"
)

# Ensure output directory exists
mkdir -p "$OUT_DIRECTORY"

# Run benchmarks for each filter
for FILTER in "${FILTERS[@]}"; do
    echo "Running benchmark for filter: $FILTER"
    "$TESTS_EXE" --benchmark_filter="BM_$FILTER/.*" --benchmark_out="$OUT_DIRECTORY/$FILTER.csv" --benchmark_out_format=csv
done

echo "All benchmarks completed. Results are saved in: $OUT_DIRECTORY"

