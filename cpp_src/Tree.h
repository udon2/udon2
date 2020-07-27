#ifndef UD_TREE_H
#define UD_TREE_H

#include "Util.h"        // <vector> is included via this header
#include "Node.h"


// Represents sentence
class Tree {
    Node* root;
public:
    Tree();
    void initNodes(std::vector<std::string> words);
    Node* getRoot();
    Node* getRootWord();
    std::string toString();
};

#endif