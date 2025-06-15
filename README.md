# Compiler Project

This is an assignment for the Compilers course at the Federal University of Rio Grande do Sul (UFRGS), taught by Professor Lucas Mello Schnorr. The project is divided by the typical phases of a compiler, having each step built in a different directory:

```
compilers-project
├── e1 # Lexic Analyser
├── e2 # Syntactic Analyser
│ └── tests
├── e3 # Abstract Syntax Tree
│ ├── images
│ └── tests
├── e4 # Semantic Analyser
│ └── tests
└── e5 # Code Generator
```

## How to Run

### E1

Receives entries via console:

```console
make
./etapa1
```

Or, to use a test file:

```console
./etapa1 < tests/test_file.txt
```

### E2

```console
make
./etapa2 < tests/test_file.txt
```

### E3

```console
make
./etapa3 < tests/test_file.txt
```

You can also create a PNG of the code Syntax Tree:

```console
./etapa3 < tests/test_file.txt > exit.dot ; dot exit.dot -Tpng -o images/graph.png
```

### E4

```console
make
./etapa4 < tests/test_file.txt
```
