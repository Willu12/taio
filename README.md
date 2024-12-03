# TAiO

University course project that provides a CLI for multigraph (graph theory) operations, including comparison, finding minimal \(k\)-Hamiltonian extensions, and identifying maximal \(k\)-cycles.

---

## Getting Started

### Dependencies and Installation

#### Using Nix
If you're using Nix, a development environment is already set up with a flake. You can enter it with:

```bash
nix develop
```

### Compilation

#### GNU/Linux

1. **Clone the repository**
   Clone the repository and initialize submodules:
   ```bash
   git clone <repository-url>
   cd <repository>
   git submodule update --init --recursive
   ```

2. **Build with CMake**
   The recommended approach is:
   ```bash
   mkdir build
   cd build
   cmake .. && cmake --build .
   ```
   You can use other CMake commands as long as they generate a compatible build.

#### Windows
We recommend using Visual Studio and compile the project using Developer PowerShell.

1. **Clone the repository**
   Clone the repository and initialize submodules:
   ```bash
   git clone <repository-url>
   cd <repository>
   git submodule update --init --recursive
   ```

2. **Build with CMake**
   The recommended approach is:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
---

## Command Line Interface

The CLI has three subcommands: `compare`, `find_hamiltonian_extension`, and `find_max_cycles`.

### Subcommands

#### **`compare`**: Compares two multigraphs from files.
```bash
./app compare [OPTIONS] file1 file2
```

##### **Positionals**
- `file1`
  Path to the first multigraph file.
- `file2`
  Path to the second multigraph file.

##### **Options**
- `-h,--help`
  Print this help message and exit.
- `-i,--index1` `<uint>`
  Index of the multigraph in the first file (default: 0).
- `-j,--index2` `<uint>`
  Index of the multigraph in the second file (default: 0).
- `--approx`
  Use a heuristic metric for comparison.
- `--counting-sort`
  Use counting sort in heuristic metric.

##### **Example**
```bash
./app compare file1.txt file1.txt -i 0 -j 1
./app compare file1.txt file2.txt --approx
```

---

#### **`find_hamiltonian_extension`**: Finds minimal \(k\)-Hamiltonian extensions for a multigraph.
```bash
./app find_hamiltonian_extension [OPTIONS] filepath
```

##### **Positionals**
- `filepath`
  Path to the multigraph file.

##### **Options**
- `-h,--help`
  Print this help message and exit.
- `-i,--index` `<uint>`
  Index of the multigraph in the file (default: 0).
- `-k` `<uint>`
  Value for \(k\) in the minimal \(k\)-Hamiltonian extension (default: 1).
- `--approx`
  Use an approximation algorithm (not implemented).

##### **Example**
```bash
./app find_hamiltonian_extension graph.txt -i 0 -k 2
./app find_hamiltonian_extension graph.txt
```

---

#### **`find_max_cycles`**: Finds all maximal \(k\)-cycles of a multigraph.
```bash
./app find_max_cycles [OPTIONS] filepath
```

##### **Positionals**
- `filepath`
  Path to the multigraph file.

##### **Options**
- `-h,--help`
  Print this help message and exit.
- `-i,--index` `<uint>`
  Index of the multigraph in the file (default: 0).
- `-k` `<uint>`
  Value for \(k\) in maximal cycle finding (default: 1).
- `--approx`
  Use an approximation algorithm for finding cycles.
- `-p,--print` `<uint>`
  Maximum amount of printed cycles.  

##### **Example**
```bash
./app find_max_cycles graph.txt -i 0 -k 3 -p 10
./app find_max_cycles graph.txt --approx
```

---

### Multigraph Input Format

Multigraphs are saved using their adjacency matrix in the following format:

```
N <- count of multigraphs saved in file
n_1 <- count of vertices in multigraph of index 1
r_1 <- first row of adjacency matrix
...
r_{n_1} <- n_1-th row of adjacency matrix
<- empty line indicating end of multigraph 1
n_2
r_1
...
r_{n_2}

...
n_N
r_1
...
r_{n_N}
```

#### Example Input File
```
2
2
0 2
1 0

3
0 1 2
2 0 1
1 0 0
```

This file defines two multigraphs:
1. A graph with 2 vertices and adjacency matrix:
   ```
   0 2
   1 0
   ```
2. A graph with 3 vertices and adjacency matrix:
   ```
   0 1 2
   2 0 1
   1 0 0
   ```
