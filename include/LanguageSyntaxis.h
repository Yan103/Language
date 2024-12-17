#ifndef LANGUAGESYNTAXIS_H
#define LANGUAGESYNTAXIS_H

#include <stdio.h>

enum DeclaratorCode {
    FUNC_DECLARATOR = 0,
    VAR_DECLARATOR  = 1,
};

struct Declarator {
    const char*    name;
    DeclaratorCode code;
};

const Declarator DECLARATORS[] = {
    {"итак_коллеги", FUNC_DECLARATOR},
    {"родные_фивты", VAR_DECLARATOR},
};

const size_t DECLARATORS_COUNT = sizeof(DECLARATORS) / sizeof(DECLARATORS[0]);

enum KeyWordsCode {
    IF     = 0,
    ELSE   = 1,
    WHILE  = 2,
    RETURN = 3,
    SCAN   = 4,
    PRINT  = 5,
};

struct KeyWord {
    const char*  name;
    KeyWordsCode code;
};

// TODO keyword for else
const KeyWord KEYWORDS[] = {
    {"ееесссли",                 IF},
    {"сейчас_пойдёт_деградация", WHILE},
    {"получаем",                 RETURN},
    {"так_и_запишем",            SCAN}, // запомним_это
    {"покажем_что",              PRINT},
};

const size_t KEYWORDS_COUNT = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

enum OperatorCode {
    ADD        = 0,
    SUB        = 1,
    MUL        = 2,
    DIV        = 3,
    LESS       = 4,
    MORE       = 5,
    LESS_EQUAL = 6,
    MORE_EQUAL = 7,
    EQUAL      = 8,
    NOT_EQUAL  = 9,
    ASSIGN     = 10,
    SQRT       = 11,
};

struct Operator {
    const char*  name;
    OperatorCode code;
};

const Operator OPERATORS[] = {
    {"+",                    ADD},
    {"-",                    SUB},
    {"*",                    MUL},
    {"/",                    DIV},
    {"<",                    LESS},
    {">",                    MORE},
    {"<=",                   LESS_EQUAL},
    {">=",                   MORE_EQUAL},
    {"==",                   EQUAL},
    {"!=",                   NOT_EQUAL},
    {"зафиксируем_эпсилон:", ASSIGN},
    {"корень",                     SQRT},
};

const size_t OPERATORS_COUNT = sizeof(OPERATORS) / sizeof(OPERATORS[0]);

enum SeparatorCode {
    END_LINE              = 0,
    BEGIN_FUNC_PARAMETERS = 1,
    END_FUNC_PARAMETERS   = 2,
    BEGIN_EXPRESSION      = 3,
    END_EXPRESSION        = 4,
    BEGIN_STATEMENT_BODY  = 5,
    END_STATEMENT_BODY    = 6,
    END_CONDITION         = 7,
};

struct Separator {
    const char*   name;
    SeparatorCode code;
};

const Separator SEPARATORS[] = {
    {"перерыв_коллеги",           END_LINE},
    {"прочувствуйте",             BEGIN_FUNC_PARAMETERS},
    {"следующий_факт",            END_FUNC_PARAMETERS},
    {"(",                         BEGIN_EXPRESSION},
    {")",                         END_EXPRESSION},
    {"начинаем_очередную_лекцию", BEGIN_STATEMENT_BODY},
    {"коллеги_лекция_закончена",  END_STATEMENT_BODY},
    {"=>",                        END_CONDITION},
};

const size_t SEPARATORS_COUNT = sizeof(SEPARATORS) / sizeof(SEPARATORS[0]);

const char* const USELESS_LEXEMS[] = {
    "заметим",
    "очевидно",
    "матан",
};

const size_t USELESS_LEXEM_COUNT = sizeof(USELESS_LEXEMS) / sizeof(USELESS_LEXEMS[0]);

#endif // LANGUAGESYNTAXIS_H