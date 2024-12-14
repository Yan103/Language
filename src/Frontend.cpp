#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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

    NameTableDtor(tokens->nametable);
    FREE(tokens->lexems);
    FREE(tokens);
}

Tokens* GetLexerTokens(Text* program_text) {
    ASSERT(program_text != NULL, "NULL POINTER WAS PASSED!\n");

    Tokens* tokens = TokensCtor();
    char lexem[MAX_NAME_LENGTH] = "";
    size_t  read_symbols = 0;

    while (sscanf(program_text->text + program_text->offset, "%s%n", lexem, &read_symbols) != EOF) {
        SHIFT(program_text, read_symbols);
        lexem[ strcspn(lexem, "\n\t\r") ] = '\0';

        if (IS_COMMENT(lexem)) {
            size_t delta = strcspn(program_text->text + program_text->offset, "\n");
            SHIFT(program_text, delta);
            continue;
        }

        if (CheckForUselessLexem(lexem)) {
            size_t delta = strcspn(program_text->text + program_text->offset, " ");
            SHIFT(program_text, delta);
            continue;
        }

        Node* node = NULL;
        int index  = -1, var_index = -1;

        if ((index = CheckForDeclarator(lexem)) != -1) {
            switch (DECLARATORS[index].code) {
                case FUNC_DECLARATOR:
                    node = CreateNode(DECLARATOR, FUNC_DECLARATOR, NULL, NULL);
                    break;
                case VAR_DECLARATOR:
                    node = CreateNode(DECLARATOR, VAR_DECLARATOR, NULL, NULL);
                    break;
                default:
                    fprintf(stderr, "No declarator %s in DECLARATORS!\n", lexem);
                    break;
            }
        } else if ((index = CheckForKeyWord(lexem)) != -1) {
            node = CreateNode(KEYWORD, index, NULL, NULL);
        } else if ((index = CheckForOperator(lexem)) != -1) {
            node = CreateNode(OPERATOR, index, NULL, NULL);
        } else if ((index = CheckForSeparator(lexem)) != -1) {
            node = CreateNode(SEPARATOR, index, NULL, NULL);
        } else if (CheckForNumber(lexem)) {
            node = CreateNode(NUMBER, atoi(lexem), NULL, NULL);
        } else if (CheckForVariable(lexem)) {
            if ((var_index = TryFindInNameTable(lexem, tokens->nametable)) == -1)
                var_index = UpdateInNameTable(lexem, tokens->nametable);

            if (var_index == -1) fprintf(stderr, RED("Could't find place in nanetable!\n"));

            node = CreateNode(VARIABLE, var_index, NULL, NULL);
        } else {
            fprintf(stderr, RED("Unknown lexem in code \"%s\"\n"), lexem);
        }

        tokens->lexems[tokens->size++] = node;
    }

    return tokens;
}

int CheckForUselessLexem(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < USELESS_LEXEM_COUNT; i++) {
        if (strcmp(lexem, USELESS_LEXEMS[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int CheckForDeclarator(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < DECLARATORS_COUNT; i++) {
        if (strcmp(lexem, DECLARATORS[i].name) == 0) {
            return int(i);
        }
    }

    return -1;
}

int CheckForKeyWord(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < KEYWORDS_COUNT; i++) {
        if (strcmp(lexem, KEYWORDS[i].name) == 0) {
            return int(i);
        }
    }

    return -1;
}

int CheckForOperator(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < OPERATORS_COUNT; i++) {
        if (strcmp(lexem, OPERATORS[i].name) == 0) {
            return int(i);
        }
    }

    return -1;
}

int CheckForSeparator(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < SEPARATORS_COUNT; i++) {
        if (strcmp(lexem, SEPARATORS[i].name) == 0) {
            return int(i);
        }
    }

    return -1;
}

int CheckForNumber(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    if (*lexem == '-') lexem++;
    while (isdigit(*lexem)) lexem++;

    if (isspace(*lexem) || *lexem == '\0') return 1;

    return 0;
}

int CheckForVariable(const char* lexem) {
    ASSERT(lexem != NULL, "NULL POINTER WAS PASSED!\n");

    if (!isalpha (*lexem) && *lexem == '_') return 0;

    while (*++lexem) {
        if (!isalnum (*lexem) && *lexem == '_') return 0;
    }

    return 1;
}

int TryFindInNameTable(const char* lexem, const NameTable* nametable) {
    ASSERT(lexem     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(nametable != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < NAMETABLE_SIZE; i++) {
        if (strcmp(lexem, nametable->names[i]) == 0) return int(i);
    }

    return -1;
}

int UpdateInNameTable(const char* lexem, NameTable* nametable) {
    ASSERT(lexem     != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(nametable != NULL, "NULL POINTER WAS PASSED!\n");

    strcpy(nametable->names[nametable->free], lexem);

    return int(nametable->free++);
}