# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `SPTree` of the MCFClass submodule has the parameter `kNegCycl`, which
  `MCFSolver< SPTree >` gives as its own: with the default `kNoNegCst` the
  costs are taken as nonnegative, with `kNoNegCycl` as giving no cycle of
  negative cost, and in both cases the bound of the check of a negative
  cycle is not computed; `kMayNegCycl` computes it again after each change
  of the costs, and it is the value to give when a negative cycle has to be
  found and reported as unbounded

- `dblRelAcc` and `dblFAccSol` of `MCFSolver` are relative tolerances: if
  positive, `compute()` sets `kEpsCst` to `dblRelAcc` times the largest
  absolute value of a cost, and `kEpsFlw` to `dblFAccSol` times the largest
  among the finite capacities and the absolute deficits, computing them
  again only after a change of the data; the default 0 leaves the absolute
  tolerances of MCFClass

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

- `SPTree` of the MCFClass submodule computes the bound of its check of a
  negative cycle (the sum of the n - 1 most negative costs) again only after
  a change of the costs or of the arcs, rather than at each solve

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

- the tables that map the parameters of `MCFSolver< SPTree >` to those of
  `SPTree` lacked `intMaxThread`, `intEverykIt` and `dblEveryTTm`, so that
  the parameters after them went to the wrong `SPTree` parameter (e.g.,
  `dblAbsAcc` to none and `kReopt` read past the end of the table)

- `MCFSolver<MCFSimplex>` re-optimizing balanced the flow of the whole tree
  at each change of a single capacity or deficit and at each closed arc,
  rather than once before the next solve: after closing 1% of the arcs of
  an instance with 2^12 nodes, re-optimizing took about 8 times the
  instructions of a solve from scratch, and it now takes about 0.6 times

- `MCFSolver<MCFSimplex>` with the Dual Simplex re-optimizing after a change
  of the costs put the arcs out of the tree at the bound that their reduced
  cost under the potentials of before the change says, the potentials not
  being recomputed; and switching from the Primal to the Dual Simplex (or
  back) with an instance loaded deleted the modified balances that both use

- `MCFSimplex::ChgDfcts()` (MCFClass) bounded the range of the nodes by the
  number of arcs, and hence with the default range it wrote past the vector
  of the nodes (a caller of `MCFSolver` is not affected, since it always
  passes the range)

- `MCFSolver<*>::int_par_str2idx()` looked a name that is not a parameter of
  the :MCFClass up among the double parameters of the `CDASolver`, so that
  an int parameter such as `intMaxIter` given by name in a `ComputeConfig`
  was not found

- `MCFSolver<MCFCplex>` ignored `kReopt`, the network simplex of CPLEX
  starting from the basis of the last solve in any case: with `kReopt` off
  it now starts from the initial basis

- `MCFSolver<SPTree>` with a label-setting queue (Dijkstra or the heap, the
  default) stopped at the destinations also when there are negative costs,
  whose labels are not final then, giving wrong ones and missing negative
  cycles; with negative costs it now goes on until the queue is empty, as a
  label-correcting algorithm does, and stops as before otherwise

- `MCFSolver<SPTree>` found a directed cycle of negative cost only if the
  queue of SPTree is a FIFO one: a node scanned more than n times is the
  hint of a cycle, but with any other queue the predecessors need not close
  one then; a label below the sum of the n - 1 most negative arc costs, the
  cost of the most negative simple path, now proves one whatever the queue,
  the count of the scans staying as the early hint

- `MCFSolver<MCFSimplex>` with `kReopt` could give a flow outside the
  bounds after a change of the capacities or of the deficits: the primal
  simplex of MCFClass, balancing the tree before re-optimizing, followed the
  thread while moving subtrees under its dummy root, and skipped the nodes
  the move took away; MCFClass now collects the sons of a node first, and on
  5 instances times 5 seeds of 40 rounds of changes the runs that failed go
  from up to 11 in 25 to none

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
