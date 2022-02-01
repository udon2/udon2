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
  int ref = 0;

 public:
  MultiWordNode(int minId, int maxId, std::string token);
  explicit MultiWordNode(MultiWordNode* mw);
  bool isReadyToDelete() { return ref <= 0; }
  void decRef() { ref--; }
  int getRef() { return ref; }
  int getMinId() { return minId; }
  int getMaxId() { return maxId; }
  std::string getToken() { return token; }

  std::string toString() {
    return token + "/" + std::to_string(minId) + ":" + std::to_string(maxId);
  }
};

#endif  // UDON2_CPP_SRC_MULTIWORDNODE_H_
