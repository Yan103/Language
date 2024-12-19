/*!
    \file
    File with declararion of SPU methods
*/

#ifndef SPUMETHODS_H
#define SPUMETHODS_H

#include <stdio.h>

#include "Stack.h"
#include "Tools.h"

/// @brief Structure with SPU
struct SPU {
    int ip = 0;
    Stack* st = NULL;
    Stack* stFunc = NULL;
    int cmds[CMDS_SIZE] = {};
    int regs[REGS_SIZE] = {};
    int ram[RAM_SIZE] = {};
};

/*!
    @brief Function that initialize and create SPU struct
    \param [out] spu - the pointer on SPU struct
*/
SPU* SpuInit();

/*!
    @brief Function that determinate SPU structer
    \param [out] spu - the pointer on SPU struct
*/
void SpuDtor(SPU* spu);

#endif // SPUMETHODS_H