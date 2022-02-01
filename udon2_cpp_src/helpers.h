/*
 * Copyright 2022 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_HELPERS_H_
#define UDON2_CPP_SRC_HELPERS_H_

#include <string>

#include "Node.h"

namespace helpers {
struct Chain {
  std::string direct = "";   // 1 -> 2
  std::string reverse = "";  // 2 -> 1
};

Node *findCommonAncestor(Node *n1, Node *n2);
Chain getDeprelChain(Node *n1, Node *n2);
}  // namespace helpers

#endif  // UDON2_CPP_SRC_HELPERS_H_
