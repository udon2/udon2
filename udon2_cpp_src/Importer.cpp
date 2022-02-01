/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include "Importer.h"  // <string> is included via this header
#include "ConllReader.h"
#include <memory>

#include <boost/python/extract.hpp>

TreeList* Importer::fromConllFile(std::string fname) {
  return ConllReader::readFile(fname);
}

// TODO(dmytro): Stanza now produces MWT sometimes - make sure to include those
// as well: Example:
// {
//   "id": [
//     8,
//     9
//   ],
//   "text": "Enron's",
//   "start_char": 42,
//   "end_char": 49
// }
TreeList* Importer::fromStanza(boost::python::list lst) {
  TreeList* forest = new TreeList();

  boost::python::ssize_t n = boost::python::len(lst);
  for (boost::python::ssize_t i = 0; i < n; i++) {
    boost::python::list words = (boost::python::list)lst[i];
    boost::python::ssize_t nwords = boost::python::len(words);

    Node** nodes = new Node*[nwords + 1];  // with a root
    for (int k = 0; k < nwords + 1; k++) {
      nodes[k] = new Node();
    }

    for (boost::python::ssize_t j = 0; j < nwords; j++) {
      boost::python::dict word = (boost::python::dict)words[j];

      boost::python::extract<std::string> text(word.get("text", ""));
      boost::python::extract<std::string> lemma(word.get("lemma", ""));
      boost::python::extract<std::string> upos(word.get("upos", ""));
      boost::python::extract<std::string> xpos(word.get("xpos", ""));
      boost::python::extract<std::string> feats(word.get("feats", ""));
      boost::python::extract<std::string> deprel(word.get("deprel", ""));
      boost::python::extract<std::string> misc(word.get("misc", ""));
      boost::python::extract<int> id(word.get("id"));
      boost::python::extract<int> dephead(word.get("head"));

      nodes[j + 1]->init(id(),               // id
                         text(),             // form
                         lemma(),            // lemma
                         upos(),             // upos
                         xpos(),             // xpos
                         feats(),            // feats
                         deprel(),           // deprel
                         misc(),             // misc
                         nodes[dephead()]);  // dephead
    }

    std::shared_ptr<Node> root(nodes[0]);
    // Add when multi-word is added
    // for (int k = j; k < nwords + 1; k++) delete nodes[k];
    delete[] nodes;
    forest->push_back(root);
  }
  return forest;
}
