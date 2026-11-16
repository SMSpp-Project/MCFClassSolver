# --------------------------------------------------------------------------- #
#    CMake file for MCFClassSolver/test                                       #
#                                                                             #
#    In this file we define the unit tests for this library.                  #
#    Tests that should be supported by ctest executable must be added         #
#    with the add_test() command.                                             #
#                                                                             #
#                                Donato Meoli                                 #
#                         Dipartimento di Informatica                         #
#                             Universita' di Pisa                             #
# --------------------------------------------------------------------------- #

# The classes of the modules register themselves in the factories from
# static objects, which nothing here names: a shared library is kept by
# what its module asks of the linker, while an archive has to be taken
# whole [see SMSppModule.cmake of SMS++].
smspp_keep_modules()

# ----- MCFClassSolver_test and MCFClassSolver_tolerance_test --------------- #
add_executable(${modName}_test test.cpp)
add_executable(${modName}_tolerance_test test_tolerance.cpp)

foreach (_exe ${modName}_test ${modName}_tolerance_test)
    target_link_libraries(${_exe} PRIVATE SMS++::MCFClassSolver)

    smspp_copy_runtime(${_exe})
endforeach ()

# the netCDF instances dmx2nc4 writes out of those of data/dmx, when the tests
# that read them are run [see the CMakeLists.txt of MCFBlock]; the lock is that
# of the data of the module
add_test(NAME MCFClassSolver_run_dmx2nc4
         COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                 --target run_dmx2nc4)
set_tests_properties(MCFClassSolver_run_dmx2nc4 PROPERTIES
                     FIXTURES_SETUP dmx2nc4_generated
                     RESOURCE_LOCK mcf_data)

# Add a test for each batch file
file(GLOB BATCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/batch"
                      "${CMAKE_CURRENT_SOURCE_DIR}/batch-l")
foreach (BATCH_FILE ${BATCH_FILES})
    # Remove the source directory prefix to get the batch name
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" BATCH_NAME ${BATCH_FILE})
    add_test(NAME ${modName}_test/${BATCH_NAME}
             COMMAND ${SMSpp_BASH}
             "${BATCH_FILE}" "$<TARGET_FILE:${modName}_test>"
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    # batch-l reads the larger instances produced by dmx2nc4
    if (BATCH_NAME STREQUAL "batch-l")
        set_tests_properties(${modName}_test/${BATCH_NAME}
                             PROPERTIES FIXTURES_REQUIRED dmx2nc4_generated)
    endif ()
endforeach ()

# The infeasible instances with fractional costs of data/infeas-frac-*.dmx,
# on which every MCFSolver of MCFBSCfg-tol.txt must say infeasible, and
# MCFSimplex again once MCFBSCfg-tol-dual.txt turns it to the dual simplex
foreach (_inst infeas-frac-3 infeas-frac-4)
    add_test(NAME ${modName}_tolerance_test/${_inst}
             COMMAND ${modName}_tolerance_test data/${_inst}.dmx infeasible
                     MCFBSCfg-tol.txt MCFBSCfg-tol-dual.txt
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach ()

# ----- the label of this module's tests ------------------------------------ #

# The label lets the module CI select exactly the tests exercising this module
# with `ctest -L <module>` (see tests/cmake/TestLabels.cmake in the umbrella
# for the system-tests counterpart): without it these tests are built and
# never run.
get_property(_tests DIRECTORY PROPERTY TESTS)
if (_tests)
    set_tests_properties(${_tests} PROPERTIES LABELS "${modName}")
endif ()

# --------------------------------------------------------------------------- #
