/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include "helpers.h"
#include "Util.h"
#include <vector>

namespace helpers {
Node *findCommonAncestor(Node *n1, Node *n2) {
  Node *n1p = n1, *n2p = n2;

  while (n1p->getParent() != n2 && n2p->getParent() != n1 &&
         n1p->getParent() != n2p->getParent() && !n1p->getParent()->isRoot() &&
         !n2p->getParent()->isRoot()) {
    n1p = n1p->getParent();
    n2p = n2p->getParent();
  }

  Node *ca = NULL;
  if (n1p->getParent() == n2) {
    ca = n2;
  } else if (n2p->getParent() == n1) {
    ca = n1;
  } else if (n2p->getParent()->isRoot()) {
    ca = n2p;
  } else if (n1p->getParent()->isRoot()) {
    ca = n1p;
  } else if (n1p->getParent() == n2p->getParent()) {
    ca = n1p->getParent();
  }
  return ca;
}

Chain getDeprelChain(Node *n1, Node *n2) {
  // the chain will be formed only if both nodes are in the same subtree,
  // hence the common ancestor of these nodes should be one of the nodes
  // themselves
  Node *n1p = n1, *n2p = n2;

  std::vector<std::string> n1Chain, n2Chain;
  n1Chain.push_back(n1p->getDeprel());
  n2Chain.push_back(n2p->getDeprel());

  while (n1p->getParent() != n2 && n2p->getParent() != n1 &&
         n1p->getParent() != n2p->getParent()) {
    if (!n1p->getParent()->isRoot()) {
      n1p = n1p->getParent();
      n1Chain.push_back(n1p->getDeprel());
    }

    if (!n2p->getParent()->isRoot()) {
      n2p = n2p->getParent();
      n2Chain.push_back(n2p->getDeprel());
    }
  }

  Chain ci;
  if (n1p->getParent() == n2 || n1p == n2) {
    std::reverse(n1Chain.begin(), n1Chain.end());
    // 2 -> 1
    ci.reverse = Util::stringJoin(n1Chain, ".");
  } else if (n2p->getParent() == n1 || n2p == n1) {
    std::reverse(n2Chain.begin(), n2Chain.end());
    // 1 -> 2
    ci.direct = Util::stringJoin(n2Chain, ".");
  }
  return ci;
}
}  // namespace helpers
