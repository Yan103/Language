/*!
    \file
    File with processor functions
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>

#include "Tools.h"
#include "ReturnCodes.h"
#include "StackMethods.h"
#include "Stack.h"
#include "Config.h"
#include "SpuMethods.h"
#include "processor.h"

/// @brief Constant for the register bit
const int REG_BIT      = 1;

/// @brief Constant for the memory bit
const int MEMORY_BIT   = 4;

/// @brief Constant for the constant bit
const int CONSTANT_BIT = 2;

/// @brief Constant for calculation accuracy
const int CALC_ACCURACY = 2;

/*!
    @brief Function that get arguments from the command line
    \param [in] argc - argument count
    \param [in] argv - argument values
*/
void GetCommandsArgs(int argc,  char* argv[]) {
    assert(argv != NULL);

    for (int i = 1; i < argc; i++) {
        if (strcasecmp(argv[i], "-f") == 0) {
            if (i != argc - 1)
                INPUT_FILENAME = argv[i + 1];
        }
    }
}

/*!
    @brief Function that get argument for the processor commands
    \param [in] spu - the pointer on the spu struct
*/
int* GetArg (SPU* spu) {
    assert(spu != NULL);

    int arg_type  = (spu->cmds)[++(spu->ip)];
    int* arg_value = NULL;

    if (arg_type & REG_BIT) {
        arg_value = &(spu->regs[ (spu->cmds)[++(spu->ip)] ]);
    }

    if (arg_type & CONSTANT_BIT) {
        if (arg_value != NULL) {
            spu->regs[XX] = *arg_value + (spu->cmds)[++(spu->ip)];
        } else {
            spu->regs[XX] = (spu->cmds)[++(spu->ip)];
        }
        arg_value = &spu->regs[XX];
    }

    if (arg_type & MEMORY_BIT) {
        arg_value = &spu->ram[*arg_value];
    }
    return arg_value;
}

/*!
    @brief Function that launched processor
    \param [in] spu - the pointer on SPU struct
*/
void CPUWork(SPU* spu) {
    assert(spu != NULL);

    int DoFlag = 1;

    while (DoFlag) {
        switch(spu->cmds[ spu->ip ]) {
            case PUSH: {
                int* push_elem_ptr = GetArg(spu);
                StackPush(spu->st, *push_elem_ptr * CALC_ACCURACY);
                (spu->ip)++;
                break;
            }

            case POP: {
                int* pop_ptr = GetArg(spu);
                *pop_ptr = StackPop(spu->st) / CALC_ACCURACY;
                (spu->ip)++;
                break;
            }

            case ADD: {
                StackPush(spu->st, StackPop(spu->st) + StackPop(spu->st));
                (spu->ip)++;
                break;
            }

            case SUB: {
                int a = StackPop(spu->st);
                StackPush(spu->st, StackPop(spu->st) - a);
                (spu->ip)++;
                break;
            }

            case DIV: {
                int a = StackPop(spu->st);
                if (a != 0)
                    StackPush(spu->st, CALC_ACCURACY * StackPop(spu->st) / a);
                else {
                    printf(RED("ERROR! DIVISION BY ZERO!!!\n"));
                    DoFlag = 0;
                }
                (spu->ip)++;
                break;
            }

            case MUL: {
                StackPush(spu->st, StackPop(spu->st) * StackPop(spu->st) / CALC_ACCURACY);
                (spu->ip)++;
                break;
            }

            case IN: {
                int a = 0;
                printf(YELLOW("Enter a number: ")); scanf("%d", &a);
                StackPush(spu->st, a * CALC_ACCURACY);
                (spu->ip)++;
                break;
            }

            case OUT: {
                printf(GREEN("RESULT: %lg\n"), (double) StackPop(spu->st) / CALC_ACCURACY);
                (spu->ip)++;
                break;
            }

            case HLT: {
                DoFlag = 0;
                break;
            }

            case JMP: {
                int next_ip = spu->cmds[ ++(spu->ip) ];
                spu->ip = next_ip;
                break;
            }

            case CALL: {
                StackPush(spu->stFunc, spu->ip + 2);
                int next_ip = spu->cmds[ ++(spu->ip) ];
                spu->ip = next_ip;
                break;
            }

            case RET: {
                int next_ip = StackPop(spu->stFunc);
                spu->ip = next_ip;
                break;
            }

            case SQRT: {
                int a = StackPop(spu->st) / CALC_ACCURACY;
                if (a >= 0) {
                    StackPush(spu->st, (int)sqrt(a) * CALC_ACCURACY);
                } else {
                    printf(RED("SQRT ERROR!\n"));
                    DoFlag = 0;
                }
                (spu->ip)++;
                break;
            }

            case SQR: {
                int a = StackPop(spu->st) / CALC_ACCURACY;
                StackPush(spu->st, a * a * CALC_ACCURACY);
                (spu->ip)++;
                break;
            }

            case MOD: {
                int a = StackPop(spu->st) / CALC_ACCURACY;
                int b = StackPop(spu->st) / CALC_ACCURACY;

                if (a != 0) {
                    StackPush(spu->st, (b % a) * CALC_ACCURACY);
                } else {
                    printf(RED("ZERO DIVISION!\n"));
                    DoFlag = 0;
                }
                (spu->ip)++;
                break;
            }

            case IDIV: {
                int a = StackPop(spu->st) / CALC_ACCURACY;
                int b = StackPop(spu->st) / CALC_ACCURACY;

                if (a != 0) {
                    StackPush(spu->st, (b / a) * CALC_ACCURACY);
                } else {
                    printf(RED("ZERO DIVISION!\n"));
                    DoFlag = 0;
                }
                (spu->ip)++;
                break;
            }

            case JNE: {
                int a = StackPop(spu->st);
                int b = StackPop(spu->st);

                if (a != b) {
                    int next_ip = spu->cmds[ ++(spu->ip) ];
                    spu->ip = next_ip;
                } else spu->ip += 2;

                StackPush(spu->st, b);
                StackPush(spu->st, a);
                break;
            }

            case JGE: {
                int a = StackPop(spu->st);
                int b = StackPop(spu->st);

                if (a >= b) {
                    int next_ip = spu->cmds[ ++(spu->ip) ];
                    spu->ip = next_ip;
                } else spu->ip += 2;

                StackPush(spu->st, b);
                StackPush(spu->st, a);
                break;
            }

            case JLZ: {
                int a = StackPop(spu->st);

                if (a < 0) {
                    int next_ip = spu->cmds[ ++(spu->ip) ];
                    spu->ip = next_ip;
                } else spu->ip += 2;

                StackPush(spu->st, a);
                break;
            }

            case JGZ: {
                int a = StackPop(spu->st);

                if (a > 0) {
                    int next_ip = spu->cmds[ ++(spu->ip) ];
                    spu->ip = next_ip;
                } else spu->ip += 2;

                StackPush(spu->st, a);
                break;
            }

            case DRAW: {
                for (size_t i = 0; i < RAM_SIZE; i++) {
                    if (spu->ram[i] != 0) {
                        printf(GREEN("%c "), spu->ram[i]);
                    } else {
                        printf("· ");
                    }

                    if ((i + 1) % (size_t)sqrt(RAM_SIZE) == 0) {
                        printf("\n");
                    }
                }
                (spu->ip)++;
                break;
            }

            case JE: {
                int a = StackPop(spu->st);
                int b = StackPop(spu->st);

                if (a == b) {
                    int next_ip = spu->cmds[ ++(spu->ip) ];
                    spu->ip = next_ip;
                } else spu->ip += 2;

                StackPush(spu->st, b);
                StackPush(spu->st, a);
                break;
            }

            default: {
                printf("%d %d ", spu->ip, spu->cmds[ spu->ip ]);
                printf(RED("UNKNOWN CODE!!!\n"));
                DoFlag = 0;
                break;
            }
        }
    }
}