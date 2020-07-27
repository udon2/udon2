#include <iostream>
#include "Tree.h"

using namespace std;


Tree::Tree() {}

void Tree::initNodes(vector<string> words) {
    int N = words.size();
    Node* nodes[N + 1]; // with a root

    for (int i = 0; i < N + 1; i++) {
        nodes[i] = new Node();
    }
    root = nodes[0];

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
        int id = stoi(word[0].c_str());
        nodes[i]->init(id, word[1], word[2], word[3], word[5], word[7], nodes[dephead]);
        nodes[dephead]->addChild(nodes[i]);
    }
}

Node* Tree::getRoot() {
    // cout << root->getName() << endl;
    return root;
}

Node* Tree::getRootWord() {
    return root->getChildren()[0];
}

string Tree::toString() {
    return getRootWord()->getSubtreeText();
}