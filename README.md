# LcVRP Genetic Algorithm

A C++ implementation of a Genetic Algorithm (GA) for solving the **Limited Capacitated Vehicle Routing Problem (LcVRP)** — a university mini-project for the *Techniki Efektywnego Programowania* course at Wrocław University of Technology.

## Problem

LcVRP assigns customers to vehicle routes (groups) minimizing total travel distance while respecting vehicle capacity constraints.

## Project Structure

| File | Description |
|---|---|
| `GeneticAlgorithm.h/cpp` | GA driver — population management, crossover, mutation, evaluation loop |
| `Individual.h/cpp` | Single solution — genotype encoding, crossover, mutation |
| `Evaluator.h/cpp` | Loads `.lcvrp` instance files, computes fitness |
| `MySmartPointer.h` | Custom reference-counted smart pointer |
| `RefCounter.h` | Reference counter helper |
| `const.h` | Global constants (population size, probabilities, iterations) |

## Algorithm

1. Initialize random population (`POP_SIZE = 1024`)
2. Evaluate all individuals
3. Tournament selection + single-point crossover (`CROSS_PROB = 0.6`)
4. Per-gene random mutation (`MUT_PROB = 0.2`)
5. Repeat for `MAX_ITER = 10000` iterations

Each gene encodes a group (route) assignment for one customer location.

## Configuration

Edit `const.h` to tune parameters:

```cpp
POP_SIZE_  = 1024   // Population size
NUM_GROUPS_ = 5     // Number of vehicle routes
MUT_PROB_  = 0.2    // Mutation probability per gene
CROSS_PROB_ = 0.6   // Crossover probability
MAX_ITER_  = 10000  // Stopping criterion
```

## Usage

Update the instance file path in `main.cpp`, then build and run:

```cpp
string path = "path/to/instance.lcvrp";
```

```bash
g++ -std=c++11 -O2 -o lcvrp *.cpp
./lcvrp
```

## Notes

- Smart pointers (`indPtr`, `evPtr`) are custom implementations — no STL smart pointers used.
- Compatible with MSVC 2017+ and GCC with C++11.
