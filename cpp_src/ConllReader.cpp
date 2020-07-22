#include <iostream>
#include <fstream>
#include "ConllReader.h" // <string> is included via this header

using namespace std;


TreeList ConllReader::readFile(string fname) {
    ifstream conllFile(fname);
    string line;

    TreeList forest;
    vector<string> words;
    Tree* tree = new Tree();

    if (conllFile.is_open()) {
        while (getline(conllFile, line)) {
            if (line.empty()) { // TODO: maybe strip strings in future as well
                // end of sentence
                if (words.size() > 0) {
                    tree->initNodes(words);
                    forest.push_back(tree);
                    tree = new Tree();
                    words.clear();
                }
            } else {
                words.push_back(line);
            }
        }
    }

    conllFile.close();

    if (words.size() == 0) {
        delete tree;
        tree = NULL;
    }
    
    return forest;
}