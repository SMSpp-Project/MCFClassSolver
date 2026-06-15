# MCFClassSolver

This project implements `MCFSolver`, a SMS++ `:Solver` for
[MCFBlock](https://gitlab.com/smspp/mcfblock) based on forwarding the interface
of (objects derived from the general abstract) [MCFClass of the MCFClass
project](http://www.di.unipi.it/optimize/Software/MCF.html). `MCFSolver` is
template over the underlying `:MCFClass` object, so the same source produces a
solver for any of the min-cost flow algorithms provided by MCFClass, such as
`MCFSimplex`, `RelaxIV` and `MCFCplex`.

Upon compiling, one `MCFSolver< :MCFClass >` variant is added to the `Solver`
factory for each `:MCFClass` selected through the corresponding `HAVE_*` macro
(see [MCFSolver.h](include/MCFSolver.h) for the full list); the selection is
made in [CMakeLists.txt](CMakeLists.txt) for the CMake build and in the
`MCFClssSlvr` macro of the [makefile](makefile) for the makefile build. The
chosen `:MCFClass` must of course have been compiled into the MCFClass library.


## Getting started

These instructions will let you build `MCFClassSolver` on your system.


### Requirements

- The [SMS++ core library](https://gitlab.com/smspp/smspp) and its
  requirements.

- The [MCFBlock](https://gitlab.com/smspp/mcfblock) module.

- [MCFClass](https://github.com/frangio68/Min-Cost-Flow-Class) and its
  requirements (depending on the actual `:MCFClass` solvers built). It is
  provided here as a submodule.


### Build and install with CMake

Configure and build the library with:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

The library has the same configuration options of
[SMS++](https://gitlab.com/smspp/smspp-project/-/wikis/Customize-the-configuration).

Optionally, install the library in the system with:

```sh
cmake --install .
```


### Usage with CMake

After the library is built, you can use it in your CMake project with:

```cmake
find_package(MCFClassSolver)
target_link_libraries(<my_target> SMS++::MCFClassSolver)
```


### Running the tests with CMake

A unit test will be built with the library. To disable it, set the option
`BUILD_TESTING` to `OFF`.

The test takes an instance of a MCF in DIMACS or NC4 format. The MCF problem
is then repeatedly solved with several changes in costs/capacities/deficits,
arcs openings/closures and arcs additions/deletions. The same operations are
performed on the two solvers, and the results are compared.


### Build and install with makefiles

Carefully hand-crafted makefiles have also been developed for those unwilling
to use CMake. Makefiles build the executable in-source (in the same directory
tree where the code is) as opposed to out-of-source (in the copy of the
directory tree constructed in the build/ folder) and therefore it is more
convenient when having to recompile often, such as when developing/debugging
a new module, as opposed to the compile-and-forget usage envisioned by CMake.

Each executable using `MCFClassSolver` has to include a "main makefile" of the
module, which typically is either [makefile-c](makefile-c) including all
necessary libraries comprised the "core SMS++" one, or
[makefile-s](makefile-s) including all necessary libraries but not the "core
SMS++" one (for the common case in which this is used together with other
modules that already include them). One relevant case is the
[tester comparing MCFBlock + MCFSolver with direct usage of the
original :MCFClass solver](test/test.cpp) alluded to in the previous section.
The makefiles in turn recursively include all the required other makefiles,
hence one should only need to edit the "main makefile" for compilation type
(C++ compiler and its options) and it all should be good to go. In case some
of the external libraries are not at their default location, it should only be
necessary to create the `../extlib/makefile-paths` out of the
`extlib/makefile-default-paths-*` for your OS `*` and edit the relevant bits
(commenting out all the rest).

Check the [SMS++ installation wiki](https://gitlab.com/smspp/smspp-project/-/wikis/Customize-the-configuration#location-of-required-libraries)
for further details.

Note that the [MCFClass
project](https://github.com/frangio68/Min-Cost-Flow-Class) has a similar
arrangement with its own extlib/ folder, but due to some magic it is not
necessary to independently edit it in an analogous way.


## Tests

The [test](test) folder contains a tester that reads an instance of a MCF
from a file (in either DIMACS or netCDF format) in an `MCFBlock`, and from
there in an object of a class MCFC derived from `MCFClass`, as decided by
the macro `WHICH_MCF`. Then, a `MCFSolver< MCFC >` is attached to the
`MCFBlock`. The MCF problem is then repeatedly solved with several changes in
costs / capacities / deficits, arcs openings / closures and arcs additions /
deletions. The same operations are performed on the two solvers, and the
results are compared. This mostly tests `MCFBlock` and `MCFSolver`, since
the actual `MCFClass` solved is the same, and so it can easily be wrong in
the same way for both the objects. The `batch` file tests basically only one
instance but in many different configurations (there can actually be two
`MCFBlock`, one of which is modified and the other solved, in all possible
combinations) and repeatedly, while the `batch-l` tests only the simplest
case but on several different problems taken from the `data` folder of the
[MCFBlock](https://gitlab.com/smspp/mcfblock) module, generated there by its
`dmx2nc4` tool.


## Getting help

If you need support, you want to submit bugs or propose a new feature, you can
[open a new issue](https://gitlab.com/smspp/mcfclasssolver/-/issues/new).


## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of
conduct, and the process for submitting merge requests to us.


## Authors

### Current Lead Authors

- **Antonio Frangioni**  
  Dipartimento di Informatica  
  Università di Pisa


## License

This code is provided free of charge under the [GNU Lesser General Public
License version 3.0](https://opensource.org/licenses/lgpl-3.0.html) -
see the [LICENSE](LICENSE) file for details.


## Disclaimer

The code is currently provided free of charge under an open-source license.
As such, it is provided "*as is*", without any explicit or implicit warranty
that it will properly behave or it will suit your needs. The Authors of
the code cannot be considered liable, either directly or indirectly, for
any damage or loss that anybody could suffer for having used it. More
details about the non-warranty attached to this code are available in the
license description file.
