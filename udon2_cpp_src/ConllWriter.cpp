/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "ConllWriter.h"  // <string> is included via this header

#include <math.h>

#include <vector>
#include <fstream>
#include <iostream>

#include "Util.h"

std::string ConllWriter::node2conllu(Node* node) {
  int N = 10;
  // To avoid C2131 on Windows
  std::vector<std::string> nodeStr;
  nodeStr.reserve(N);
  char buffer[10];
  float id = node->getId();
  if (ceil(id) == id) {
    std::snprintf(buffer, sizeof(buffer), "%.0f", id);
  } else {
    std::snprintf(buffer, sizeof(buffer), "%.1f", id);
  }
  std::string res(buffer);
  nodeStr.push_back(res);
  nodeStr.push_back(node->getForm());
  nodeStr.push_back(node->getLemma());
  nodeStr.push_back(node->getUpos());
  nodeStr.push_back(node->getXpos());
  Util::FeatMap feats = node->getFeats();
  nodeStr.push_back(feats.empty() ? "_" : Util::stringifyFeatMap(feats));
  Node* parent = node->getParent();
  if (parent == NULL) {
    nodeStr.push_back("_");
  } else {
    id = parent->getId();
    if (ceil(id) == id) {
      std::snprintf(buffer, sizeof(buffer), "%.0f", id);
    } else {
      std::snprintf(buffer, sizeof(buffer), "%.1f", id);
    }
    std::string resParent(buffer);
    nodeStr.push_back(resParent);
  }
  nodeStr.push_back(node->getDeprel());
  nodeStr.push_back("_");
  Util::FeatMap misc = node->getMisc();
  nodeStr.push_back(misc.empty() ? "_" : Util::stringifyFeatMap(misc));
  return Util::stringJoin(nodeStr, "\t");
}

std::string ConllWriter::node2conllu(MultiWordNode* node) {
  int N = 10;
  // To avoid C2131 on Windows
  std::string* nodeStr = new std::string[N];
  nodeStr[0] =
      std::to_string(node->getMinId()) + "-" + std::to_string(node->getMaxId());
  nodeStr[1] = node->getToken();
  for (int i = 2; i < N; i++) nodeStr[i] = "_";
  std::string nodeRepr = Util::stringJoin(nodeStr, N, '\t');
  delete[] nodeStr;
  return nodeRepr;
}

// TODO(dmytro): check whether multi-word nodes are written
void ConllWriter::writeToFile(TreeList* nodes, std::string fname) {
  /**
   * Write the subtree rooted at every Node from the list `nodes` to the file
   * `fname` in a [CoNLL-U
   * format](https://universaldependencies.org/format.html).
   *
   * [Multi-word
   * tokens](https://universaldependencies.org/format.html#words-tokens-and-empty-nodes)
   * are supported, wheres empty nodes are currently not.
   */
  std::ofstream outFile;
  outFile.open(fname);
  size_t len = nodes->size();
  for (size_t i = 0; i < len; i++) {
    NodeList linear = (*nodes)[i]->linear();
    int sz = linear.size();
    for (int j = 1; j < sz; j++) {
      MultiWordNode* mw = linear[j]->getMultiWord();
      if (mw != NULL && j == mw->getMinId()) {
        outFile << ConllWriter::node2conllu(mw) << std::endl;
      }
      outFile << ConllWriter::node2conllu(linear[j]) << std::endl;
    }
    outFile << std::endl;
  }
  outFile.close();
}

void ConllWriter::writeToFile(Node* node, std::string fname) {
  /**
   * Write the subtree rooted at a Node `node` to the file `fname` in a [CoNLL-U
   * format](https://universaldependencies.org/format.html).
   *
   * [Multi-word
   * tokens](https://universaldependencies.org/format.html#words-tokens-and-empty-nodes)
   * are supported, wheres empty nodes are currently not.
   */
  std::ofstream outFile;
  outFile.open(fname);
  NodeList linear = node->linear();
  int sz = linear.size();
  for (int j = 1; j < sz; j++) {
    MultiWordNode* mw = linear[j]->getMultiWord();
    if (mw != NULL && j == mw->getMinId()) {
      outFile << ConllWriter::node2conllu(mw) << std::endl;
    }
    outFile << ConllWriter::node2conllu(linear[j]) << std::endl;
  }
  outFile << std::endl;
  outFile.close();
}
