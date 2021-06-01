/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include "ConllReader.h"  // <string> is included via this header

#include <math.h>

#include <fstream>
#include <iostream>

#include "MultiWordNode.h"
#include "Util.h"

std::shared_ptr<Node> ConllReader::initNodes(std::vector<std::string> words) {
  int N = words.size();
  // To avoid C2131 on Windows
  Node** nodes = new Node*[N + 1];  // with a root
  std::vector<MultiWordNode*> mwNodes;

  for (int i = 0; i < N + 1; i++) {
    nodes[i] = new Node();
  }

  int j = 1;  // root pseudonode is accounted for already
  for (int i = 1; i < N + 1; i++) {
    std::vector<std::string> word = Util::stringSplit(words[i - 1], '\t');
    if (word.size() != 10) {
      word = Util::stringSplit(words[i - 1]);
      if (word.size() != 10) {
        // TODO(dmytro): better error message
        perror("Failed to split a line of the CONLL-U format");
        exit(1);
      }
    }

    std::size_t idx = word[0].find('-');
    if (idx == std::string::npos) {
      float id = std::stof(word[0].c_str());

      if (ceil(id) != id) {
        // it's an empty node then - ignore as those are typically not provided
        // by the dependency parsers
        continue;
      }

      int dephead = std::stoi(word[6].c_str());
      nodes[j]->init(id,               // id
                     word[1],          // form
                     word[2],          // lemma
                     word[3],          // upos
                     word[4],          // xpos
                     word[5],          // feats
                     word[7],          // deprel
                     word[9],          // misc
                     nodes[dephead]);  // dephead
      j++;
    } else {
      // a multi-word expression
      int minId = std::stoi(word[0].substr(0, idx));
      int maxId = std::stoi(word[0].substr(idx + 1));
      mwNodes.push_back(new MultiWordNode(minId, maxId, word[1]));
    }
  }

  for (MultiWordNode* mwNode : mwNodes) {
    for (int i = mwNode->getMinId(); i <= mwNode->getMaxId(); i++) {
      nodes[i]->setMultiWord(mwNode);
    }
  }

  std::shared_ptr<Node> root(nodes[0]);
  for (int i = j; i < N + 1; i++) delete nodes[i];
  delete[] nodes;
  return root;  // root pseudonode
}

TreeList* ConllReader::readFile(std::string fname) {
  /**
   * Reads a file `fname` in a [CoNLL-U
   * format](https://universaldependencies.org/format.html).
   *
   * CoNLL-U comments are currently ignored as well as DEPS (enhanced dependency
   * graph in the form of a list of head-deprel pairs), because many of the
   * currently available dependency parsers do not support DEPS either.
   *
   * [Multi-word
   * tokens](https://universaldependencies.org/format.html#words-tokens-and-empty-nodes)
   * are supported, wheres empty nodes are currently not.
   */
  std::ifstream conllFile(fname);
  std::string line;

  TreeList* forest = new TreeList();
  std::vector<std::string> words;

  if (conllFile.is_open()) {
    while (std::getline(conllFile, line)) {
      if (line.empty()) {
        // end of sentence
        if (words.size() > 0) {
          // std::cout << "Hello" << std::endl;
          forest->push_back(initNodes(words));
          // std::cout << "world" << std::endl;
          words.clear();
        }
      } else if (line[0] == '#') {
        // comments, skip for now
        continue;
      } else {
        words.push_back(line);
      }
    }

    if (words.size() > 0) {
      forest->push_back(initNodes(words));
      words.clear();
    }
  }

  conllFile.close();
  return forest;
}
