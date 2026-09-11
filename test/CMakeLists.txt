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

# Since we are using the block factory, objects from linked libraries
# may wrongly appear unused, and by default the linker does not include them,
# so we have to force the linking.
if (BUILD_SHARED_LIBS)
    if (UNIX AND (NOT APPLE))
        add_link_options("-Wl,--no-as-needed")
    endif ()
else ()
    if (MSVC)
        add_link_options("/WHOLEARCHIVE" "/FORCE:MULTIPLE")
    else () # Unix
        if (APPLE)
            add_link_options("-Wl,-all_load")
        else ()
            add_link_options("-Wl,--whole-archive,--allow-multiple-definition")
        endif ()
    endif ()
endif ()

# ----- MCFClassSolver_test ------------------------------------------------- #
add_executable(${modName}_test test.cpp)
target_link_libraries(${modName}_test PRIVATE SMS++::MCFClassSolver)

if (WIN32 AND BUILD_SHARED_LIBS AND NOT DEFINED ENV{CONDA_BUILD})
    # On Windows with shared libraries enabled copies all runtime DLL
    # dependencies next to the executable in the build directory
    # (POST_BUILD), using CMake's TARGET_RUNTIME_DLLS generator expression
    add_custom_command(TARGET ${modName}_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${modName}_test>
            $<TARGET_FILE_DIR:${modName}_test>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying runtime DLLs next to ${modName}_test.exe")
endif ()

# The batch-l tester runs over the larger instances produced by the dmx2nc4
# tool of the MCFBlock module; build that target as a setup fixture so the
# instances exist before the test runs.
add_test(NAME MCFClassSolver_run_dmx2nc4
         COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target run_dmx2nc4)
set_tests_properties(MCFClassSolver_run_dmx2nc4
                     PROPERTIES FIXTURES_SETUP mcfcs_dmx2nc4_generated)

# Add a test for each batch file
file(GLOB BATCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/batch"
                      "${CMAKE_CURRENT_SOURCE_DIR}/batch-l")
foreach (BATCH_FILE ${BATCH_FILES})
    # Remove the source directory prefix to get the batch name
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" BATCH_NAME ${BATCH_FILE})
    if (UNIX)
        add_test(NAME ${modName}_test/${BATCH_NAME}
                 COMMAND bash ${BATCH_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${modName}_test
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    elseif (WIN32)
        add_test(NAME ${modName}_test/${BATCH_NAME}
                 # using Git for Windows that includes Git Bash
                 COMMAND "C:/Program Files/Git/bin/bash.exe"
                 "${BATCH_FILE}" "$<TARGET_FILE:${modName}_test>"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif ()
    # batch-l reads the larger instances produced by dmx2nc4
    if (BATCH_NAME STREQUAL "batch-l")
        set_tests_properties(${modName}_test/${BATCH_NAME}
                             PROPERTIES FIXTURES_REQUIRED mcfcs_dmx2nc4_generated)
    endif ()
endforeach ()

# --------------------------------------------------------------------------- #
