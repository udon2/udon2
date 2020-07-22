#ifndef UD_CONLLREADER_H
#define UD_CONLLREADER_H

#include <string>
#include <vector>
#include "Tree.h"

typedef std::vector<Tree*> TreeList;


class ConllReader {
public:
    static TreeList readFile(std::string fname);
};

#endif