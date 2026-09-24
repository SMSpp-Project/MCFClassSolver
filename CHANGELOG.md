# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `MCFSolver::get_var_direction()`, which threw "not implemented yet": it
  writes one unit of flow along the cycle of negative cost and infinite
  capacity that the :MCFClass gives as the certificate of unboundedness [see
  `MCFClass::MCFGetUnbCycl()`] and tells the MCFBlock that its flow Variable
  hold a direction; `get_Solution()` on an unbounded instance gives a
  `MCFSolution` that says it holds a direction rather than the flow of a
  solution that there is not. A :MCFClass that gives no certificate, which
  the base class allows, makes the former throw and the latter give nothing

- the certificate itself in two of the solvers of the MCFClass submodule:
  `MCFSimplex` keeps the cycle of the pivot whose step is infinite, while
  `SPTree`, which used to cycle for ever on a directed cycle of negative
  cost, stops on it and reads it off its predecessor function

### Changed

- whoever links the module keeps it: the classes of a module register
  themselves in the factory from a static initialiser, and a linker that
  drops what looks unused takes the registration away with it, so the target
  now tells whoever links it to keep the symbol that forces the module in,
  and on ELF, where naming the symbol is not enough, the library as a whole

- the makefile of the test asks for `-O3 -DNDEBUG` and nothing else, the
  macro of the patch for `boost::any` on macOS having no reason to be there
  since there is no `boost::any` left in the core

### Fixed

- `RelaxIV` of the MCFClass submodule raised its finite stand-in for an
  infinite capacity only at `LoadNet()`, so a later `ChgDfcts()` asking for
  more flow than that could make it declare a feasible instance unfeasible,
  and `ChgDfcts()` on a range of nodes wrote the node before the range

- `SPTree::MCFArcs()` of the same submodule gave the arcs of the graph
  mismatched with their names, reading the dictionary of the Forward Star
  the wrong way round

## [0.2.0] - 2026-09-12

### Added

- `MCFSolver::get_Solution()`, which builds the MCFSolution out of the data
  structures of the MCFClass solver, without writing anything into the
  MCFBlock and therefore without requiring any Variable to exist

### Changed

- the version of the module is the git tag of its repository, or the
  VERSION.txt of a release tarball, and the shared library carries it: its
  SONAME is major.minor while the major is 0, and it is installed with an
  RPATH relative to itself, so that an installed tree keeps working wherever
  it is moved

[Unreleased]: https://gitlab.com/smspp/mcfclasssolver/-/compare/0.2.0...develop
[0.2.0]: https://gitlab.com/smspp/mcfclasssolver/-/compare/0.1.0...0.2.0
