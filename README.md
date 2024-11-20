## TAiO

University course's project, provides CLI for multigraph (graph theory) comparison.

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

### Command Line Interface
```bash
./multigraph_comparator [OPTIONS] file1 file2
```

Compares multigraphs.

POSITIONALS:
  - file1 TEXT:FILE REQUIRED    Path to the first multigraph file
  - file2 TEXT:FILE REQUIRED    Path to the second multigraph file

OPTIONS:
  - -h,     --help              Print this help message and exit
  - -i,     --index1 UINT [0]   Index of the multigraph in the first file (default: 0)
  - -j,     --index2 UINT [0]   Index of the multigraph in the second file (default: 0)

Example:
```bash
./multigraph_comparator file1.txt file1.txt -i 0 -j 1
./multigraph_comparator file1.txt file2.txt
```
