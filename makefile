##############################################################################
################################ makefile ####################################
##############################################################################
#                                                                            #
#   makefile of MCFClassSolver                                               #
#                                                                            #
#   The makefile takes in input the -I directives for all the external       #
#   libraries needed by MCFClassSolver, i.e., core SMS++, MCFBlock and       #
#   MCFClass. These are *not* copied into $(MCFCSINC): adding those -I       #
#   directives to the compile commands will have to done by whatever "main"  #
#   makefile is using this. Analogously, any external library and the        #
#   corresponding -L< libdirs > will have to be added to the final linking   #
#   command by whatever "main" makefile is using this.                       #
#                                                                            #
#   Note that, conversely, $(SMS++INC) is also assumed to include any        #
#   -I directive corresponding to external libraries needed by SMS++, at     #
#   least to the extent in which they are needed by the parts of SMS++       #
#   used by MCFClassSolver.                                                  #
#                                                                            #
#   The makefile defines internally (cf. MCFClssSlvr below) which            #
#   MCFSolver< :MCFClass > will be available.                                #
#                                                                            #
#   Input:  $(CC)          = compiler command                                #
#           $(SW)          = compiler options                                #
#           $(SMS++INC)    = the -I$( core SMS++ directory )                 #
#           $(SMS++OBJ)    = the core SMS++ library                          #
#           $(MCFBkINC)    = the -I$( MCFBlock directory )                   #
#           $(MCFBkOBJ)    = the MCFBlock library                            #
#           $(libMCFClINC) = the -I$( MCFClass library )                     #
#           $(libMCFClOBJ) = the MCFClass library                            #
#           $(MCFCSSDR)    = the directory where the source is               #
#                                                                            #
#   Output: $(MCFCSOBJ)    = the final object(s) / library                   #
#           $(MCFCSH)      = the .h files to include                         #
#           $(MCFCSINC)    = the -I$( source directory )                     #
#                                                                            #
#                              Antonio Frangioni                             #
#                         Dipartimento di Informatica                        #
#                             Universita' di Pisa                            #
#                                                                            #
##############################################################################

# define the set of MCFSolver< :MCFClass > that will be available by
# uncommenting the -DHAVE_* below corresponding to the :MCFClass; see
# MCFSolver.h for details. note that, obviously, the :MCFClass selected
# here must have been compiled in the MCFClass library
MCFClssSlvr = -DHAVE_MFSMX -DHAVE_CPLEX -DHAVE_RELAX
# -DHAVE_CSCL2 -DHAVE_MFZIB -DHAVE_SPTRE

# macros to be exported - - - - - - - - - - - - - - - - - - - - - - - - - - -

MCFCSOBJ = $(MCFCSSDR)/obj/MCFSolver.o

MCFCSINC = -I$(MCFCSSDR)/include

MCFCSH   = $(MCFCSSDR)/include/MCFSolver.h

# clean - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean::
	rm -f $(MCFCSOBJ) $(MCFCSSDR)/*~

# dependencies: every .o from its .cpp + every recursively included .h- - - -

$(MCFCSSDR)/obj/MCFSolver.o: $(MCFCSSDR)/src/MCFSolver.cpp $(MCFCSH) \
	$(MCFBkH) $(SMS++OBJ) $(libMCFClOBJ)
	$(CC) -c $(MCFCSSDR)/src/MCFSolver.cpp -o $@ \
	$(MCFCSINC) $(MCFBkINC) $(SMS++INC) $(libMCFClINC) $(MCFClssSlvr) $(SW)

########################## End of makefile ###################################
