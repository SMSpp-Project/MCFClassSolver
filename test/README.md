# test

A tester for `MCFSolver`.

It reads a Min-Cost Flow instance (in either DIMACS or netCDF format) into an
`MCFBlock` and, from there, into an object of a `:MCFClass` chosen at compile
time, then attaches an `MCFSolver< :MCFClass >` to the `MCFBlock`. The problem
is repeatedly solved while changing costs, capacities and deficits, opening
and closing arcs, and adding and deleting arcs; the same operations are
applied to both objects and the results are compared. This mostly exercises
`MCFBlock` and `MCFSolver`, since the underlying `:MCFClass` is the same in
the two paths.

Small instances live in the `data` folder, used by `batch`; the larger ones
used by `batch-l` are taken from the `data` folder of the `MCFBlock` module
(generated there by its `dmx2nc4` converter). Each `batch` file runs the tester
over a set of them; all of them passing is a good sign that no regressions have
been introduced. The `makefile` builds the executable including the
`MCFClassSolver` and `MCFBlock` modules and the core SMS++ library.

`MCFClassSolver_tolerance_test` (`test_tolerance.cpp`) reads a DIMACS
instance, registers to it the `MCFSolver` of a `BlockSolverConfig` and checks
that each of them returns the expected status. It is run on the infeasible
instances with fractional costs `data/infeas-frac-*.dmx`, on which a solver
that takes the rounding of the reduced costs for a violation pivots for ever;
`MCFBSCfg-tol.txt` limits the iterations, so that it stops and the test fails.


## Authors

- **Antonio Frangioni**  
  Dipartimento di Informatica  
  Università di Pisa


## License

This code is provided free of charge under the [GNU Lesser General Public
License version 3.0](https://opensource.org/licenses/lgpl-3.0.html),
see the [LICENSE](../LICENSE) file for details.
