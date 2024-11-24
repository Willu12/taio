## TAiO

University course's project, provides CLI for multigraph (graph theory) comparison.

### Getting started
TODO (dependencies, how to install on different OSes)
#### Nix
There is flake prepared, so it is enough to enter development environment:
```bash
nix develop
```
### Compilation
#### GNU/Linux
**git**

First clone repository, then
```bash
git submodule update --init --recursive
```
**CMake**

We recommend:
```bash
mkdir build
cd build
cmake .. && cmake --build .
```
but similar result can be obtained in many ways using CMake CLI.

### Command Line Interface
```bash
./multigraph_comparator [OPTIONS] file1 file2
```

Compares multigraphs.

#### Positionals
  - `file1` Path to the first multigraph file
  - `file2` Path to the second multigraph file

#### Options
  - `-h,--help`              Print this help message and exit
  - `-i,--index1` uint       Index of the multigraph in the first file (default: 0)
  - `-j,--index2` uint       Index of the multigraph in the second file (default: 0)

#### Example
```bash
./multigraph_comparator file1.txt file1.txt -i 0 -j 1
./multigraph_comparator file1.txt file2.txt
```

### Input format
Multigraphs are saved using their adjacency matrix in following format:
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
example:
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


