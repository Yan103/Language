#include <stdio.h>

#include "Tools.h"
#include "BinaryTree.h"
#include "Frontend.h"

const char* INPUT_FILENAME = "../Language//Programs/test1.red";

int main() {

    Text* program_text = ReadTextFromProgramFile(INPUT_FILENAME);
    Tokens* tokens = GetLexerTokens(program_text);

    ProgramTextDtor(program_text);

    TokensDtor(tokens);

    return 0;
}