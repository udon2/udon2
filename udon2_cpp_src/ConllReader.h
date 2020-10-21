/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_CONLLREADER_H_
#define UDON2_CPP_SRC_CONLLREADER_H_

#include <string>
#include <vector>

#include "Node.h"

class ConllReader {
  static Node *initNodes(std::vector<std::string> words);

 public:
  static TreeList readFile(std::string fname);
};

#endif  // UDON2_CPP_SRC_CONLLREADER_H_
