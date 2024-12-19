/*!
    \file
    File with the declaration of assembler's functions
*/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "Tools.h"
#include "ReturnCodes.h"

/// @brief Constant for the input filename
static const char* INPUT_FILENAME  = "/home/yan/projects/processor/tests/program.asm";

/// @brief Constant for thу output filename
static const char* OUTPUT_FILENAME = "/home/yan/projects/processor/tests/assembler_code.txt";

/*!
    @brief Function that get register value from argument of command
    \param [in] cmd - command
*/
Reg GetRegValue(char* cmd);

/*!
    @brief Function that skip comments in assembler code
    \param [out] curr_line - current line
*/
FuncReturn SkipAsmComments(char* curr_line);

/*!
    @brief Function that get arguments from the command line
    \param [in] argc - argument count
    \param [in] argv - argument values
*/
FuncReturn GetCommandsArgs(int argc,  char* argv[]);

/*!
    @brief Function that fill the argument type with bits if necessary
    \param [in]  arg      - command argument
    \param [out] arg_type - type of argument
*/
FuncReturn FillArgType(char* arg, int* arg_type);

/*!
    @brief Function that that generates arguments for Push and Pop
    \param [in]  cmds - array with commands
    \param [out] ip   - instruction pointer
    \param [in]  arg  - argument
*/
FuncReturn PushPopFill(int* cmds, int* ip, char* arg);

/*!
    @brief Function that search a constant in command argument
    \param [out] str     - argument
    \param [in]  str_len - length of the argument
*/
char* SearchConstPtr(char* str, size_t str_len);

/*!
    @brief Function that formats the text command to code type
    \param [in]  cmd          - command
    \param [out] machine_code - future formatted machine code
*/
void FromTextToCode(char* cmd, int* machine_code);

/*!
    @brief Assembler function
    \param [in] input_file  - input filename
    \param [in] output_file - output filename
*/
void Assembler(FILE* input_file, FILE* output_file);

/*!
    @brief Function that formats the received argument
    \param [in] push_arg_unformated - unformatted argument
    \param [in] push_arg            - future formatted argument
*/
FuncReturn FormateArg(char* push_arg_unformated, char* push_arg);

#endif // ASSEMBLER_H