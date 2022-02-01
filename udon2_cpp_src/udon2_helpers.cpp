/*
 * Copyright 2022 Dmytro Kalpakchi
 */

#include <boost/python.hpp>

#include "helpers.h"

BOOST_PYTHON_MODULE(helpers) {
  namespace bp = boost::python;

  bp::class_<helpers::Chain>("Chain")
      .add_property("direct", &helpers::Chain::direct)
      .add_property("reverse", &helpers::Chain::reverse);

  bp::def("find_common_ancestor", &helpers::findCommonAncestor,
          (bp::arg("n1"), bp::arg("n2")), bp::return_internal_reference<1>());
  bp::def("get_deprel_chain", &helpers::getDeprelChain,
          (bp::arg("n1"), bp::arg("n2")));
}
