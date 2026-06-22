# LcVRP — Genetic Algorithm Solver

> **C++ implementation of a Genetic Algorithm for the Limited Capacitated Vehicle Routing Problem (LcVRP).**
> Minimises total route distance across a fleet of capacity-constrained vehicles using tournament selection, single-point crossover, and elitism — built as a university mini-project for the *Techniki Efektywnego Programowania* course at Wrocław University of Technology.

---

## Table of Contents

- [Problem Description](#problem-description)
- [Algorithm Overview](#algorithm-overview)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Getting Started](#getting-started)
  - [Clone the Repository](#clone-the-repository)
  - [Build](#build)
  - [Run](#run)
- [Instance Files](#instance-files)
- [Configuration](#configuration)
- [Output](#output)

---

## Problem Description

The **Limited Capacitated Vehicle Routing Problem** is a combinatorial optimisation problem in which a fleet of vehicles departs from a depot to serve a set of customers. Each vehicle has a fixed **capacity** and each customer has a **demand**. The goal is to find a set of routes that:

- serves every customer exactly once,
- respects vehicle capacity on each route,
- minimises the **total travel distance**.

---

## Algorithm Overview

| Step | Details |
|---|---|
| **Initialisation** | Random population of `POP_SIZE` individuals |
| **Encoding** | Each gene holds a group (route) index for one customer |
| **Evaluation** | Sum of Euclidean inter-stop distances; infeasible routes penalised |
| **Selection** | Tournament selection (`TOURNAMENT_SIZE = 3`) |
| **Crossover** | Single-point crossover with probability `CROSS_PROB` |
| **Mutation** | Per-gene random reassignment with probability `MUT_PROB` |
| **Elitism** | Top `ELITE_SIZE` individuals carried to next generation unchanged |
| **Termination** | After `MAX_ITER` iterations |

Progress is printed every `PRINT_INTERVAL` iterations.

---

## Project Structure

```
.
├── main.cpp                  # Entry point — set instance path here
├── GeneticAlgorithm.h/cpp    # GA driver: population loop, selection, crossover, mutation
├── Individual.h/cpp          # Solution encoding, crossover and mutation operators
├── Evaluator.h/cpp           # .lcvrp file loader, distance matrix, fitness function
├── const.h                   # All tunable parameters in one place
├── smartPointer/
│   ├── MySmartPointer.h      # Custom reference-counted smart pointer template
│   └── RefCounter.h          # Reference counter helper class
└── data/
    └── lcvrp/
        ├── Vrp-Set-A/        # Augerat Set A benchmark instances
        ├── Vrp-Set-B/        # Augerat Set B benchmark instances
        ├── Vrp-Set-E/        # Christofides Set E instances
        ├── Vrp-Set-X/        # Large-scale X instances
        └── ...               # CMT, Golden, Li, M, P, F, tai, XML100, XXL sets
```

---

## Requirements

- **CMake** 3.20+ (or 4.0+ to match the project's `CMakeLists.txt`)
- **C++17**-capable compiler — GCC 7+, MSVC 2017+, or Clang 5+
- No external libraries — standard library only

> **Windows users:** CLion ships with a bundled MinGW toolchain (`<CLion install>\bin\mingw\bin\`). If CLion is installed, no separate compiler installation is needed — just open the project.

---

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/<your-username>/LcVRP.git
cd LcVRP
```

### 2. Choose an instance

Open `main.cpp` and set the path to the `.lcvrp` file you want to solve.
The path is **relative to the working directory of the executable**, so it depends on how you build:

| Build method | Executable location | Correct path prefix |
|---|---|---|
| CLion (CMake, default) | `cmake-build-debug\` | `../data/lcvrp/...` |
| Manual build from repo root | repo root | `data/lcvrp/...` |

Example for CLion (default):

```cpp
// main.cpp  line 11
string filePath = "../data/lcvrp/Vrp-Set-A/A-n32-k5.lcvrp";
```

### 3. Build & Run

#### Option A — CLion (recommended)

1. Open the `LcVRP` folder in CLion (`File → Open`).
2. CLion auto-detects `CMakeLists.txt` and configures the project.
3. Press **Run** (▶) or `Shift+F10`.

The `CMakeLists.txt` already lists all source files — no manual compiler flags needed.

#### Option B — CMake from the command line

```bash
# From the repo root
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run (exe is in build/)
# Windows:
build\LcVRP.exe
# Linux / macOS:
./build/LcVRP
```

With this method the exe runs from `build\`, so use `../data/lcvrp/...` as the path prefix.

#### Option C — Single g++ command (Linux / macOS / MinGW)

```bash
# Run from the repo root
g++ -std=c++17 -O2 -o lcvrp main.cpp GeneticAlgorithm.cpp Individual.cpp Evaluator.cpp

# The exe runs from the repo root, so use:  data/lcvrp/...
./lcvrp        # Linux / macOS
lcvrp.exe      # Windows MinGW
```

> **Important:** Never compile only `main.cpp` — you will get linker errors for `Evaluator`, `GeneticAlgorithm`, and `Individual`. Always include all four `.cpp` files.

---

## Instance Files

Benchmark instances are included under `data/lcvrp/` in the standard `.lcvrp` format (compatible with [CVRPLIB](http://vrp.atd-lab.inf.puc-rio.br/)):

| Set | Instances | Nodes |
|---|---|---|
| Vrp-Set-A | 27 | 32 – 80 |
| Vrp-Set-B | 23 | 31 – 78 |
| Vrp-Set-E | 13 | 13 – 101 |
| Vrp-Set-CMT | 14 | varies |
| Vrp-Set-F | 3 | 45 – 135 |
| Vrp-Set-Golden | 20 | varies |
| Vrp-Set-Li | 12 | varies |
| Vrp-Set-M | 5 | 101 – 200 |
| Vrp-Set-P | 24 | 16 – 101 |
| Vrp-Set-X | 36 | 101 – 1001 |
| Vrp-Set-XML100 | 27 | 100 |
| Vrp-Set-XXL | 10 | large |
| Vrp-Set-tai | 13 | 75 – 385 |
| Vrp-Set-D (Loggi/ORTEC) | 12 | 242 – 1001 |

---

## Configuration

All parameters live in `const.h` — no recompilation of other files needed when tuning:

```cpp
// const.h
namespace Config {
    constexpr int    POP_SIZE        = 300;    // Population size
    constexpr int    NUM_GROUPS      = 5;      // Number of vehicle routes
    constexpr double MUT_PROB        = 0.01;   // Per-gene mutation probability
    constexpr double CROSS_PROB      = 0.85;   // Crossover probability
    constexpr int    MAX_ITER        = 10000;  // Number of generations
    constexpr int    ELITE_SIZE      = 2;      // Elitism — top N survivors
    constexpr int    TOURNAMENT_SIZE = 3;      // Tournament selection pool size
    constexpr int    PRINT_INTERVAL  = 1000;   // Log every N iterations
    constexpr int    SEED            = 69420;  // RNG seed (reproducibility)
    constexpr int    WRONG_SOL       = 100000; // Penalty for infeasible solutions
}
```

---

## Output

The GA prints a progress line every `PRINT_INTERVAL` iterations and a final summary:

```
Instancja wczytana pomyslnie.
Liczba lokalizacji: 34
Liczba grup: 5
Pojemnosc pojazdu: 100

[Iter 1000] Best fitness: 892.43
[Iter 2000] Best fitness: 847.11
...
[Iter 10000] Best fitness: 799.56
```

A lower fitness value means a shorter total route distance.

---

## Notes

- Custom smart pointers (`MySmartPointer<T>`) are used throughout — no `std::shared_ptr` or `std::unique_ptr` — as part of the course requirement.
- The RNG is seeded with `Config::SEED` for reproducible runs. Change it in `const.h` for different random behaviour.
