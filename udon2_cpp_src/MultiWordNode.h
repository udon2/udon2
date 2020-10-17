/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include <string>

#ifndef UDON2_CPP_SRC_MULTIWORDNODE_H_
#define UDON2_CPP_SRC_MULTIWORDNODE_H_

class MultiWordNode {
  int minId;
  int maxId;
  std::string token;

 public:
  MultiWordNode(int minId, int maxId, std::string token);
  int getMinId() { return minId; }
  int getMaxId() { return maxId; }
  std::string getToken() { return token; }

  std::string toString() {
    return token + "/" + std::to_string(minId) + ":" + std::to_string(maxId);
  }
};

#endif  // UDON2_CPP_SRC_MULTIWORDNODE_H_
