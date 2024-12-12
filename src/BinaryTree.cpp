#include <ctype.h>

#include "BinaryTree.h"

const size_t MAX_OPERATION_NAME_SIZE = 100;

static int SkipSpaces(FILE* filename, int symbol) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    while (isspace(symbol = fgetc(filename))) {
        ;
    }

    return symbol;
}

static int SkipComments(FILE* filename, int symbol) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    while ((symbol = fgetc(filename)) != '\n'){
        ;
    }

    return symbol;
}

void ReadFile(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    int symbol = 0;

    symbol = SkipSpaces(filename, symbol);

    if (symbol == '\n')      printf("\n");
    else if (symbol == '/')  SkipComments(filename, symbol);

    printf("%s ", ReadOperation(filename));
}

char* ReadOperation(FILE* filename) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");

    char symbol = 0;
    symbol = (char) SkipSpaces(filename, int(symbol));

    if (!isalpha(int(symbol))) {
        printf("%c\n", symbol);
        printf("%d\n", symbol);
        fprintf(stderr, RED("ERROR WITH READ TREE FROM FILE1\n"));

        // abort() ?
    }

    char* node_data = (char*) calloc(1, MAX_OPERATION_NAME_SIZE * sizeof(char));
    if (!node_data) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        // abort() ?
    }

    fscanf(filename, "%s", node_data);
    symbol = (char)fgetc(filename);

    return node_data;
}