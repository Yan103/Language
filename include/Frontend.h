#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdio.h>

#include "Tools.h"
#include "BinaryTree.h"

const size_t TOKENS_COUNT = 1024;

struct Text {
    char*    text;
    size_t offset;
};

struct Tokens {
    Node**        lexems;
    NameTable* nametable;
    size_t          size;
    size_t        offset;
};

Text* ReadTextFromProgramFile(const char* program_file);

Text* ProgramTextCtor(const char* program_code, const size_t program_size);

Text* ProgramTextDtor(Text* program_text);

Tokens* TokensCtor();

void TokensDtor(Tokens* tokens);

Tokens* GetLexerTokens(Text* program_text);

#endif // FRONTEND_H