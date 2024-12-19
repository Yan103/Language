/*!
    \file
    File with definition of stack functions
*/

#ifndef STACKMETHODS_H
#define STACKMETHODS_H

#include <assert.h>
#include <stdio.h>

#include "ReturnCodes.h"
#include "Stack.h"

/// @brief Constant for poison elements in stack
static const int POISON_NUM = -666;

/// @brief Constant for default stack size
static const int START_SIZE = 64;

/*!
    @brief Function that checks the right and left kanaryees
    \param [in] st - the pointer on Stack structer
*/
void StackKanaryCheck(Stack* st);

/*!
    @brief Function that do stack initialization
    \param [in] st       - the pointer on pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
    \param [in] var_name - the name of variable with stack
*/
Stack* StackInit(const char *file, const char* func, int line, const char* var_name);

/*!
    @brief Function that creates the stack
    \param [in] st       - the pointer on Stack structer
    \param [in] capacity - the max size of stack
*/
FuncReturn StackCtor(Stack* st, size_t capacity);

/*!
    @brief Function that dumping all information about stack
    \param [in] st - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
FuncReturn StackDump(Stack* st, const char *file, const char* func, int line);

/*!
    @brief Function that checks stack state
    \param [in] st - the pointer on Stack structer
*/
int StackOk(Stack* st);

/*!
    @brief Function that get string name of error
    \param [in] error - the enum value of FuncReturn
*/
const char* StackStrErr(FuncReturn error);

/*!
    @brief Function that pushs new value in end of stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the value which will push in last place in stack
*/
FuncReturn StackPush(Stack* st, StackElem_t value);

/*!
    @brief Function that gets last element in stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the pointer on variable kuda will save the pop element
*/
StackElem_t StackPop(Stack* st);

/*!
    @brief Function that destroys the stack
    \param [in] st - the pointer on Stack structer
*/
FuncReturn StackDtor(Stack* st);

/*!
    @brief Function that shows information about stack
    \param [in] filename - the pointer on FILE
    \param [in] st       - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
FuncReturn DumpPrint(FILE* filename, Stack* st, const char *file, const char *func, int line);

/*!
    @brief Function that resizes stack size
    \param [in] st       - the pointer on Stack structer
    \param [in] new_size - the new size of stack
*/
FuncReturn StackResize(Stack* st, size_t new_size);

/*!
    @brief Function that realloc memory and fills it POISON_NUM
    \param [in] st       - the pointer on Stack structer
    \param [in] new_size - the new size of stack
*/
FuncReturn ReCalloc(Stack* st, size_t new_size);

/*!
    @brief Function that try find element in stack (return element's index if he in stack, else capacity)
    \param [in] st         - the pointer on Stack structer
    \param [in] find_value - the find value
*/
size_t StackFind(Stack* st, StackElem_t find_value);

/*!
    @brief Function that cleans the stack
    \param [in] st - the pointer on Stack structer
*/
FuncReturn StackClean(Stack* st);

/*!
    @brief Function that shows information about stack errors
    \param [in] filename - the pointer on FILE
    \param [in] st       - the pointer on Stack structer
    \param [in] file     - file in which was called function
    \param [in] func     - function in which was called function
    \param [in] line     - line in which was called function
*/
FuncReturn DumpErrorPrint(FILE* filename, Stack* st, const char *file, const char *func, int line);

/*!
    @brief Function that calculates first hash for stack
    \param [in] st - the pointer on Stack structer
*/
size_t StackHashFunc1(Stack* st);

/*!
    @brief Function that calculates second hash for stack
    \param [in] st - the pointer on Stack structer
*/
size_t StackHashFunc2(Stack* st);

/*!
    @brief Function that checks the stack hashes
    \param [in] st - the pointer on Stack structer
*/
void StackHashCheck(Stack* st);

/*!
    @brief Function that updates stack hashes
    \param [in] st - the pointer on Stack structer
*/
FuncReturn StackUpdateHash(Stack* st);

/*!
    @brief Function that push value in the end of stack
    \param [in] st    - the pointer on Stack structer
    \param [in] value - the value which will push in last place in stack
*/
void StackPushValue(Stack* st, StackElem_t* value);

#endif // STACKMETHODS_H