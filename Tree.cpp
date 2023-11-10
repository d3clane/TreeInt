#include <assert.h>
#include <ctype.h>

#include "Tree.h"
#include "Log.h"

static const TreeValueType POISON = 0xDEAD;

static void TreePrintPrefixFormat (TreeNodeType* node, FILE* outStream);
static void TreePrintInfixFormat  (TreeNodeType* node, FILE* outStream);
static void TreePrintPostfixFormat(TreeNodeType* node, FILE* outStream);

static void TreeNodeDtor(TreeNodeType* node);
static void TreeDtor    (TreeNodeType* node);

static void TreeNodeInit(TreeNodeType* node, int value, TreeNodeType* left, TreeNodeType* right);
static TreeNodeType* TreeReadPrefixFormat(FILE* inStream);

static void DotFileCreateNodes(TreeNodeType* node, FILE* outDotFile, int depth = 0);
static void TreeGraphicDump   (TreeNodeType* node, FILE* outDotFile, int depth = 0);

TreeErrors TreeCtor(TreeType* tree, size_t treeSize, TreeNodeType* root)
{
    assert(tree);

    if (root != nullptr)
    {
        tree->root = root;
        tree->size = treeSize;

        return TreeErrors::NO_ERR;
    }

    TreeNodeType* newRoot = nullptr;

    tree->root = newRoot;
    tree->size = 0;

    return TreeErrors::NO_ERR;
}

TreeErrors TreeDtor(TreeType* tree)
{
    assert(tree);

    TreeDtor(tree->root);
    tree->root = nullptr;
    tree->size = 0;

    return TreeErrors::NO_ERR;
}

static void TreeDtor(TreeNodeType* node)
{
    if (node == nullptr)
        return;
    
    TreeDtor(node->left);
    TreeDtor(node->right);

    TreeNodeDtor(node);
}

TreeErrors TreeNodeCtor(TreeNodeType** node, TreeValueType value, TreeNodeType* left, TreeNodeType* right)
{
    assert(node);

    *node = (TreeNodeType*) calloc(1, sizeof(**node));

    if (*node == nullptr)
        return TreeErrors::MEM_ERR;

    (*node)->value = value;
    (*node)->left  =  left;
    (*node)->right = right;

    return TreeErrors::NO_ERR;
}

static void TreeNodeDtor(TreeNodeType* node)
{
    assert(node);

    node->value = POISON;
    node->left  = nullptr;
    node->right = nullptr;

    free(node);
}

void TreePrintPrefixFormat(TreeType* tree, FILE* outStream)
{
    assert(tree);
    assert(outStream);
    
    TreePrintPrefixFormat(tree->root, outStream);
    fprintf(outStream, "\n");
}

void TreePrintInfixFormat(TreeType* tree, FILE* outStream)
{
    assert(tree);
    assert(outStream);
    
    TreePrintInfixFormat(tree->root, outStream);
    fprintf(outStream, "\n");
}

void TreePrintPostfixFormat(TreeType* tree, FILE* outStream)
{
    assert(tree);
    assert(outStream);
    
    TreePrintPostfixFormat(tree->root, outStream);
    fprintf(outStream, "\n");
}

static void TreePrintPrefixFormat(TreeNodeType* node, FILE* outStream)
{
    if (node == nullptr)
    {
        fprintf(outStream, "nil ");
        return;
    }

    fprintf(outStream, "(");
    //TODO: 
    fprintf(outStream, "%d ", node->value);
    TreePrintPrefixFormat(node->left, outStream);
    TreePrintPrefixFormat(node->right, outStream);

    fprintf(outStream, ")");
}

static void TreePrintInfixFormat(TreeNodeType* node, FILE* outStream)
{
    if (node == nullptr)
    {
        fprintf(outStream, "nil ");
        return;
    }

    //TODO: 
    fprintf(outStream, "(");

    TreePrintInfixFormat(node->left, outStream);
    fprintf(outStream, "%d ", node->value);
    TreePrintInfixFormat(node->right, outStream);

    fprintf(outStream, ")");
}

static void TreePrintPostfixFormat(TreeNodeType* node, FILE* outStream)
{
    if (node == nullptr)
    {
        fprintf(outStream, "nil ");
        return;
    }

    fprintf(outStream, "(");
    //TODO: 
    TreePrintPostfixFormat(node->left, outStream);
    TreePrintPostfixFormat(node->right, outStream);
    fprintf(outStream, "%d ", node->value);

    fprintf(outStream, ")");
}

//TODO: renaming
void TreeReadPrefixFormat(TreeType* tree, FILE* inStream)
{
    assert(tree);
    assert(inStream);

    tree->root = TreeReadPrefixFormat(inStream);
}

static TreeNodeType* TreeReadPrefixFormat(FILE* inStream)
{
    assert(inStream);

    int symbol = 0;
    while (true)
    {
        symbol = getc(inStream);

        if (!isspace(symbol) || symbol == EOF)
            break;
    }

    TreeNodeType* node = nullptr;

    if (symbol == '(')
    {
        int value = POISON;
        fscanf(inStream, "%d", &value);
        TreeNodeCtor(&node, value);
    }
    else
    {
        fscanf(inStream, "%*s"); //skipping nill
        return nullptr;
    }

    TreeNodeType* left  = TreeReadPrefixFormat(inStream);
    TreeNodeType* right = TreeReadPrefixFormat(inStream);

    while (getc(inStream) != ')');

    TreeNodeInit(node, node->value, left, right);

    return node;
}

static inline void TreeNodeInit(TreeNodeType* node, int value, TreeNodeType* left, 
                                                               TreeNodeType* right)
{
    assert(node);

    node->value = value;
    node->left  = left;
    node->right = right;
}

static inline void CreateImgInLogFile(const size_t imgIndex)
{
    static const size_t maxImgNameLength  = 64;
    static char imgName[maxImgNameLength] = "";
    snprintf(imgName, maxImgNameLength, "imgs/img_%zu_time_%s.png", imgIndex, __TIME__);

    static const size_t     maxCommandLength  = 128;
    static char commandName[maxCommandLength] = "";
    snprintf(commandName, maxCommandLength, "dot list.dot -T png -o %s", imgName);
    //TODO: fork + exec
    system(commandName);

    snprintf(commandName, maxCommandLength, "<img src = \"%s\">", imgName);    
    Log(commandName);
}

static inline void DotFileBegin(FILE* outDotFile)
{
    fprintf(outDotFile, "digraph G{\nrankdir=TB;\ngraph [bgcolor=\"#31353b\"];\n");
}

static inline void DotFileEnd(FILE* outDotFile)
{
    fprintf(outDotFile, "\n}\n");
}

void TreeGraphicDump(TreeType* tree)
{
    assert(tree);

    static const char* dotFileName = "list.dot";
    FILE* outDotFile = fopen(dotFileName, "w");

    if (outDotFile == nullptr)
        return;

    DotFileBegin(outDotFile);

    DotFileCreateNodes(tree->root, outDotFile);

    TreeGraphicDump(tree->root, outDotFile);

    DotFileEnd(outDotFile);

    fclose(outDotFile);

    static size_t imgIndex = 0;
    CreateImgInLogFile(imgIndex);
    imgIndex++;
}

static void DotFileCreateNodes(TreeNodeType* node, FILE* outDotFile, int depth)
{
    if (node == nullptr)
        return;
    
    fprintf(outDotFile, "node%p"
                        "[shape=Mrecord, style=filled, fillcolor=\"#7293ba\","
                        "label = \"{ value: %d |  { <left> left | <right> right } } \" ," 
                        "color = \"#008080\"];\n",
                        node, node->value); 

    DotFileCreateNodes(node->left,  outDotFile, depth + 1);
    DotFileCreateNodes(node->right, outDotFile, depth + 1);
}

static void TreeGraphicDump(TreeNodeType* node, FILE* outDotFile, int depth)
{
    if (node == nullptr)
    {
        fprintf(outDotFile, "\n");
        return;
    }
    
    fprintf(outDotFile, "node%p;\n", node);

    if (node->left != nullptr) fprintf(outDotFile, "node%p:<left>->", node);
    TreeGraphicDump(node->left, outDotFile, depth + 1);

    if (node->right != nullptr) fprintf(outDotFile, "node%p:<right>->");
    TreeGraphicDump(node->right, outDotFile, depth + 1);
}