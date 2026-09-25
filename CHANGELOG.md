# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- the tests of this directory carry the label of the module, so that the
  pipeline, which selects with `ctest -L <module>`, runs them: they were built
  and never run

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
  `SPTree` reads it off its predecessor function

### Changed

- `batch-l` runs one seed of its three when `$CI` is set: the three are the
  same sweep with another random stream, while the whole of it takes 41
  minutes on a machine of ours, which is more than a shared runner has to
  give a single test

- whoever links the module keeps it: the classes of a module register
  themselves in the factory from a static initialiser, and a linker that
  drops what looks unused takes the registration away with it, so the target
  now tells whoever links it to keep the symbol that forces the module in,
  and on ELF, where naming the symbol is not enough, the library as a whole

- the makefile of the test asks for `-O3 -DNDEBUG` and nothing else, the
  macro of the patch for `boost::any` on macOS having no reason to be there
  since there is no `boost::any` left in the core

### Fixed

- on macOS a program linking the module lost the classes the module
  registers in the factories when the linker dropped the library, as it
  does under `-dead_strip_dylibs`, which conda sets: the target now asks the
  linker for the symbol that forces the module in (`-u`), which ld64,
  unlike the ELF linker, counts as a use of the library

- `SPTree` of the MCFClass submodule cycled for ever on a directed cycle of
  negative cost, which is what makes the instance unbounded: it stops on it
  and gives it as the certificate

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
