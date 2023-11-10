#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef int TreeValueType;

struct TreeNodeType
{
    TreeValueType value;

    TreeNodeType*  left;
    TreeNodeType* right;
};

struct TreeType
{
    TreeNodeType* root;
    
    size_t size;
};

enum class TreeErrors
{
    NO_ERR,

    MEM_ERR,
    
};

TreeErrors TreeCtor(TreeType* tree, size_t treeSize = 1, TreeNodeType* root = nullptr);
TreeErrors TreeDtor(TreeType* tree);

TreeErrors TreeNodeCtor(TreeNodeType** node, 
                        TreeValueType value, TreeNodeType* left  = nullptr, 
                                             TreeNodeType* right = nullptr);

void TreePrintPrefixFormat (TreeType* tree, FILE* outStream = stdout);
void TreePrintPostfixFormat(TreeType* tree, FILE* outStream = stdout); 
void TreePrintInfixFormat  (TreeType* tree, FILE* outStream = stdout); 

void TreeReadPrefixFormat(TreeType* tree, FILE* inStream = stdin);

void TreeGraphicDump(TreeType* tree);
#endif