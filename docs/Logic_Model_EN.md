# Tapa Game

**Authors:** ALIAN Nastaran, DUPRAZ Cha, GHAREHBAGHI Donya, and SKUBISZ Magdalena

# Introduction

We chose the Tapa game both for its familiarity—several group members already knew this game—and for its complexity. Three main rules appear: local topology, global topology of "rivers" (black zones), and arrangement constraints (clues).

## Tapa Game Rules

1. **Cell Color**: Each cell is either black or white
2. **No 2×2 Black Squares**: No group of 4 adjacent black cells can form a square
3. **Numbers on Cells**: Clues give the length of consecutive black cell blocks in adjacent cells. The number of numbers indicates the number of distinct neighborhoods.
4. **Continuity of Black Cells**: All black cells must form a single orthogonally connected region (no isolated black cells)

Tapa game rules can be reduced to three fundamental constraints: **no 2×2 black squares**, **respect for numerical clues**, and **continuity of black cells**. These three constraints form the **minimal and complete set** characterizing a valid Tapa grid.

### A Minimal Characterization

Each constraint captures a distinct and independent rule from the solution:

- **2×2 Squares**: constraint that is **local and negative** — forbids certain configurations of 4 adjacent cells. It acts on the *shape* of black regions.
- **Numerical Clues**: constraint that is **local and positive** — fixes the possible positions of the neighborhood of a clued cell. It acts on the *number*, *size*, and *position* of black blocks around clues.
- **Continuity**: constraint that is **global** — imposes a structural property on the entire graph of black cells. It acts on the *cohesion* of the solution.

| Constraint | Scale | Modeling Type |
| ---------- | ----- | ------------- |
| 2×2 Squares | Local (4 cells) | Direct negative clauses in CNF |
| Clues | Neighborhood (up to 8 cells) | DNF (arrangements) → CNF |
| Continuity | Global (entire grid) | Auxiliary distance variables → CNF |

These three constraints are **independent**: none can be deduced from the other two.

Any solution simultaneously satisfying these three constraints is a valid puzzle solution, and conversely.

### Conversion to DIMACS Format

Since the three rules are independent, we can initialize a DIMACS file and add them successively in any order. However, it is necessary to keep track of the number of clauses added and the number of auxiliary variables to update the file header at the end of execution.

# Rule Modeling

## Cell Indexing

We represent the grid as a two-dimensional array of size `number_rows × number_columns` indexed from 1.

- Cell indices range from 1 to `total number of cells`
- **Indexing formula**: `cell_index = 1 + column_number + row_number × number_columns`

A cell's index represents its name in DIMACS format.

**Example**:

```
Position (row=1, column=2) in a 3×3 grid
→ Index = 1 + 2 + 1 × 3 = 6
```

## Cell Color

**Variable $X$** to determine a cell's color:

```
X ∈ {
    0 → white cell
    1 → black cell
    }
```

## CONSTRAINT: No 2×2 Black Squares

```
  Valid Configuration              Invalid Configuration
|-----|-----|-----|-----|              |-----|-----|-----|-----|
| $$$ | $$$ | $$$ |     |              |     | $$$ | $$$ |     |
| $$$ | $$$ | $$$ |     |              |     | $$$ | $$$ |     |
|-----|-----|-----|-----|              |-----|-----|-----|-----|
|     |     | $$$ |     |              | $$$ | $$$ | $$$ |     |
|     |     | $$$ |     |              | $$$ | $$$ | $$$ |     |
|-----|-----|-----|-----|     --->     |-----|-----|-----|-----|
|     | $$$ | $$$ |     |              |     |     |     |     |
|     | $$$ | $$$ |     |              |     |     |     |     |
|-----|-----|-----|-----|              |-----|-----|-----|-----|
```

**Rule**: No group of 4 adjacent black cells can form a square

**Logic Modeling**: For every set of cells {i, i+1, i+number_rows, i+number_rows+1} denoted {a,b,c,d}:

```
-a + -b + -c + -d
```

"At least one of these 4 cells is not black"

**Note**: When traversing the grid to generate clauses, if one of the four cells is forced white by a number (rule 3), this set could be omitted.

## CONSTRAINT: Numbers on Cells

Clues (0) give the length of consecutive black cell blocks (1) in adjacent cells (2). The number of numbers (4) indicates the number of distinct neighborhoods (5).

```
    Input Grid                         Solved Grid
|-----|-----|-----|-----|              |-----|-----|-----|-----|
| 1,1 |     |     |     |              | 1,1 | $$$ | $$$ |     |
|     |     |     |     |              |     | $$$ | $$$ |     |
|-----|-----|-----|-----|              |-----|-----|-----|-----|
|     | 7   |     | 3   |              | $$$ | 7   | $$$ | 3   |
|     |     |     |     |              | $$$ |     | $$$ |     |
|-----|-----|-----|-----|     --->     |-----|-----|-----|-----|
|     |     |     |     |              | $$$ | $$$ | $$$ |     |
|     |     |     |     |              | $$$ | $$$ | $$$ |     |
|-----|-----|-----|-----|              |-----|-----|-----|-----|
|     |     |     | 2   |              |     |     | $$$ | 2   |
|     |     |     |     |              |     |     | $$$ |     |
|-----|-----|-----|-----|              |-----|-----|-----|-----|
```

### Formalization

The cell at index $p$ of a clue (0) is always white.

A clue (0) is a grid cell with at least one number written in it. All restrictions imposed by a clue (0) are respected.

A neighborhood (5) is a set of consecutive black cells (1) on graph $C$. Two distinct neighborhoods are separated by at least one white cell.

The clue (0) contains a set of numbers $E$ composing the restrictions imposed by the clue. The size of set $E$ corresponds to the number of distinct neighborhoods (5) adjacent to the clue. The values of elements (4) of $E$ correspond to the sizes of these neighborhoods. Neighborhoods have no order.

Since two distinct neighborhoods (5) are separated by at least one white cell and there are no direct restrictions on the number of white cells, a neighborhood of size $n$ can be oriented and represented on $n+1$ cells: $n$ consecutive black cells (1) followed by a white cell. This representation of neighborhoods will be used hereafter to impose distinction.

For a cyclic graph $C$ of $l$ consecutive cells (1) and a set $F$, imposed by a clue (0) at index $p$, of distinct neighborhoods (5) and cumulatively represented on $l$ cells or fewer, each arrangement $A_{i} ∈ A$ of these neighborhoods on $C$ is modeled by a conjunction $\lambda_{i}$ of literals, each literal representing the cell state for this arrangement (default white if unrestricted).

There exists an arrangement $A_{i} ∈ A$ that is respected so that the clue (0) at index $p$ is respected.

### Modeling

Restrictions of a clue (0) at index $p$ are represented for $l=8$, on the cyclic graph defined such that $C =(V,E)$ with $V$ = {a,b,c,d,e,f,g,h}, $E$ = {(a,b),(b,c),(c,d),(d,e),(e,f),(f,g),(g,h),(h,a)} on the set {$p$, $p$-number_columns-1, $p$-number_columns, $p$-number_columns+1, $p$+1, $p$+number_columns+1, $p$+number_columns, $p$+number_columns-1, $p$-1} denoted $I=$ {x,a,b,c,d,e,f,g,h}, $C$ corresponding to cells adjacent (2) to {x} $(\forall v \in V,v \sim x)$

### Case Distinctions

##### Free Clue

A clue (0) is said to be free if all adjacent cells ($V$) are in the grid (the clue is not on a border/corner) and if none of the adjacent cells is also a clue (in this case: $R = \empty$). In this case all arrangements $A_{i} ∈ A$ are possible, and we will consider the induced DNF of the disjunction of monomials $\lambda_i$

##### Restricted Clue

A clue (0) is said to be restricted if at least one adjacent cell ($V$) is not in the grid (the clue is on a border/corner) or if at least one adjacent cell is also a clue. The set of restrictive cells of the clue is denoted $R \sub V$. In this case only arrangements $A' \subset A$ where none of the elements of $R$ is black in arrangements $A'$ are valid. We will consider the induced DNF of the disjunction of monomials $\lambda_k$ of each arrangement $A'_k ∈ A'$

In practice, when evaluating arrangements, only arrangements $A'$ are computed by the program

### Conversion from DNF to CNF

Transformation from DNF to CNF is performed incrementally, by explicit distributivity.

CNF is initialized from the first monomial, converting its literals into unit clauses.

Then, for each subsequent monomial of the DNF, the literals are integrated into the CNF by updating existing clauses:

- Each clause of the CNF is combined with each literal of the current monomial
- Redundant literals are removed within clauses
- Valid clauses are eliminated
- Clause duplicates are removed after each iteration

This process is repeated sequentially for all monomials of the DNF, producing an equivalent CNF without explicit generation of the form developed by distributivity.

#### Optimization for Free Clues

In the case of a free clue, it can be noted that all clues of the same form have the same DNF/CNF up to isomorphism. Since the CNF of most free clues is on the order of 5000 clauses, to avoid redoing the distribution which is computationally significant, we can pre-calculate and transcribe the CNF into files (present in the archive at: "/indices/indice_abcd.clauses") with the function ***initialise_clauses()*** in indices.c and retrieve them with ***lire_clause(a,b,c,d,\<Destination List>)***. [This allows transcribing clues of a 10x10 grid in ~0.1s versus ~1s without this optimization]

### Conversion to DIMACS Format

Each clause of the CNF being represented on an array of 8 columns, the columns represent respectively a, b, c, d, e, f, g, h as defined on $I$. Possible values for each element are:

```
-1 : literal not present in the clause  
0 : negation of literal present in the clause  
1 : literal present in the clause  
```

Note: by construction, all values represented by the literals of the CNF and of $p$ are in $I \setminus R$ and are therefore defined on the grid.

For each clue we can write the following comment in the file: <br>    `c Clauses for cell (i,j) with clue k0 ... k3`<br>   Where $i$ is the column index and $j$ is the row index induced from $p$, and k0 ... k3 are the sizes of the neighborhoods of the clue (0). For $n$ neighborhoods, only k0 to k(n-1) are written<br>    Followed by the unit clause representing the clue cell which must be white<br>    `p 0` <br>    Each clause of the CNF is then written in the file.

Example (Clue 5 on a grid border):

```
c Clauses for cell (5,6) with clue 5
-55 0
54 0
64 0
65 0
66 0
44 0
```

**Note 1**: Using the following property: two arrangements $A_j$, $A_k$ are equivalent if $\lambda_j ≡ \lambda_k$ when computing arrangements we can obtain a distinct set $A$ (which will avoid duplicates later) <br>**Note 2**: The set of clauses of the CNF and monomials of the DNF is represented in the program by a linked list of arrays, which allows us to perform efficient insertion without duplicates.<br>**Note 3**: The chosen index is intentionally very short, the size of the CNFs being mostly either less than 8 clauses, or (rarely) between 50 and 80 clauses, or on the order of 5000 clauses.<br>**Note 4**: The number of clauses added by a clue (more particularly their sum for all clues) is returned at the end of writing clauses in the file, to update the header of the .dimacs file.

## CONSTRAINT: Continuity of Black Cells

```
 Input Grid                    Solved Grid
|-----|-----|-----|                |-----|-----|-----|
|     |     |     |                | $$$ | $$$ | $$$ |
|     |     |     |                | $$$ | $$$ | $$$ |
|-----|-----|-----|                |-----|-----|-----|
|     | --- |     |      --->      | $$$ | --- | $$$ |
|     | --- |     |                | $$$ | --- | $$$ |
|-----|-----|-----|                |-----|-----|-----|
| $$$ | --- | $$$ |                | $$$ | --- | $$$ |
| $$$ | --- | $$$ |                | $$$ | --- | $$$ |
|-----|-----|-----|                |-----|-----|-----|
```

### Formalization

We treat the grid as a graph $A = (V_A, E_A)$ where each cell represents a vertex, and two vertices are connected by an edge if they are orthogonally adjacent (each vertex thus has 2, 3 or 4 edges depending on its position in the grid).

We also define the induced subgraph $B$ of $A$ by the set of black cells. The continuity rule for black cells is respected if and only if $B$ is connected.

**Note**: Consistent with our version of Tapa rules, the adjacency relation is strictly orthogonal (up, down, left, right). Diagonals do not count as connection.

### Modeling

Directly verifying graph connectivity with a CNF formula is not trivial, because connectivity is not a local property. We therefore use an encoding by **oriented spanning tree** (spanning tree encoding), based on a distance function from a root.

#### Principle

Subgraph $B$ is connected if and only if there exists a spanning tree on $B$. We force the existence of such a tree by assigning each black cell a **distance** relative to a unique root cell:

- The root has distance 0.
- Any other black cell has distance $d \geq 1$ and has at least one orthogonal black neighbor at distance $d-1$.

By induction on $d$, each black cell is connected to the root by a chain of orthogonal black neighbors, which guarantees the connectivity of $B$.

### Auxiliary Variables

For each cell at index $v \in \{1, ..., \text{number\_cells}\}$ and each distance $d \in \{0, 1, ..., D_{\max}\}$ we introduce a boolean variable $D_{v,d}$:

```
D_{v,d} ∈ {
    0 → cell v is not at distance d from the root
    1 → cell v is at distance d from the root
    }
```

where $D_{\max} = \text{number\_cells} - 1$ is the maximum possible distance in the grid.

In DIMACS format, these variables are named after the color variables: `number_cells + (v-1) × number_cells + d + 1`.

### Constraints for Continuity

#### (1) Link between Color and Distance

A cell has a distance if and only if it is black.

**(1a)** If a cell $v$ is black, then it has at least one distance assigned:

$$
X_v \implies \bigvee_{d=0}^{D_{\max}} D_{v,d}
$$

In CNF, for each cell $v$:

```
-X_v + D_{v,0} + D_{v,1} + ... + D_{v,D_max}
```

**(1b)** If a cell $v$ has distance $d$ assigned, then it is black:

$$
D_{v,d} \implies X_v
$$

In CNF, for each cell $v$ and each distance $d$:

```
-D_{v,d} + X_v
```

#### (2) Uniqueness of Distance

Each black cell has **exactly one** distance. The disjunction (at least one) is already given by (1a). It remains to impose **at most one**, encoded by pairs:

For each cell $v$ and each pair $(d_1, d_2)$ with $d_1 < d_2$:

$$
\lnot(D_{v,d_1} \land D_{v,d_2})
$$

In CNF:

```
-D_{v,d_1} + -D_{v,d_2}
```

#### (3) Uniqueness of Root

For the spanning tree to be unique, there can only be one root on the entire grid. If two cells were roots (distance 0), we would have two disjoint trees hence two connected components.

For every pair of cells $(u, v)$ with $u < v$:

$$
\lnot(D_{u,0} \land D_{v,0})
$$

In CNF:

```
-D_{u,0} + -D_{v,0}
```

#### (4) Conditional Existence of Root

If at least one cell is black, then a root must exist. If all cells are white, no root is necessary.

For each cell $v$:

$$
X_v \implies \bigvee_{u=1}^{\text{number\_cells}} D_{u,0}
$$

In CNF:

```
-X_v + D_{1,0} + D_{2,0} + ... + D_{number_cells,0}
```

#### (5) Distance Propagation

This is the central constraint of the encoding. It forces each non-root black cell to be orthogonally adjacent to a cell at the immediately previous distance, which guarantees the existence of a chain to the root.

Let $N_\perp(v)$ be the set of **orthogonal** neighbors of $v$ in the grid (at most 4, fewer at borders and corners).

For each cell $v$ and each distance $d \in \{1, 2, ..., D_{\max}\}$:

$$
D_{v,d} \implies \bigvee_{u \in N_\perp(v)} D_{u, d-1}
$$

In CNF:

```
-D_{v,d} + D_{u_1,d-1} + D_{u_2,d-1} + ... + D_{u_k,d-1}
```

where $u_1, ..., u_k$ are the orthogonal neighbors of $v$ present in the grid.

#### Articulation of Constraints

Constraints (3), (4) and (5) work together:

- **(3)** guarantees the **uniqueness** of the root (otherwise, multiple disjoint trees).
- **(4)** guarantees the **existence** of the root (otherwise, no tree possible for black cells).
- **(5)** guarantees that each black cell is **connected** to the root by an orthogonal chain.

Constraints (1) and (2) ensure consistency between coloring and assigned distances.

#### Conversion to DIMACS Format

Once the logical modeling of the continuity constraint is established (auxiliary variables $D_{v,d}$ and the five families of constraints (1a), (1b), (2), (3), (4) and (5)), it remains to translate this logical formula into a DIMACS format file exploitable by the SAT solver.

##### The Formula is Already in CNF

Unlike the numerical clue constraint, which requires conversion from Disjunctive Normal Form (DNF) to CNF, **the continuity constraint is directly formulated in CNF** from the modeling step onwards. Indeed, each of the five families of constraints naturally produces **clauses** (disjunctions of literals):

| Constraint | Logical Form | Generated Clause Type |
| ---------- | ------------ | --------------------- |
| (1a) | $\lnot X_v \lor \bigvee_d D_{v,d}$ | Long clause of size $D_{\max} + 2$ |
| (1b) | $\lnot D_{v,d} \lor X_v$ | Binary clause |
| (2) | $\lnot D_{v,d_1} \lor \lnot D_{v,d_2}$ | Binary clause |
| (3) | $\lnot D_{u,0} \lor \lnot D_{v,0}$ | Binary clause |
| (4) | $\lnot X_v \lor \bigvee_u D_{u,0}$ | Long clause of size $N + 1$ |
| (5) | $\lnot D_{v,d} \lor \bigvee_{u \in N_\perp(v)} D_{u,d-1}$ | Clause of size 3 to 5 |

The set of these clauses, linked by conjunction, forms a formula in CNF. No further transformation is necessary before writing to the DIMACS file.

##### Variable Numbering

The DIMACS format requires variables to be identified by **strictly positive integers**. A variable is represented by its positive integer, its negation by the corresponding negative integer. Our project uses two families of variables:

###### Color Variables $X_v$

These variables already exist at the previous project step (2×2 square constraint, clues). They occupy identifiers $\{1, 2, \dots, N\}$ where $N = \text{number\_rows} \times \text{number\_columns}$. The indexing formula is:

```
index_X = 1 + column_number + row_number × number_columns
```

###### Auxiliary Distance Variables $D_{v,d}$

For the continuity constraint, we introduce $N \times N$ auxiliary variables $D_{v,d}$, one for each pair (cell, possible distance). To avoid any collision with $X_v$ identifiers, these variables are numbered **after** the color variables, starting from $N + 1$.

The indexing formula is:

```
index_D(v, d) = N + (v - 1) × N + d + 1
```

with $v \in \{1, \dots, N\}$ and $d \in \{0, \dots, D_{\max}\}$.

These variables thus occupy the interval $[N+1, N + N^2]$.

**Example** for a $3 \times 3$ grid ($N = 9$):

| Variable | DIMACS Identifier |
| -------- | ----------------- |
| $X_1, X_2, \dots, X_9$ | $1, 2, \dots, 9$ |
| $D_{1,0}$ | $9 + (1-1) \times 9 + 0 + 1 = 10$ |
| $D_{1,1}$ | $9 + 0 + 1 + 1 = 11$ |
| $D_{1,8}$ | $9 + 0 + 8 + 1 = 18$ |
| $D_{2,0}$ | $9 + 9 + 0 + 1 = 19$ |
| $D_{9,8}$ | $9 + 72 + 8 + 1 = 90$ |

The maximum identifier is thus $N + N^2 = 9 + 81 = 90$ for this example.

##### Writing Clauses

Each clause is written on a line of the file, as a sequence of integers (separated by spaces) terminated by `0`. Positive literals (affirmed variable) are noted directly, negative literals (negated variable) are prefixed by `-`.

**Example** for constraint (1a) applied to cell $v = 1$ of a $3 \times 3$ grid:

$$
\lnot X_1 \lor D_{1,0} \lor D_{1,1} \lor D_{1,2} \lor \dots \lor D_{1,8}
$$

is written in the DIMACS file:

```
-1 10 11 12 13 14 15 16 17 18 0
```

**Example** for constraint (5) applied to central cell $v = 5$ at distance $d = 2$ (its orthogonal neighbors are $v = 2, 4, 6, 8$):

$$
\lnot D_{5,2} \lor D_{2,1} \lor D_{4,1} \lor D_{6,1} \lor D_{8,1}
$$

is written:

```
-48 20 38 56 74 0
```

Note: identifiers are calculated according to the ad hoc formula

# Tests

The construction of the rules allows us to test each rule individually or in combination. We created a functional test suite to verify the correct functioning of the rules, and to verify that they work correctly when applied together. These tests verify both satisfiable and unsatisfiable cases.

The test file is structured as follows:

```
/JeuTests/OK/indices
            /indices-continuite
            /continuite
            /2x2
            /2x2-indices
            /2x2-continuite
            /2x2-indices-continuite
         /ERREUR <same subdirectories>
```

The tests present were specifically constructed for each different combination of rules (the ad hoc illustrations are part of these tests). To be able to test combinations of rules that do not have the "clue" constraint, we changed the grid format to accept having specific cells forced white or black.

To this manually constructed test suite, we added to all OK subdirectories at least 4 imported grids, different for each folder.

A `/GrillesImporte` file contains a large number of grids imported from www.puzzle-tapa.com. Since the construction of these grids is correct, each of these grids can be run with any combination of rules and is satisfiable.

We did not create format robustness tests on grids, as it is inconsistent: since grids have construction constraints, we assumed that the input format was respected.

**Note 1**: Only 3 grids of 15x15 were used in the test folder for rule combinations including continuity for time reasons.<br>**Note 2**: Some tests are unsatisfiable for their rules but satisfiable for a subset of rules; conversely, some tests in OK are satisfiable for the given rules, but unsatisfiable as soon as an additional rule is added.

# Conclusion

The passage of all formulated tests, as well as all grids from the website [www.puzzle-tapa.com](http://www.puzzle-tapa.com) that we imported, proving satisfactory and correctly executed, and given the formalization of each rule and the strict adherence to this formalization in their transcription to C, we have a high level of confidence that our program is capable of solving any Tapa grid, regardless of size, provided we have sufficient time and memory.

### Limitations

This program takes Tapa grids as input, which can be satisfiable or unsatisfiable, but it is expected that they respect an expected format. Illegal clues cause undefined behavior.

Grids can be rectangular; grids of other shapes are not supported.

The execution time of the continuity constraint is exponential: solving a 15x15 grid takes ~15 s and a 20x20 grid ~1 min 20 s. Time and the number of variables make solving larger grids functionally difficult.<br>The other 2 rules execute in O(N), N: number of cells or number of clues respectively.

# Prerequisites

- C compiler installed on your machine
  @ (e.g.: gcc, clang…) the Makefile uses the default compiler on your machine
- Makefile installed on your machine
  @ (apt install make / apt install build-essential)

The SAT solver used is picosat
@ https://fmv.jku.at/picosat/

The picosat.c and picosat.h modules were copied from the archive https://fmv.jku.at/picosat/picosat-965.tar.gz

No installation of the SAT solver is required, the files are already present in the provided archive.

COMPILATION:
To compile or recompile, copy the following command to your terminal at the location where the archive was extracted:
make

To clean the files created during compilation:
make clean

External Sources:
Grids named \<size>x\<size>Normal\<identifier> and \<size>x\<size>Hard\<identifier>
were extracted from https://www.puzzle-tapa.com/print.php
SAT Solver:
https://fmv.jku.at/picosat/

# Usage

`./TapaSolver <flags> <file>`

```
| Flag | Rule Enabled |
| 2 | 2x2 Squares |
| i | Clues/Indices |
| c | Continuity of Black Cells |
```

Flags can be combined. For example, to enable all rules:
`./TapaSolver 2ic tests/my_grid`
