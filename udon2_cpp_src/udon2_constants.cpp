/*
 * Copyright 2022 Dmytro Kalpakchi
 */

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// adapted from:
// https://mail.python.org/pipermail/cplusplus-sig/2005-July/008878.html
#define DEFINE_CONSTANT(N, X) boost::python::scope().attr(N) = X

#include "constants.h"
#include "map_indexing_suite_v2.hpp"

BOOST_PYTHON_MODULE(constants) {
  namespace bp = boost::python;

  bp::class_<constants::UniversalToken>("UniversalToken")
      .add_property("code", &constants::UniversalToken::code)
      .add_property("name", &constants::UniversalToken::name)
      .def("__str__", &constants::UniversalToken::toString);

  bp::class_<constants::UniversalMap>("UniversalMap")
      .def(bp::map_indexing_suite_v2<constants::UniversalMap>());

  bp::class_<constants::UposArray>("UposArray")
      .def("__getitem__", &constants::UposArray::operator[],
           bp::return_internal_reference<1>())
      .def("__len__", &constants::UposArray::size)
      .def("__iter__",
           bp::range(&constants::UposArray::begin, &constants::UposArray::end));

  bp::class_<constants::DeprelArray>("DeprelArray")
      .def("__getitem__", &constants::DeprelArray::operator[],
           bp::return_internal_reference<1>())
      .def("__len__", &constants::DeprelArray::size)
      .def("__iter__", bp::range(&constants::DeprelArray::begin,
                                 &constants::DeprelArray::end));

  bp::class_<constants::UfeatArray>("UfeatArray")
      .def("__getitem__", &constants::UfeatArray::operator[],
           bp::return_internal_reference<1>())
      .def("__len__", &constants::UfeatArray::size)
      .def("__iter__", bp::range(&constants::UfeatArray::begin,
                                 &constants::UfeatArray::end));

  bp::class_<constants::UniversalTokenList>("UniversalTokenList")
      .def(bp::vector_indexing_suite<constants::UniversalTokenList>());

  for (int i = 0; i < constants::NUM_UPOS_TAGS; i++) {
    constants::UniversalToken token = constants::UPOS_TAGS[i];
    std::string name = token.code + "_UPOS";

    DEFINE_CONSTANT(name.c_str(), token);
  }

  for (int i = 0; i < constants::NUM_DEPRELS; i++) {
    constants::UniversalToken deprel = constants::DEPRELS[i];
    std::string code = deprel.code;
    boost::to_upper(code);
    boost::replace_all(code, ":", "__");
    std::string name = code + "_DEPREL";

    DEFINE_CONSTANT(name.c_str(), deprel);
  }

  for (int i = 0; i < constants::NUM_UFEATS; i++) {
    constants::UniversalToken ufeat = constants::UFEATS[i];
    std::string code = ufeat.code;
    boost::to_upper(code);
    boost::replace_all(code, "[", "__");
    boost::replace_all(code, "]", "");
    std::string name = code + "_UFEAT";

    DEFINE_CONSTANT(name.c_str(), ufeat);
  }

  DEFINE_CONSTANT("UPOS_TAGS", constants::UPOS_TAGS);
  DEFINE_CONSTANT("DEPRELS", constants::DEPRELS);
  DEFINE_CONSTANT("FEATS", constants::FEATS);
}
