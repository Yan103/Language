/*!
    \file
    File with SPU methods
*/

#include <stdio.h>
#include <stdlib.h>

#include "Tools.h"
#include "ReturnCodes.h"
#include "StackMethods.h"
#include "Stack.h"
#include "Config.h"
#include "SpuMethods.h"

/*!
    @brief Function that initialize and create SPU struct
    \param [out] spu - the pointer on SPU struct
*/
SPU* SpuInit() {
    SPU* spu = (SPU*)calloc(1, sizeof(SPU));
    ASSERT(spu != NULL, "MEMORY ERROR!\n");

    spu->st = STACK_INIT(spu->st);
    spu->stFunc = STACK_INIT(spu->stFunc);

    return spu;
}

/*!
    @brief Function that determinate SPU structer
    \param [out] spu - the pointer on SPU struct
*/
void SpuDtor(SPU* spu) {
    ASSERT(spu != NULL, "NULL POINTER!\n");

    StackDtor(spu->st);
    StackDtor(spu->stFunc);

    FREE(spu)
}