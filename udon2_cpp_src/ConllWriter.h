/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_CONLLWRITER_H_
#define UDON2_CPP_SRC_CONLLWRITER_H_

#include <string>

#include "Node.h"
#include "MultiWordNode.h"

class ConllWriter {
  static std::string node2conllu(Node* node);
  static std::string node2conllu(MultiWordNode* node);

 public:
  static void writeToFile(TreeList* nodes, std::string fname);
  static void writeToFile(Node* node, std::string fname);
};

#endif  // UDON2_CPP_SRC_CONLLWRITER_H_
