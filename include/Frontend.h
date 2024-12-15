#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdio.h>

#include "Tools.h"
#include "BinaryTree.h"
#include "LanguageSyntaxis.h"

#define SHIFT(text, delta) { (text)->offset += delta; }
#define IS_COMMENT(lexem)  *(lexem) == '/'

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

int CheckForUselessLexem(const char* lexem);

int CheckForDeclarator(const char* lexem);

int CheckForKeyWord(const char* lexem);

int CheckForOperator(const char* lexem);

int CheckForSeparator(const char* lexem);

int CheckForNumber(const char* lexem);

int CheckForVariable(const char* lexem);

int TryFindInNameTable(const char* lexem, const NameTable* nametable);

int UpdateInNameTable(const char* lexem, NameTable* nametable);

Tree* CreateAST(const Tokens* tokens);

FuncReturnCode WriteAST(const Tree* ast);

Node* GetTree(Tokens* tokens, NameTable* nametable);

Node* GetFuncDeclarator(Tokens* tokens, NameTable* nametable);
Node* GetCompoundStatement(Tokens* tokens, NameTable* nametable);

Node* GetBlockStatement  (Tokens* tokens, NameTable* nametable);
Node* GetSimpleStatement (Tokens* tokens, NameTable* nametable);
Node* GetIf              (Tokens* tokens, NameTable* nametable);
Node* GetWhile           (Tokens* tokens, NameTable* nametable);
Node* GetAssign          (Tokens* tokens, NameTable* nametable);
Node* GetReturn          (Tokens* tokens, NameTable* nametable);
Node* GetPrint           (Tokens* tokens, NameTable* nametable);
Node* GetVarDeclarator   (Tokens* tokens, NameTable* nametable);
Node* GetExpression      (Tokens* tokens, NameTable* nametable);
Node* GetPlusMinusRes    (Tokens* tokens, NameTable* nametable);
Node* GetMulDivRes       (Tokens* tokens, NameTable* nametable);
Node* GetOperation       (Tokens* tokens, NameTable* nametable);
Node* GetSimpleCondition (Tokens* tokens, NameTable* nametable);
Node* GetFuncCall        (Tokens* tokens, NameTable* nametable);
Node* GetIdentificator   (Tokens* tokens, NameTable* nametable);
Node* GetNumber          (Tokens* tokens, NameTable* nametable);


#endif // FRONTEND_H