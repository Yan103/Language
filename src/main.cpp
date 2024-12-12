#include <stdio.h>

#include "Tools.h"
#include "BinaryTree.h"

const char* INPUT_FILENAME = "../Language//Programs/test2.red";

int main() {
    FILE* file = fopen(INPUT_FILENAME, "r");
    ReadFile(file);

    return 0;
}