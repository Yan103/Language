/*!
    \file
    File with declaration of processor function
*/

#include <stdio.h>

#include "SpuMethods.h"

/// @brief Constant for the input filename
static const char* INPUT_FILENAME  = "/home/yan/projects/processor/tests/assembler_code.txt";

/*!
    @brief Function that get arguments from the command line
    \param [in] argc - argument count
    \param [in] argv - argument values
*/
void GetCommandsArgs(int argc,  char* argv[]);

/*!
    @brief Function that launched processor
    \param [in] spu - the pointer on SPU struct
*/
void CPUWork(SPU* spu);

/*!
    @brief Function that get argument for the processor commands
    \param [in] spu- the pointer on the spu struct
*/
int* GetArg (SPU* spu);