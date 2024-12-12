#ifndef OPERATIONS_H
#define OPERATIONS_H

enum NodeDataType {
    NUM   = 1,
    VAR   = 2,
    UN_OP = 3,
    BI_OP = 4,
};

enum NodeOperatorCodes {
    ADD  =  0,
    SUB  =  1,
    DIV  =  2,
    MUL  =  3,
    SIN  =  4,
    COS  =  5,
    TG   =  6,
    CTG  =  7,
    LN   =  8,
    SQRT =  9,
    EXP  = 10,
    POW  = 11,
    ASIN = 12,
    ACOS = 13,
    ATG  = 14,
    ACTG = 15,
    SH   = 16,
    CH   = 17,
    TH   = 18,
    CTH  = 19,
};

struct Operation {
    NodeOperatorCodes OpCode;
    const char*         name;
    NodeDataType      OpType;
};

const Operation OPERATIONS[] {
    { ADD,      "+", BI_OP},
    { SUB,      "-", BI_OP},
    { DIV,      "/", BI_OP},
    { MUL,      "*", BI_OP},
    { SIN,    "sin", UN_OP},
    { COS,    "cos", UN_OP},
    {  TG,    "tan", UN_OP},
    { CTG,    "cot", UN_OP},
    {  LN,     "ln", UN_OP},
    {SQRT,   "sqrt", UN_OP},
    { EXP,    "exp", UN_OP},
    { POW,      "^", BI_OP},
    {ASIN, "arcsin", UN_OP},
    {ACOS, "arccos", UN_OP},
    { ATG, "arctan", UN_OP},
    {  SH,   "sinh", UN_OP},
    {  CH,   "cosh", UN_OP},
    {  TH,   "tanh", UN_OP},
    { CTH,   "coth", UN_OP},
};

const size_t OPERATIONS_COUNT = sizeof(OPERATIONS) / sizeof(Operation);

#endif // OPERATIONS_H