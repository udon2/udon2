/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_IMPORTER_H_
#define UDON2_CPP_SRC_IMPORTER_H_

#include <string>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>

#include "Node.h"

class Importer {
 public:
  static TreeList* fromConllFile(std::string fname);
  static TreeList* fromStanza(boost::python::list l);
};

#endif  // UDON2_CPP_SRC_IMPORTER_H_
