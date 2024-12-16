#ifndef FRONTEND_H
#define FRONTEND_H

#include <stdio.h>

#include "Tools.h"
#include "BinaryTree.h"
#include "LanguageSyntaxis.h"

#define DELTA_SHIFT(text, delta) { (text)->offset += delta; }
#define IS_COMMENT(lexem)  *(lexem) == '/'

#define SYNTAX_ASSERT(condition, text_error) SyntaxAssert(condition, text_error, __FILE__, __func__, __LINE__);
#define SHIFT(tokens) { (tokens)->offset++;}


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

void SyntaxAssert(bool condition, const char *text_error, const char *file, const char *func, int line);

Tree* CreateAST(Tokens* tokens);

FuncReturnCode WriteAST(const Tree* ast);

Node* GetTree              (Tokens* tokens);
Node* GetFuncDeclarator    (Tokens* tokens);
Node* GetCompoundStatement (Tokens* tokens);
Node* GetBlockStatement    (Tokens* tokens);
Node* GetSimpleStatement   (Tokens* tokens);
Node* GetIf                (Tokens* tokens);
Node* GetWhile             (Tokens* tokens);
Node* GetAssign            (Tokens* tokens);
Node* GetReturn            (Tokens* tokens);
Node* GetPrint             (Tokens* tokens);
Node* GetVarDeclarator     (Tokens* tokens);
Node* GetExpression        (Tokens* tokens);
Node* GetPlusMinusRes      (Tokens* tokens);
Node* GetMulDivRes         (Tokens* tokens);
Node* GetOperation         (Tokens* tokens);
Node* GetSimpleCondition   (Tokens* tokens);
Node* GetFuncCall          (Tokens* tokens);
Node* GetIdentificator     (Tokens* tokens);
Node* GetParameter         (Tokens* tokens);
Node* GetNumber            (Tokens* tokens);


#endif // FRONTEND_H