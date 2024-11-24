# TAiO

University course project that provides a CLI for multigraph (graph theory) operations, including comparison and finding minimal \(k\)-Hamiltonian extensions.

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

---

## Command Line Interface

The CLI has two subcommands: `compare` and `find_hamiltonian_extension`.

### Subcommands

#### **`compare`**: Compares two multigraphs from files.
```bash
./multigraph_cli compare [OPTIONS] file1 file2
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

##### **Example**
```bash
./multigraph_cli compare file1.txt file1.txt -i 0 -j 1
./multigraph_cli compare file1.txt file2.txt
```

---

#### **`find_hamiltonian_extension`**: Finds minimal \(k\)-Hamiltonian extensions for a multigraph.
```bash
./multigraph_cli find_hamiltonian_extension [OPTIONS] filepath
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

##### **Example**
```bash
./multigraph_cli find_hamiltonian_extension graph.txt -i 0 -k 2
./multigraph_cli find_hamiltonian_extension graph.txt
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

