#include <sys/stat.h>
#include <string.h>

#include "Frontend.h"
#include "BinaryTree.h"

static size_t GetFileLength(const char* filename) {
    struct stat st = {};
    stat(filename, &st);

    return size_t(st.st_size);
}

Text* ReadTextFromProgramFile(const char* program_name) {
    ASSERT(program_name != NULL, "NULL POINTER WAS PASSED!\n");

    FILE* program_file = fopen(program_name, "rb");
    NULL_CHECK(program_file);

    size_t program_file_size = GetFileLength(program_name);

    char* program_code = (char*) calloc(program_file_size + 1, sizeof(char));
    NULL_CHECK(program_code);

    size_t read_symbols = fread(program_code, sizeof(char), program_file_size, program_file);
    fclose(program_file);

    if (read_symbols != program_file_size) {
        fprintf(stderr, "Wrong number of characters was considered!!!\n");
        return NULL;
    }

    Text* program_text = ProgramTextCtor(program_code, program_file_size + 1);
    FREE(program_code);

    return program_text;
}

Text* ProgramTextCtor(const char* program_code, const size_t program_size) {
    ASSERT(program_code != NULL, "NULL POINTER WAS PASSED!\n");

    char* copied_text = (char*) calloc(program_size, sizeof(char));
    NULL_CHECK(copied_text);

    Text* program_text = (Text*) calloc(1, sizeof(Text));
    NULL_CHECK(program_text);

    strncpy(copied_text, program_code, program_size);

    program_text->text   = copied_text;
    program_text->offset = 0;

    return program_text;
}

Text* ProgramTextDtor(Text* program_text) {
    ASSERT(program_text != NULL, "NULL POINTER WAS PASSED!\n");

    FREE(program_text->text);
    FREE(program_text);

    return 0;
}

Tokens* TokensCtor() {
    Tokens* tokens = (Tokens*) calloc(1, sizeof(Tokens));
    NULL_CHECK(tokens);

    tokens->lexems = (Node**) calloc(TOKENS_COUNT, sizeof(Node*));
    NULL_CHECK(tokens->lexems);

    tokens->nametable = NameTableCtor();
    tokens->offset    = 0;
    tokens->size      = 0;

    return tokens;
}

void TokensDtor(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < tokens->size && tokens->lexems[i]; i++) {
        FREE(tokens->lexems[i]);
    }

    FREE(tokens->lexems);
    FREE(tokens);
}