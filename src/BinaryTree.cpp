/*!
    \file
    File with binary tree functions
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "BinaryTree.h"
#include "TreeDump.h"

/*!
    @brief Function that creates binary tree
    \param [out] root - pointer on tree root
    @return The pointer on the tree
*/
Tree* TreeCtor(Node* root) {
    ASSERT(root != NULL, "NULL POINTER WAS PASSED!\n");

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    if (!tree) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }

    tree->root = root;

    return tree;
}

/*!
    @brief Function that creates node
    \param [in] value - node data
    @return The pointer on the node
*/
Node* CreateNode(NodeDataType type, NodeData value, Node* left, Node* right) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {
        fprintf(stderr, RED("MEMORY ERROR!\n"));

        return NULL;
    }

    node->type  =  type;
    node->data  = value;
    node->left  =  left;
    node->right = right;

    return node;
}

/*!
    @brief Function that deletes tree node
    \param [out] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode SubTreeDtor(Node* node) {
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");

    if (node->left) {
        SubTreeDtor(node->left);
    }

    if (node->right) {
        SubTreeDtor(node->right);
    }

    FREE(node);

    return SUCCESS;
}

/*!
    @brief Function that deletes binary tree
    \param [out] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode TreeDtor(Tree* tree) {
    ASSERT(tree != NULL, "NULL POINTER WAS PASSED!\n");

    SubTreeDtor(tree->root);
    FREE(tree);

    return SUCCESS;
}

FuncReturnCode TreeNodeDtor(Node* node) {
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");
    FREE(node);

    return SUCCESS;
}

int SubTreeHaveArgs(Node* node) {
    if (!node)             return 0;
    if (node->type == NUM) return 0;
    if (node->type == VAR) return 1;

    return SubTreeHaveArgs(node->left) + SubTreeHaveArgs(node->right);
}

FuncReturnCode SubTreeToNum(Node* node, NodeData value) {
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");

    node->data = value;
    node->type = NUM;

    SubTreeDtor(node->left);
    SubTreeDtor(node->right);

    node->left = NULL;
    node->right = NULL;

    return SUCCESS;
}

void SyntaxError() {
    fprintf(stderr, "SyntaxError!\n");
    abort();
}

Node* GetG(ReadString* rs) {
    if (!rs) SyntaxError();

    //TODO macro SHIFT() -> p++ !!!

    Node* val = GetE(rs);

    if (rs->string[rs->pointer] != '$') {
        SyntaxError();
    }
    rs->pointer++;

    return val;
}

Node* GetE(ReadString* rs) {
    if (!rs) SyntaxError();

    Node* val = GetT(rs);
    while (rs->string[rs->pointer] == '+' || rs->string[rs->pointer] == '-') {
        int op = rs->string[rs->pointer];
        rs->pointer++;

        Node* val2 = GetT(rs);

        if (op == '+') {
            val = CreateNode(BI_OP, ADD, val, val2);
        } else {
            val = CreateNode(BI_OP, SUB, val, val2);
        }
    }

    return val;
}

Node* GetT(ReadString* rs) {
    if (!rs) SyntaxError();

    Node* val = GetP(rs);

    while (rs->string[rs->pointer] == '*' || rs->string[rs->pointer] == '/') {
        int op = rs->string[rs->pointer];

        rs->pointer++;
        Node* val2 = GetP(rs);

        if (op == '*') {
            val = CreateNode(BI_OP, MUL, val, val2);
        } else {
            val = CreateNode(BI_OP, DIV, val, val2);
        }
    }

    return val;
}

Node* GetP(ReadString* rs) {
    if (!rs) SyntaxError();

    Node* val = GetB(rs);

    if (rs->string[rs->pointer] == '^') {
        rs->pointer++;
        Node* val2 = GetP(rs);

        return CreateNode(BI_OP, POW, val, val2);
    }
    return val;
}

Node* GetB(ReadString* rs) {
    if (!rs) SyntaxError();

    if (rs->string[rs->pointer] == '(') {
        rs->pointer++;
        Node* val = GetE(rs);

        if (rs->string[rs->pointer] != ')') {
            SyntaxError();
        }
        rs->pointer++;

        return val;
    } else {
        return GetS(rs);
    }
}

Node* GetS(ReadString* rs) {
    if (!rs) SyntaxError();

    size_t old_p = rs->pointer;
    Node* val = GetV(rs);

    if (old_p == rs->pointer) return GetN(rs);

    return val;
}

Node* GetV(ReadString* rs) {
    if (!rs) SyntaxError();

    size_t  old_p = rs->pointer;
    char read_name[MAX_NAME_LENGTH] = {};

    //! isalpha
    while (isalpha(rs->string[rs->pointer]) && rs->pointer - old_p < MAX_NAME_LENGTH) {
        read_name[rs->pointer - old_p] = rs->string[rs->pointer];
        rs->pointer++;
    }

    if (old_p == rs->pointer) return NULL;

    if (rs->pointer - old_p == 1 && rs->string[rs->pointer] != '(') {
        return CreateNode(VAR, (int)(*read_name), NULL, NULL); //! podumai
    }

    else if (rs->string[rs->pointer] == '(') {
        rs->pointer++;
        Node* val = GetF(rs, read_name);

        if (rs->string[rs->pointer] != ')') SyntaxError();  //! TODO macro for assert (SyntaxAssert)
        rs->pointer++;

        return val;
    }

    SyntaxError();
    return NULL;
}

Node* GetF(ReadString* rs, const char* read_name) {
    if (!rs || !read_name) SyntaxError();

    Node* val = GetE(rs);

    for (size_t i = 0; i < OPERATIONS_COUNT; i++) {
        if (strcmp(read_name, OPERATIONS[i].name) == 0) {
            return CreateNode(OPERATIONS[i].OpType, OPERATIONS[i].OpCode, NULL, val);
        }
    }

    SyntaxError();
    return NULL;
}

Node* GetN(ReadString* rs) {
    if (!rs) SyntaxError();

    size_t old_p = rs->pointer;
    char* end = {};

    NodeData number = strtod(&(rs->string[rs->pointer]), &end);

    if (rs->string[old_p - 1] == *end) { // TODO
        SyntaxError();
    }
    rs->pointer += (size_t)(end - &rs->string[old_p]);

    return CreateNode(NUM, number, NULL, NULL);
}

FuncReturnCode ConnectChildWithParent(Node* node, NodeLocation location) {

    if (!node) return SUCCESS;

    Node* child_node = location == LEFT ? node->left : node->right;

    if (!child_node) fprintf(stderr, RED("Nothing to connect, child null pointer"));

    node->data  = child_node->data;
    node->type  = child_node->type;

    if (child_node == node->left)
        TreeNodeDtor(node->right);
    else
        TreeNodeDtor(node->left);

    node->left  = child_node->left;
    node->right = child_node->right;

    TreeNodeDtor(child_node);

    return SUCCESS;
}

static size_t GetFileLength(const char* filename) {
    if (!filename) return NULL;

    struct stat st = {};
    stat(filename, &st);

    return (size_t)st.st_size;
}

ReadString* ReadExpFromFile(const char* filename) {
    if (!filename) {
        fprintf(stderr, "Error with occured the filename pointer!\n");
        return NULL;
    }

    ReadString* rs = (ReadString*) calloc(1, sizeof(ReadString));
    if (!rs) {
        fprintf(stderr, "Memory error!\n");
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error with occured with open the file!\n");
        return NULL;
    }

    size_t file_size = GetFileLength(filename);

    rs->string = (char*) calloc(file_size, sizeof(char));
    if (!rs->string) {
        fprintf(stderr, "Memory error!\n");
        return NULL;
    }

    fread(rs->string, sizeof(char), file_size, file);
    fclose(file);

    return rs;
}

FuncReturnCode ReadStringDtor(ReadString* rs) {
    ASSERT(rs != NULL, "NULL POINTER WAS PASSED!\n");

    FREE(rs->string);
    FREE(rs);

    return SUCCESS;
}

FuncReturnCode MemoryFree(Tree* tree, Tree* diff_tree, ReadString* rs) {
    ASSERT(tree      != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(diff_tree != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(rs        != NULL, "NULL POINTER WAS PASSED!\n");

    TreeDtor(tree);
    TreeDtor(diff_tree);
    ReadStringDtor(rs);

    return SUCCESS;
}

NameTable* NameTableCtor() {
    NameTable* nametable = (NameTable*) calloc(1, sizeof(NameTable));
    NULL_CHECK(nametable);

    for (size_t i = 0; i < NAMETABLE_SIZE; i++) {
        nametable->names[i] = (char*) calloc(MAX_NAME_LENGTH, sizeof(char));
        NULL_CHECK(nametable->names[i]);

        nametable->parameters[i] = NULL;
    }

    nametable->free_names = 0;

    return nametable;
}

void NameTableDtor(NameTable* nametable) {
    ASSERT(nametable != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < NAMETABLE_SIZE; i++) {
        FREE(nametable->names[i]);
    }

    FREE(nametable);
}