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
#include "LanguageSyntaxis.h"

/*!
    @brief Function that creates binary tree
    \param [out] root - pointer on tree root
    @return The pointer on the tree
*/
Tree* TreeCtor() {
    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    NULL_CHECK(tree);

    tree->nametable = NameTableCtor();
    NULL_CHECK(tree->nametable);

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
    NameTableDtor(tree->nametable);
    FREE(tree);

    return SUCCESS;
}

FuncReturnCode TreeNodeDtor(Node* node) {
    ASSERT(node != NULL, "NULL POINTER WAS PASSED!\n");
    FREE(node);

    return SUCCESS;
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

        nametable->parameters[i]       = NULL;
        nametable->parameters_count[i] = 0;
    }

    nametable->free = 0;

    return nametable;
}

void NameTableDtor(NameTable* nametable) {
    ASSERT(nametable != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < NAMETABLE_SIZE; i++) {
        FREE(nametable->names[i]);
    }

    FREE(nametable);
}

/*!
    @brief Function that writes binary tree in the file
    \param [in] filename - pointer on the file
    \param [in]     tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode WriteTree(FILE* filename, const Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n");

    WriteSubTree(filename, tree->root, tree);

    return SUCCESS;
}

/*!
    @brief Function that writes subtree in file
    \param [in] filename - pointer on the file
    \param [in]     node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode WriteSubTree(FILE* filename, const Node* node, const Tree* tree) {
    ASSERT(filename != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(tree     != NULL, "NULL POINTER WAS PASSED!\n");

    if (node == NULL) {
        fprintf(filename, "* ");

        return SUCCESS;
    }

    fprintf(filename, "{ ");

    WriteSubTree(filename, node->left, tree);
    WriteSubTreeNodeData(filename, node->type, node->data, tree->nametable);
    WriteSubTree(filename, node->right, tree);

    fprintf(filename, "} ");

    return SUCCESS;
}

FuncReturnCode WriteSubTreeNodeData(FILE* filename, NodeDataType type, NodeData data, const NameTable* nametable) {
    ASSERT(filename  != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(nametable != NULL, "NULL POINTER WAS PASSED!\n");

    int index = -1;

    switch (type) {
        case NUMBER: {
            fprintf(filename, "%d ", data);
            break;
        }
        case VARIABLE: {
            fprintf(filename, "%s ", nametable->names[data]);
            break;
        }
        case DECLARATOR: {
            if ((index = FindDeclarator(data)) != -1) fprintf(filename, "%s ", DECLARATORS[index].name);
            else                                      fprintf(stderr, RED("Unknown declarator!\n"));
            break;
        }
        case KEYWORD: {
            if ((index = FindKeyWord(data)) != -1)    fprintf(filename, "%s ", KEYWORDS[index].name);
            else                                      fprintf(stderr, RED("Unknown keyword!\n"));
            break;
        }
        case SEPARATOR: {
            if ((index = FindSeparator(data)) != -1)  fprintf(filename, "%s ", SEPARATORS[index].name);
            else                                      fprintf(stderr, RED("Unknown separator!\n"));
            break;
        }
        case OPERATOR: {
            if ((index = FindOperator(data)) != -1)   fprintf(filename, "%s ", OPERATORS[index].name);
            else                                      fprintf(stderr, RED("Unknown operator!\n"));
            break;
        }
        default:
            fprintf(stderr, "Unknown error in WriteSubTreeNodeData!\n");
            return UNKNOWN_ERROR;
    }

    return SUCCESS;
}

int FindDeclarator(const NodeData code) {
    for (size_t i = 0; i < DECLARATORS_COUNT; i++) {
        if (code == DECLARATORS[i].code)
            return int(i);
    }

    return -1;
}

int FindKeyWord(const NodeData code) {
    for (size_t i = 0; i < KEYWORDS_COUNT; i++) {
        if (code == KEYWORDS[i].code)
            return int(i);
    }

    return -1;
}

int FindSeparator(const NodeData code) {
    for (size_t i = 0; i < SEPARATORS_COUNT; i++) {
        if (code == SEPARATORS[i].code)
            return int(i);
    }

    return -1;
}

int FindOperator(const NodeData code) {
    for (size_t i = 0; i < OPERATORS_COUNT; i++) {
        if (code == OPERATORS[i].code)
            return int(i);
    }

    return -1;
}

FuncReturnCode CopyOfNameTable(NameTable* nt_dest, const NameTable* nt_src) {
    ASSERT(nt_dest != NULL, "NULL POINTER WAS PASSED!\n");
    ASSERT(nt_src  != NULL, "NULL POINTER WAS PASSED!\n");

    for (size_t i = 0; i < NAMETABLE_SIZE; i++) {
        memcpy(nt_dest->names[i], nt_src->names[i], strlen(nt_src->names[i]));
    }

    nt_dest->free = nt_src->free;

    return SUCCESS;
}