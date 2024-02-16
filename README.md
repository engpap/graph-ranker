
# Graph Ranker

This project implements a graph processing system using Dijkstra's algorithm to find the shortest paths and a ranking system to manage and rank graph results based on their path lengths. The system uses heap data structures for efficient computation.

## Motivation & Results
This project was a mandatory requirement for obtaining a Bachelor's degree from the Politecnico di Milano, Italy.
The score is calculated based on the time and space efficiency. The scoring system tracked the time and space required by the program.
I received a grade of 30 cum laude out of 30.

## Features

- **Dijkstra's Algorithm**: Implementation of Dijkstra's algorithm to find the shortest paths from a source node to all other nodes in a directed graph.
- **Heap Data Structure**: Use of min-heap for managing the priority queue in Dijkstra's algorithm and max-heap for the ranking system.
- **Ranking System**: A system to rank graphs based on the sum of the shortest paths from the source node to all other nodes.
- **Dynamic Input Handling**: The system processes input dynamically, allowing for the addition of graphs and the computation of rankings on the fly.


## Input Format

The program expects input in the following format:

1. First line: Two integers `d` and `k`, where `d` is the dimension of the graph (number of nodes) and `k` is the number of top rankings to maintain.
2. Subsequent lines: Commands and graph data.
   - To add a graph: Use the command `AggiungiGrafo`, followed by `d` lines each containing `d` integers separated by commas, representing the adjacency matrix of the graph.
   - To view the top `k` rankings: Use the command `TopK`.

## Output Format

The program outputs the rankings of graphs based on the sum of their shortest paths. The output is a list of graph indices in descending order of their path lengths.

## Example

Input:

```
4 2
AggiungiGrafo
0,1,4,0
0,0,2,0
0,0,0,3
0,0,0,0
TopK
```

Output:

```
0
```

In this example, a single graph with 4 nodes is added, and the program outputs the index of the graph (0) when the `TopK` command is issued.

## Implementation Details

- The project uses `typedef struct` for defining the heap nodes and rank nodes.
- Several helper functions are implemented for heap operations, such as `swap`, `minHeapify`, `heapDecreaseKey`, and `heapExtractMin`.
- The `parser` function is used for parsing input lines and converting string numbers to integers.

## Limitations

- The program assumes well-formed input and does not handle erroneous or malformed input gracefully.
- The size of the input graph is fixed and determined by the first input line.
