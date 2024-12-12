/*!
    \file
    File with binary tree functions
*/

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tools.h"
#include "BinaryTree.h"
#include "Operations.h"

#define NULL_CHECK(pointer)                       \
if (!pointer) {                                   \
    fprintf(stderr, "Null pointer error!!!\n");   \
    return NULL;                                  \
}                                                 \

const size_t MAX_NAME_LENGTH =  200;
const size_t NAMETABLE_SIZE  = 1000;

/// @brief Type of items in a nodes' data
typedef double NodeData;

/// @brief Enum with return functions codes
enum FuncReturnCode {
    SUCCESS               =  0,
    MEMORY_ERROR          = -1,
    FILE_ERROR            = -2,
    UNKNOWN_ERROR         = -3,
    TREE_READ_ERROR       = -4,
    UNKNOWN_FLAG          = -5,
    SYSCALL_ERROR         = -6,
};

struct NameTable {
    char*      names[NAMETABLE_SIZE];
    int*  parameters[NAMETABLE_SIZE];
    size_t                free_names;
};

enum NodeLocation {
    LEFT  = 0,
    RIGHT = 1,
};

/// @brief Structure tree node
struct Node {
    NodeDataType type;
    NodeData     data;
    Node*        left;
    Node*       right;
};

/// @brief Structure binary tree
struct Tree {
    Node*    root;
};

struct ReadString {
    char*         string;
    size_t   pointer = 0;
};

/*!
    @brief Function that creates binary tree
    \param [out] root - pointer on tree root
    @return The pointer on the tree
*/
Tree* TreeCtor(Node* root);

/*!
    @brief Function that creates node
    \param [in]  type - node data type
    \param [in] value - node data
    @return The pointer on the node
*/
Node* CreateNode(NodeDataType type, NodeData value, Node* left, Node* right);

/*!
    @brief Function that deletes binary tree
    \param [out] tree - pointer on tree
    @return The status of the function (return code)
*/
FuncReturnCode TreeDtor(Tree* tree);

/*!
    @brief Function that deletes tree node
    \param [out] node - pointer on node
    @return The status of the function (return code)
*/
FuncReturnCode SubTreeDtor(Node* node);

FuncReturnCode TreeNodeDtor(Node* node);

int SubTreeHaveArgs(Node* node);

FuncReturnCode SubTreeToNum(Node* node, NodeData value);

Node* GetP(ReadString* rs);
Node* GetE(ReadString* rs);
Node* GetG(ReadString* rs);
Node* GetN(ReadString* rs);
Node* GetT(ReadString* rs);
Node* GetB(ReadString* rs);
Node* GetS(ReadString* rs);
Node* GetV(ReadString* rs);
Node* GetF(ReadString* rs, const char* read_name);

void SyntaxError() __attribute__ ((noreturn));

ReadString* ReadExpFromFile(const char* filename);

FuncReturnCode ReadStringDtor(ReadString* rs);

FuncReturnCode ConnectChildWithParent(Node* node, NodeLocation location);

FuncReturnCode MemoryFree(Tree* tree, Tree* diff_tree, ReadString* rs);

NameTable* NameTableCtor();

void NameTableDtor(NameTable* nametable);

#endif // BINARY_TREE_H