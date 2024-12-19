/*!
    \file
    File with return codes, enums and structers
*/

#ifndef RETURNCODES_H
#define RETURNCODES_H

#include <stdio.h>

#include "Tools.h"
#include "Stack.h"

/// @brief Constant for the maximum label's name
static const size_t LABEL_NAME_LEN = 50;

/// @brief Constant for the labels count
static const size_t LABELS_COUNT   = 50;

/// @brief Structure with Labels
struct Label {
    char label_name[LABEL_NAME_LEN];
    int  label_number = -1;
};

/// @brief Enum with return functions codes
enum FuncReturn {
    SUCCESS        = 0b0,        ///< SUCCESS
    BAD_PTR        = 0b1,        ///< BAD POINTER
    BAD_SIZE       = 0b10,       ///< BAD STACK SIZE
    BAD_FILE       = 0b100,      ///< BAD FILENAME
    EMPTY_STACK    = 0b1000,     ///< EMPTY STACK
    STACK_OVERFLOW = 0b10000,    ///< STACK OVERFLOW
    KANARY_DAMAGED = 0b100000,   ///< KANARY DAMAGED
    HASH_ERROR     = 0b1000000,  ///< HASH ERROR
    MEMORY_ERROR   = 0b10000000, ///< MEMORY ERROR
    ERROR          = -1,
};

/// @brief Enum with processor commands
enum Comands {
    PUSH  =  1,
    POP   =  2,
    LABEL =  3,
    ADD   =  4,
    SUB   =  5,
    DIV   =  6,
    MUL   =  7,
    IN    =  8,
    OUT   =  9,
    JNE   = 10,
    JMP   = 11,
    JZ    = 12,
    JGZ   = 13,
    JLZ   = 14,
    JE    = 15,
    CALL  = 16,
    RET   = 17,
    SQRT  = 18,
    SQR   = 19,
    MOD   = 20,
    IDIV  = 21,
    JGE   = 22,
    DRAW  = 23,
    HLT   = -1,
};

/// @brief Enum with registers
enum Reg {
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    EX = 4,
    FX = 5,
    GX = 6,
    HX = 7,
    IX = 8,
    XX = 9,
};

#endif // RETURNCODES_H