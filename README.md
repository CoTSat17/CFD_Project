# CFD_Project

A personal finite-volume solver for heat conduction (and, in the longer term, CFD), written in **C** from scratch. The goal of the project is to understand and implement the core pieces of a CFD code (mesh data structures, boundary handling, solver loop) rather than to use an existing framework.

> **Status: work in progress.** The mesh and input-reading infrastructure is in place. GPU acceleration and the Navier-Stokes equations are planned but **not implemented yet**.

## Current features

- Unstructured mesh representation: nodes, elements, faces and interfaces between elements
- Boundary conditions handled through a dedicated module
- Material definitions
- Plain-text input file reader with keyword sections
- Unit tests written with **Google Test** (C++)

## Project structure

```
CFD_Project/
├── CFD_Project/      # Solver source code (C)
├── G_TESTS/          # Google Test unit tests (C++)
└── CFD_Project.slnx  # Visual Studio solution
```

Main modules (in `CFD_Project/`):

| Module | Purpose |
|---|---|
| `node` | Mesh nodes and coordinates |
| `element` | Mesh elements |
| `faces` / `interface` | Faces and interfaces between neighbouring elements |
| `boundary_conditions` | Boundary condition definition and application |
| `materials` | Material properties |
| `setup_reader` | Parses the input file |

## Input file format

The solver reads a plain-text file organised in keyword sections:

```
NODES
ELEMENT_*
INIT_VALUES
MATERIALS
SOLVER
```

Each section contains the data for that part of the problem (mesh, initial values, material properties, solver settings). *(Add a short example input file here, for instance in an `examples/` folder, and paste a few lines.)*

## Build and run

The project is developed with **Visual Studio** on Windows.

1. Open `CFD_Project.slnx` in Visual Studio.
2. Build the solution (Release or Debug).
3. Run the solver with an input file *(add the exact command or debugging argument you use)*.

## Tests

Unit tests live in `G_TESTS/` and use Google Test. To run them, build and run the test project from the same Visual Studio solution.

## Roadmap

- [ ] Finish boundary conditions in the input file
- [ ] Verification against analytical heat-conduction solutions
- [ ] Learn CUDA and port the flux computation and element value updates to GPU
- [ ] Implement the Navier-Stokes equations
- [ ] Couple the code with an optimisation workflow

## About

Developed by Pedro Rodríguez Lorenzo, mechanical engineer specialised in numerical simulation, as a way to move from using simulation tools to developing them.
