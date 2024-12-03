#!/usr/bin/env python3

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

def derive_x_values(name_column):
    """
    Derives x-axis values (e.g., parameter values) from the `name` column.

    Args:
        name_column (pd.Series): The 'name' column of the DataFrame.

    Returns:
        pd.Series: Derived x-axis values.
    """
    # Example: Extract the parameter value after the slash in the name (e.g., "/16" -> 16)
    return name_column.str.extract(r"/(\d+)$")[0].astype(int)

def plot_benchmarks(plot_title, x_axis_name, file_label_pairs, output_path=None, log_y=False, log_x=False, width=10, height=5, legend_position="best", polish=False):
    plt.figure(figsize=(width, height))

    for filepath, label in file_label_pairs:
        metadata, df = parse_csv(filepath)
        df['x_values'] = derive_x_values(df['name'])
        if "time_unit" in df.columns:
            if df['time_unit'].iloc[0] == 'ms':
                df['real_time'] *= 1e6
            elif df['time_unit'].iloc[0] == 's':
                df['real_time'] *= 1e9
        plt.plot(
            df['x_values'], df['real_time'], marker='o', label=label
        )
        print(f"File: {filepath}")
        for key, value in metadata.items():
            print(f"{key}: {value}")
        print()

    plt.xlabel(x_axis_name)
    plt.ylabel("Zmierzony czas CPU (ns)" if polish else "Measured CPU Time (ns)")
    if log_y:
        plt.yscale("log")
    if log_x:
        plt.xscale("log")
    plt.title(plot_title)
    legend_title = "Metody" if polish else "Methods"
    if legend_position == "outside":
        plt.legend(title=legend_title, loc="upper left", bbox_to_anchor=(1.05, 1), borderaxespad=0.)
    elif legend_position == "below":
        plt.legend(title=legend_title, loc="upper center", bbox_to_anchor=(0.5, -0.15), ncol=2)
    else:
        plt.legend(title=legend_title, loc=legend_position)
    plt.grid(True)
    plt.tight_layout()

    if output_path:
        plt.savefig(f"{output_path}.png", bbox_inches="tight")
        plt.savefig(f"{output_path}.pgf", bbox_inches="tight")
        print(f"Plot saved to {output_path}.png and {output_path}.pgf")
    else:
        plt.show()

def main():
    if len(sys.argv) < 4 or "--help" in sys.argv or "-h" in sys.argv:
        print("Usage: python3 plot_benchmarks.py <plot-title> <x-axis-label> <file1.csv> <label1> [<file2.csv> <label2> ...] [-o <output-path>] [--log-y] [--log-x] [--width <value>] [--height <value>] [--legend-position <position>] [--pl]")
        sys.exit(1)

    output_path = None
    log_y = False
    log_x = False
    width = 10
    height = 5
    legend_position = "best"
    polish = False
    args = sys.argv[1:]

    if "--log-y" in args:
        log_y = True
        args.remove("--log-y")

    if "--log-x" in args:
        log_x = True
        args.remove("--log-x")

    if "--pl" in args:
        polish = True
        args.remove("--pl")

    if "--width" in args:
        width_index = args.index("--width")
        if width_index + 1 >= len(args):
            print("Error: Width value must be specified after '--width'.")
            sys.exit(1)
        width = float(args[width_index + 1])
        del args[width_index:width_index + 2]

    if "--height" in args:
        height_index = args.index("--height")
        if height_index + 1 >= len(args):
            print("Error: Height value must be specified after '--height'.")
            sys.exit(1)
        height = float(args[height_index + 1])
        del args[height_index:height_index + 2]

    if "--legend-position" in args:
        legend_index = args.index("--legend-position")
        if legend_index + 1 >= len(args):
            print("Error: Legend position must be specified after '--legend-position'.")
            sys.exit(1)
        legend_position = args[legend_index + 1]
        del args[legend_index:legend_index + 2]

    if "-o" in args:
        output_index = args.index("-o")
        if output_index + 1 >= len(args):
            print("Error: Output file path must be specified after '-o'.")
            sys.exit(1)
        output_path = args[output_index + 1]
        del args[output_index:output_index + 2]

    if len(args) < 4 or len(args[2:]) % 2 != 0:
        print("Error: Each file must be followed by a label.")
        sys.exit(1)

    plot_title = args[0]
    x_axis_name = args[1]
    file_label_pairs = [(args[i], args[i + 1]) for i in range(2, len(args), 2)]

    plot_benchmarks(plot_title, x_axis_name, file_label_pairs, output_path, log_y, log_x, width, height, legend_position, polish)

if __name__ == "__main__":
    main()

