/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include <vector>
#include <string>
#include "Node.h"

#ifndef UDON2_CPP_SRC_MULTIWORDNODE_H_
#define UDON2_CPP_SRC_MULTIWORDNODE_H_

class MultiWordNode {
  std::vector<Node *> nodes;
  std::string token;
  int minId;
  int maxId;

 public:
  MultiWordNode(int minId, int maxId, std::string token);
};

#endif  // UDON2_CPP_SRC_MULTIWORDNODE_H_
