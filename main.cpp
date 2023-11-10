#include "Tree.h"
#include "Log.h"

int main(const int argc, const char* argv[])
{
    LogOpen(argv[0]);

    TreeNodeType* node10, *node5, *node17, *node15, *node20, *node12, *node78, *node123, *node1337, *node228;
    TreeType tree;

    TreeNodeCtor(&node20, 20);
    TreeNodeCtor(&node228, 228);
    TreeNodeCtor(&node78, 78);
    TreeNodeCtor(&node123, 123, node228, node78);
    TreeNodeCtor(&node1337, 1337, node123, nullptr);
    
    TreeNodeCtor(&node12, 12, node1337);
    TreeNodeCtor(&node15, 15, node12);
    TreeNodeCtor(&node20, 20);
    TreeNodeCtor(&node17, 17, node15, node20);
    TreeNodeCtor(&node5, 5);
    TreeNodeCtor(&node10, 10, node5, node17);

    TreeCtor(&tree, 5, node10);

    tree.root = node10;

    TreePrintPrefixFormat (&tree);
    TreePrintPostfixFormat(&tree);
    TreePrintInfixFormat  (&tree);

    TreeGraphicDump(&tree);

    TreeDtor(&tree);

    TreeCtor(&tree);
    TreeReadPrefixFormat(&tree);

    TreePrintPrefixFormat (&tree);
    TreePrintPostfixFormat(&tree);
    TreePrintInfixFormat  (&tree);

    TreeGraphicDump(&tree);
}