/*--------------------------------------------------------------------------*/
/*------------------------ File MCFClassSolver.cppm ------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Interface of the C++20 module smspp.mcfclasssolver, which exports every
 * name that the headers of MCFClassSolver declare at namespace scope,
 * together with the modules it depends on; a translation unit can then write
 *
 *     import smspp.mcfclasssolver;
 *
 * in place of the #include of the headers. The macros (e.g., those of the
 * factory in SMSFactory.h) are not exported by a module, so a file that uses
 * them still includes the header that defines them.
 *
 * This file is written by SMS++/cmake/smspp_module_exports.py, which reads
 * the names from the AST of the headers: it is not to be edited by hand.
 *
 * \author Antonio Frangioni \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \copyright &copy; by Antonio Frangioni
 */
/*--------------------------------------------------------------------------*/
/*------------------------- GLOBAL MODULE FRAGMENT -------------------------*/
/*--------------------------------------------------------------------------*/

module;

#include "MCFSolver.h"

/*--------------------------------------------------------------------------*/
/*----------------------------- MODULE PURVIEW -----------------------------*/
/*--------------------------------------------------------------------------*/

export module smspp.mcfclasssolver;

export import smspp.mcfblock;

export namespace SMSpp_di_unipi_it {

 // MCFSolver.h
 using SMSpp_di_unipi_it::MCFSolver;
 using SMSpp_di_unipi_it::MCFSolverState;
}

/*--------------------------------------------------------------------------*/
/*---------------------- End File MCFClassSolver.cppm ----------------------*/
/*--------------------------------------------------------------------------*/
