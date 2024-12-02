#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt
import sys
from pathlib import Path

def parse_csv(filepath):
    """
    Parses a Google Benchmark CSV file, extracting metadata and benchmark data.

    Args:
        filepath (str): Path to the CSV file.

    Returns:
        tuple: (metadata, DataFrame) where metadata is a dict and DataFrame contains benchmark data.
    """
    metadata = {}
    with open(filepath, 'r') as f:
        lines = f.readlines()

    # Extract metadata from header lines
    metadata['timestamp'] = lines[0].strip()
    metadata['command'] = lines[1].strip()
    metadata['cpu_info'] = lines[2].strip()
    metadata['cache_info'] = [line.strip() for line in lines[3:7]]
    metadata['load_average'] = lines[7].strip()

    # Detect and load CSV data (find the header row dynamically)
    for i, line in enumerate(lines):
        if line.startswith("name,"):  # Detect header row
            header_row = i
            break
    else:
        raise ValueError(f"No valid header row found in {filepath}")

    # Read the CSV data starting from the detected header row
    df = pd.read_csv(filepath, skiprows=header_row)

    return metadata, df

def plot_benchmarks(filepaths):
    """
    Plots benchmarks from Google Benchmark CSV files on a 2D plot.
    The x-axis represents benchmark parameters, and the y-axis shows measured CPU time.

    Args:
        filepaths (list of str): List of file paths to CSV files.
    """
    plt.figure(figsize=(12, 8))

    for filepath in filepaths:
        # Parse CSV file and extract data
        metadata, df = parse_csv(filepath)

        # Ensure required columns are present
        required_columns = {"name", "real_time", "time_unit"}
        if not required_columns.issubset(df.columns):
            print(f"Skipping {filepath}: Missing required columns {required_columns}")
            continue

        # Extract parameters and real_time
        def extract_parameter(benchmark_name):
            """Extract parameter from the benchmark name."""
            parts = benchmark_name.split('/')
            return int(parts[1]) if len(parts) > 1 and parts[1].isdigit() else None

        df['parameter'] = df['name'].apply(extract_parameter)
        df = df.dropna(subset=['parameter'])  # Remove rows with missing parameters

        if df.empty:
            print(f"Skipping {filepath}: No valid parameter data found.")
            continue

        # Convert real_time to appropriate unit (ns assumed by default)
        if df['time_unit'].iloc[0] == 'ms':
            df['real_time'] *= 1e6
        elif df['time_unit'].iloc[0] == 's':
            df['real_time'] *= 1e9

        # Plot benchmark results
        plt.plot(
            df['parameter'], df['real_time'], marker='o', label=f"{Path(filepath).stem}"
        )

        # Print metadata for user information
        print(f"File: {filepath}")
        for key, value in metadata.items():
            print(f"{key}: {value}")
        print()

    plt.xlabel("Benchmark Parameter")
    plt.ylabel("Measured CPU Time (ns)")
    plt.title("Benchmark Results")
    plt.legend(title="Files", loc="upper left")
    plt.grid(True)
    plt.tight_layout()

    plt.show()


def main():
    if len(sys.argv) < 2 or "--help" in sys.argv or "-h" in sys.argv:
        print("Usage: python3 plot_benchmarks.py <file1.csv> [<file2.csv> ...]")
        print("Description: Plots Google Benchmark results from multiple CSV files.")
        print("             x-axis: Benchmark parameter, y-axis: CPU time (real time).")
        sys.exit(1)

    filepaths = sys.argv[1:]
    plot_benchmarks(filepaths)


if __name__ == "__main__":
    main()

