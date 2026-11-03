/*--------------------------------------------------------------------------*/
/*----------------------- File test_tolerance.cpp --------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Edge cases of the tolerances of the :MCFClass under MCFSolver.
 *
 * Reads a MCF instance in DIMACS format into a MCFBlock, then applies to it
 * each of the BlockSolverConfig given in turn, and after each of them
 * computes every Solver registered to the MCFBlock and checks that it
 * returns the status given (ok or infeasible); so, the first one registers
 * the Solver, the following ones, differential, change their parameters
 * once the instance is loaded. The instances are those on which a
 * :MCFClass may take the rounding of the reduced costs for a violation and
 * pivot for ever, e.g., an infeasible one with fractional costs, on which
 * the potentials of MCFSimplex stay of the order of the cost of its
 * artificial arcs; the BlockSolverConfig give a limit on the iterations, so
 * that such a Solver stops and the test fails rather than hanging.
 *
 * \author Antonio Frangioni \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \author Donato Meoli \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \copyright &copy; by Antonio Frangioni, Donato Meoli
 */
/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "BlockSolverConfig.h"
#include "MCFBlock.h"
#include "MCFSolver.h"

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace SMSpp_di_unipi_it;

/*--------------------------------------------------------------------------*/
/*--------------------------------- MAIN -----------------------------------*/
/*--------------------------------------------------------------------------*/

int main( int argc , char ** argv )
{
 if( argc < 4 ) {
  std::cerr << "Usage: " << argv[ 0 ]
            << " <dmx file> ok|infeasible <BlockSolverConfig file> ..."
            << std::endl;
  return( 1 );
  }

 const std::string expect( argv[ 2 ] );
 int status;
 if( expect == "ok" )
  status = Solver::kOK;
 else
  if( expect == "infeasible" )
   status = Solver::kInfeasible;
  else {
   std::cerr << "unknown status " << expect << std::endl;
   return( 1 );
   }

 std::unique_ptr< Block > B( Block::new_Block( "MCFBlock" ) );
 std::ifstream f( argv[ 1 ] );
 if( ( ! B ) || ( ! f.is_open() ) ) {
  std::cerr << "cannot read " << argv[ 1 ] << std::endl;
  return( 1 );
  }
 B->load( f );

 std::vector< std::unique_ptr< BlockSolverConfig > > BSC;
 bool passed = true;
 for( int i = 3 ; i < argc ; ++i ) {
  BSC.emplace_back( dynamic_cast< BlockSolverConfig * >(
                              Configuration::deserialize( argv[ i ] ) ) );
  if( ! BSC.back() ) {
   std::cerr << "cannot read " << argv[ i ] << std::endl;
   return( 1 );
   }
  BSC.back()->apply( B.get() );

  if( B->get_registered_solvers().empty() )
   passed = false;
  for( auto s : B->get_registered_solvers() ) {
   int rtrn = s->compute();
   std::cout << "[" << argv[ 1 ] << " " << argv[ i ] << " "
             << s->classname() << "]: " << rtrn;
   if( rtrn == status )
    std::cout << " OK" << std::endl;
   else {
    std::cout << " KO, expected " << status << std::endl;
    passed = false;
    }
   }
  }

 for( auto & bsc : BSC ) {  // unregister and delete the Solver
  bsc->clear();
  bsc->apply( B.get() );
  }

 if( passed )
  std::cout << "\x1B[32mAll tests passed!!\033[0m" << std::endl;
 else
  std::cout << "\x1B[31mSome test failed!!\033[0m" << std::endl;

 return( passed ? 0 : 1 );
 }

/*--------------------------------------------------------------------------*/
/*---------------------- End File test_tolerance.cpp -----------------------*/
/*--------------------------------------------------------------------------*/
