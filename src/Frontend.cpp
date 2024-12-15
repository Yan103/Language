#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "Frontend.h"
#include "BinaryTree.h"

const char* AST_FILENAME = "../Language/ast.txt";

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
        if (!isalnum (*lexem) && *lexem == '_') return 0; //! запрещаю делать переменные с _
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

FuncReturnCode WriteAST(const Tree* ast) {
    ASSERT(ast != NULL, "NULL POINTER WAS PASSED!\n");

    FILE* ast_file = fopen(AST_FILENAME, "wb");
    //NULL_CHECK(ast_file);

    WriteTree(ast_file, ast);
    fclose(ast_file);

    return SUCCESS;
}

Tree* CreateAST(const Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    Tree* ast = TreeCtor();
    CopyOfNameTable(ast->nametable, tokens->nametable);

    ast->root = GetTree(tokens, ast->nametable);

    return ast;
}

void SyntaxAssert(bool condition, const char *text_error, const char *file, const char *func, int line) {
    if (!(condition)) {
        fprintf(stderr, RED("%s in: %s -> %s -> %d\n"), text_error, file, func, line);
        abort();
    }
}

Node* GetTree(Tokens* tokens) {
    ASSERT(tokens    != NULL, "NULL POINTER WAS PASSED!\n");

    Node* new_statement_node = NULL;
    Node* end_statement_node = NULL;

    do {
        new_statement_node = GetFuncDeclarator(tokens);

        if (new_statement_node == NULL) new_statement_node = GetCompoundStatement(tokens);

        if (new_statement_node)
            end_statement_node = CreateNode(END_LINE, SEPARATOR, end_statement_node, new_statement_node);

    } while (new_statement_node && tokens->offset < tokens->size);

    SYNTAX_ASSERT(end_statement_node != NULL, "Syntax error!\n");

    return end_statement_node;
}

Node* GetFuncDeclarator(Tokens* tokens) {
    ASSERT(tokens    != NULL, "NULL POINTER WAS PASSED!\n");

    if (tokens->lexems[tokens->offset]->type != DECLARATOR ||
        tokens->lexems[tokens->offset]->data != FUNC_DECLARATOR) return NULL;
    SHIFT(tokens); //* проверяем что правильно записано начало

    Node* func_name_node = GetIdentificator(tokens);
    SYNTAX_ASSERT(func_name_node != NULL, "Syntax error!\n"); //* имя функции

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != BEGIN_FUNC_PARAMETERS) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверяем что правильно записано начало

    Node*  parameters_node    = NULL;
    Node*  new_parameter_node = NULL;
    int    parameters_count   = 0;

    do {  //* хотим считывать параметры функции (возможно их несколько)
        new_parameter_node = GetNewParameter(tokens);
        if (new_parameter_node) {
            parameters_count++;
            parameters_node = CreateNode(SEPARATOR, END_LINE, parameters_node, new_parameter_node);
        }
    } while (new_parameter_node);

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != END_FUNC_PARAMETERS) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    tokens->nametable->parameters_count[ func_name_node->data ] = parameters_count;

    Node* func_body = GetBlockStatement(tokens);
    SYNTAX_ASSERT(func_body != NULL, "Syntax error!\n");

    Node* func_info = CreateNode(SEPARATOR, END_LINE, parameters_node, func_name_node);
    //* правый сын - имя функции + параметры
    //* левый сын  - что делается

    return CreateNode(DECLARATOR, FUNC_DECLARATOR, func_body, func_info);
}

Node* GetCompoundStatement(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    int old_offset = tokens->offset;

    Node* statement = GetBlockStatement(tokens);
    if (statement) return statement;

    tokens->offset = old_offset;

    return GetSimpleStatement(tokens);
}

Node* GetBlockStatement(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != BEGIN_STATEMENT_BODY) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* statement_block = NULL;
    Node* statement       = NULL;

    do {
        statement = GetCompoundStatement(tokens);

        if (statement) statement_block = CreateNode(SEPARATOR, END_LINE, statement_block, statement);

    } while (statement);

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != END_STATEMENT_BODY) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    return statement_block;
}

Node* GetSimpleStatement(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    int old_offset = tokens->offset;

    Node* simple_statement = NULL;

    simple_statement = GetIf(tokens);
    if (simple_statement) return simple_statement;

    tokens->offset = old_offset;

    simple_statement = GetWhile(tokens);
    if (simple_statement) return simple_statement;

    tokens->offset = old_offset;

    simple_statement = GetAssign(tokens);
    if (simple_statement) {

        if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
            tokens->lexems[tokens->offset]->data != END_LINE) SYNTAX_ASSERT(0, "Syntax error!\n");
        SHIFT(tokens); //* проверка на синтаксис + скип
    }

    tokens->offset = old_offset;

    simple_statement = GetReturn(tokens);
    if (simple_statement) {

        if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
            tokens->lexems[tokens->offset]->data != END_LINE) SYNTAX_ASSERT(0, "Syntax error!\n");
        SHIFT(tokens); //* проверка на синтаксис + скип
    }

    tokens->offset = old_offset;

    simple_statement = GetPrint(tokens);
    if (simple_statement == NULL) {  //* это последний, поэтому если нет, то пока(

        tokens->offset = old_offset;
        return NULL;
    }

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != END_LINE) SYNTAX_ASSERT(0, "Syntax error!\n");

    SHIFT(tokens);

    return simple_statement;
}

Node* GetIf(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    if (tokens->lexems[tokens->offset]->type != KEYWORD ||
        tokens->lexems[tokens->offset]->data != IF) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* condition = GetExpression(tokens);
    SYNTAX_ASSERT(condition != NULL, "Syntax error!\n");

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != END_CONDITION) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* if_statement = GetCompoundStatement(tokens);
    SYNTAX_ASSERT(if_statement != NULL, "Syntax error!\n");

    Node* if_else_statement = CreateNode(KEYWORD, IF, if_statement, NULL);

    if (tokens->lexems[tokens->offset]->type != KEYWORD ||
        tokens->lexems[tokens->offset]->data != ELSE) return CreateNode(KEYWORD, IF, if_else_statement, condition);
    SHIFT(tokens);

    Node* else_statement = GetCompoundStatement(tokens);
    SYNTAX_ASSERT(else_statement != NULL, "Syntax error!\n");

    if_else_statement->right = else_statement;

    return CreateNode(KEYWORD, IF, if_else_statement, condition);
}

Node* GetWhile(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    if (tokens->lexems[tokens->offset]->type != KEYWORD ||
        tokens->lexems[tokens->offset]->data != WHILE) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* condition = GetExpression(tokens);
    SYNTAX_ASSERT(condition != NULL, "Syntax error!\n");

    if (tokens->lexems[tokens->offset]->type != SEPARATOR ||
        tokens->lexems[tokens->offset]->data != END_CONDITION) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* while_statement = GetCompoundStatement(tokens);
    SYNTAX_ASSERT(while_statement != NULL, "Syntax Error!\n");

    return CreateNode(KEYWORD, WHILE, while_statement, condition);
}

Node* GetReturn(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    if (tokens->lexems[tokens->offset]->type != KEYWORD ||
        tokens->lexems[tokens->offset]->data != RETURN) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* ret_value = GetExpression(tokens);
    SYNTAX_ASSERT(ret_value != NULL, "Syntax error!\n");

    return CreateNode(KEYWORD, RETURN, ret_value, NULL);
}

Node* GetPrint(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

    if (tokens->lexems[tokens->offset]->type != KEYWORD ||
        tokens->lexems[tokens->offset]->data != PRINT) SYNTAX_ASSERT(0, "Syntax error!\n");
    SHIFT(tokens); //* проверка на синтаксис + скип

    Node* print_value = GetExpression(tokens);
    SYNTAX_ASSERT(print_value != NULL, "Syntax error!\n");

    return CreateNode(KEYWORD, PRINT, print_value, NULL);
}

Node* GetAssign(Tokens* tokens) {
    ASSERT(tokens != NULL, "NULL POINTER WAS PASSED!\n");

}