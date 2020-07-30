#ifndef UD_CONLLREADER_H
#define UD_CONLLREADER_H

#include <string>
#include <vector>
#include "Node.h"


class ConllReader {
    static Node* initNodes(std::vector<std::string> words);
public:
    static NodeList readFile(std::string fname);
};

#endif