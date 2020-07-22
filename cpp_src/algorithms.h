#ifndef UD_ALGORITHMS_H
#define UD_ALGORITHMS_H

#include "Tree.h"

namespace algorithms {
    char const* hello();

    int convSubTreeKernel(Node* root1, Node* root2);
    int convTreeKernel(Tree* t1, Tree* t2);
}

#endif