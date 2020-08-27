#include <iostream>
#include <fstream>
#include "Util.h"
#include "ConllReader.h" // <string> is included via this header

using namespace std;

Node* ConllReader::initNodes(vector<string> words) {
    int N = words.size();
    Node* nodes[N + 1]; // with a root

    for (int i = 0; i < N + 1; i++) {
        nodes[i] = new Node();
    }

    // TODO: add children
    for (int i = 1; i < N + 1; i++) {
        vector<string> word = Util::stringSplit(words[i - 1], '\t');
        if (word.size() != 10) {
            word = Util::stringSplit(words[i - 1], ' ');
            if (word.size() != 10) {
                perror("Failed to split a line of the CONLL-U format");
                exit(1);
            }
        }
        // -1 here because words are started indexing with 0 being the dummy root word
        int dephead = stoi(word[6].c_str());
        float id = stof(word[0].c_str());
        nodes[i]->init(id, word[1], word[2], word[3], word[5], word[7], nodes[dephead]);
        // nodes[dephead]->addChild(nodes[i]);
    }
    return nodes[0]; // root pseudonode
}


NodeList ConllReader::readFile(string fname) {
    ifstream conllFile(fname);
    string line;

    NodeList forest;
    vector<string> words;

    if (conllFile.is_open()) {
        while (getline(conllFile, line)) {
            if (line.empty()) { // TODO: maybe strip strings in future as well
                // end of sentence
                if (words.size() > 0) {
                    forest.push_back(initNodes(words));
                    words.clear();
                }
            } else {
                words.push_back(line);
            }
        }
    }

    conllFile.close();
    return forest;
}