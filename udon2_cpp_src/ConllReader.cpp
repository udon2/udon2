/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "ConllReader.h"  // <string> is included via this header

#include <fstream>
#include <iostream>

#include "Util.h"

Node *ConllReader::initNodes(std::vector<std::string> words) {
  int N = words.size();
  Node *nodes[N + 1];  // with a root

  for (int i = 0; i < N + 1; i++) {
    nodes[i] = new Node();
  }

  for (int i = 1; i < N + 1; i++) {
    std::vector<std::string> word = Util::stringSplit(words[i - 1], '\t');
    if (word.size() != 10) {
      word = Util::stringSplit(words[i - 1], ' ');
      if (word.size() != 10) {
        perror("Failed to split a line of the CONLL-U format");
        exit(1);
      }
    }
    // -1 here because words are started indexing with 0 being the dummy root
    // word
    int dephead = std::__cxx11::stoi(word[6].c_str());
    float id = std::__cxx11::stof(word[0].c_str());
    nodes[i]->init(id, word[1], word[2], word[3], word[5], word[7],
                   nodes[dephead]);
    // nodes[dephead]->addChild(nodes[i]);
  }

  return nodes[0];  // root pseudonode
}

NodeList ConllReader::readFile(std::string fname) {
  std::ifstream conllFile(fname);
  std::string line;

  NodeList forest;
  std::vector<std::string> words;

  if (conllFile.is_open()) {
    while (std::getline(conllFile, line)) {
      if (line.empty()) {
        // end of sentence
        if (words.size() > 0) {
          forest.push_back(initNodes(words));
          words.clear();
        }
      } else {
        words.push_back(line);
      }
    }

    if (words.size() > 0) {
      forest.push_back(initNodes(words));
      words.clear();
    }
  }

  conllFile.close();
  return forest;
}
