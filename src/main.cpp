#include <stdio.h>
#include <time.h>

#include "Tools.h"
#include "BinaryTree.h"
#include "Frontend.h"
#include "TreeDump.h"


const char* INPUT_FILENAME = "../Language/Programs/square_solver.red";

int main() {
    srand((unsigned int)time(NULL));

    Text* program_text = ReadTextFromProgramFile(INPUT_FILENAME);

    Tokens* tokens = GetLexerTokens(program_text);

    //!printf(RED("%lu\n"), tokens->size);

    ProgramTextDtor(program_text);

    Tree* ast = CreateAST(tokens);
    WriteAST(ast);

    /*for (size_t i = 0; i < ast->nametable->free; i++) {
        printf("%s\n", ast->nametable->names[i]);
    }*/

    TREE_DUMP(ast, "End: %s", __func__);

    TreeSimplify(ast);

    TREE_DUMP(ast, "End: %s", __func__);

    TreeDtor(ast);
    TokensDtor(tokens);

    return 0;
}