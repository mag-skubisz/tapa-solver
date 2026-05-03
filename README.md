# Tapa Solver

A SAT-based solver for the Tapa puzzle game. This project implements a constraint satisfaction approach to solve Tapa puzzles by converting the game rules into logical constraints and using the PicoSAT SAT solver.

**Authors:** ALIAN Nastaran, DUPRAZ Cha, GHAREHBAGHI Donya, SKUBISZ Magdalena

## Overview

Tapa is a logic puzzle where you need to determine which cells in a grid are black and which are white, following specific rules:

1. **No 2×2 black squares** - No four adjacent cells can all be black
2. **Number clues** - Numbers indicate the lengths of consecutive black cell groups adjacent to that cell
3. **Black cell continuity** - All black cells must form a single connected region

## Project Structure

```
tapa-solver/
├── TapaSolver.c          # Main entry point
├── grille.c/h            # Grid representation and parsing
├── dimacs.c/h            # DIMACS SAT format output
├── sat.c/h               # SAT solver interface
├── indices.c/h           # Clue constraint generation
├── carre2x2.c/h          # 2×2 square constraint generation
├── continuite.c/h        # Continuity constraint generation
├── picosat.c/h           # PicoSAT solver (external library)
├── Makefile              # Build configuration
├── run_tests.sh          # Automated test runner
├── grillesImporte/       # Test puzzle grids (various sizes)
├── JeuTests/             # Test cases (OK/ and ERREUR/ subdirectories)
└── docs/                 # Documentation
    ├── Logic_Model_EN.md # Tapa game rules and their modeling in English
    ├── Logic_Model_FR.md # Tapa game rules and their modeling in French
```

## Prerequisites

- **C compiler** (gcc, clang, etc.)
- **Make** build tool
- **Bash** (for test scripts)

### For Windows Users
WSL (Windows Subsystem for Linux) is recommended for running this project.

## Installation & Compilation

1. Clone or extract the project
2. Compile the project:
   ```bash
   make
   ```
3. Set up test execution permissions (one-time):
   ```bash
   chmod +x run_tests.sh
   ```

To clean compiled files:
```bash
make clean
```

## Usage

### Basic Syntax
```bash
./TapaSolver <flags> <grid_file>
```

### Flags

| Flag | Rule Enabled |
|------|--------------|
| `2`  | 2×2 square constraint |
| `i`  | Number clue constraints |
| `c`  | Black cell continuity constraint |

Flags can be combined. For example, to enable all rules:
```bash
./TapaSolver 2ic grillesImporte/6x6Normal1
```

**Note:** Clues are always partially active (forcing white/black cells), the `i` flag additionally enables neighborhood constraints.

### Examples

Solve a 6×6 puzzle with all rules:
```bash
./TapaSolver 2ic grillesImporte/6x6Normal1
```

Solve with only continuity constraint:
```bash
./TapaSolver c grillesImporte/10x10Normal1
```

## Running Tests

Automated tests are available via:

```bash
./run_tests.sh
```

The test suite expects:
- `JeuTests/OK/` - Grids that should have a solution (SAT)
- `JeuTests/ERREUR/` - Grids that should have no solution (UNSAT)

Test flags are automatically inferred from directory names (e.g., `2x2`, `indices`, `continuite`).

Each test displays `PASS` or `FAIL` with obtained vs. expected results.

## SAT Solver

This project uses **PicoSAT** (v965) for SAT solving:
- Website: https://fmv.jku.at/picosat/
- No external installation required (included in archive)

## Test Grids

The `grillesImporte/` directory contains test puzzles sourced from:
- https://www.puzzle-tapa.com/print.php

Available sizes:
- 6×6 (Normal & Hard variants)
- 10×10 (Normal & Hard variants)
- 15×15 (Normal & Hard variants)
- 20×20 (Normal & Hard variants)

## File Format

Input grid files contain:
- Grid dimensions
- Grid values (numbers for clues, 0 for empty cells)

The solver processes these and outputs SAT results.

## Documentation

For detailed constraint modeling and technical documentation, see:
- [docs/Logic_Model_EN.md](docs/Logic_Model_EN.md) - English documentation
- [docs/Logic_Model_EN.md](docs/Logic_Model_FR.md) - French documentation

## Possible Extensions

- **Graphical User Interface** - Develop a GUI beyond terminal-based interaction for improved user experience
- **Solution Verification** - Implement tests that verify grid correctness and solution validity, beyond just satisfiability checking
- **Continuity Constraint Optimization** - Improve the continuity rule by refining the modeling approach or reducing the number of auxiliary variables, particularly distance variables, to enhance solver performance on larger grids

## Note for Non-French Speakers

As this was a course project, several naming conventions, code comments, and terminal messages are written in **French**. This may be unintuitive for non-French speakers. The project structure and logic are well-documented in English through the [Logic_Model_EN.md](docs/Logic_Model_EN.md) file and this README. For detailed explanations of the French terminology in the code, refer to [Logic_Model_FR.md](docs/Logic_Model_FR.md).
