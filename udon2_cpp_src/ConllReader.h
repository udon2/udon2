/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_CONLLREADER_H_
#define UDON2_CPP_SRC_CONLLREADER_H_

#include <string>
#include <vector>
#include <memory>

#include "Node.h"

class ConllReader {
  static std::shared_ptr<Node> initNodes(std::vector<std::string> words);

 public:
  static TreeList* readFile(std::string fname);
};

#endif  // UDON2_CPP_SRC_CONLLREADER_H_
