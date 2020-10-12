/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "ConllWriter.h"  // <string> is included via this header

#include <math.h>

#include <fstream>
#include <iostream>

#include "Util.h"

std::string ConllWriter::node2conllu(Node* node) {
  int N = 10;
  std::string nodeStr[N];
  char buffer[10];
  float id = node->getId();
  if (ceil(id) == id) {
    std::snprintf(buffer, sizeof(buffer), "%.0f", id);
  } else {
    std::snprintf(buffer, sizeof(buffer), "%.1f", id);
  }
  std::string res(buffer);
  nodeStr[0] = res;
  nodeStr[1] = node->getForm();
  nodeStr[2] = node->getLemma();
  nodeStr[3] = node->getUpos();
  nodeStr[4] = node->getXpos();
  Util::FeatMap feats = node->getFeats();
  nodeStr[5] = feats.empty() ? "_" : Util::stringifyFeatMap(feats);
  Node* parent = node->getParent();
  if (parent == NULL) {
    nodeStr[6] = "_";
  } else {
    id = parent->getId();
    if (ceil(id) == id) {
      std::snprintf(buffer, sizeof(buffer), "%.0f", id);
    } else {
      std::snprintf(buffer, sizeof(buffer), "%.1f", id);
    }
    std::string resParent(buffer);
    nodeStr[6] = resParent;
  }
  nodeStr[7] = node->getDeprel();
  nodeStr[8] = "_";
  Util::FeatMap misc = node->getMisc();
  nodeStr[9] = misc.empty() ? "_" : Util::stringifyFeatMap(misc);
  return Util::stringJoin(nodeStr, N, '\t');
}

std::string ConllWriter::node2conllu(MultiWordNode* node) {
  int N = 10;
  std::string nodeStr[N];
  nodeStr[0] =
      std::to_string(node->getMinId()) + "-" + std::to_string(node->getMaxId());
  nodeStr[1] = node->getToken();
  for (int i = 2; i < N; i++) nodeStr[i] = "_";
  return Util::stringJoin(nodeStr, N, '\t');
}

void ConllWriter::writeToFile(NodeList nodes, std::string fname) {
  std::ofstream outFile;
  outFile.open(fname);
  for (Node* node : nodes) {
    NodeList linear = node->linear();
    for (int j = 1; j < linear.size(); j++) {
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
  std::ofstream outFile;
  outFile.open(fname);
  NodeList linear = node->linear();
  for (int j = 1; j < linear.size(); j++) {
    MultiWordNode* mw = linear[j]->getMultiWord();
    if (mw != NULL && j == mw->getMinId()) {
      outFile << ConllWriter::node2conllu(mw) << std::endl;
    }
    outFile << ConllWriter::node2conllu(linear[j]) << std::endl;
  }
  outFile << std::endl;
  outFile.close();
}
