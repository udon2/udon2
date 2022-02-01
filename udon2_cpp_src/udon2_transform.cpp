/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include <boost/python.hpp>

#include "transformations.h"

// BOOST_PYTHON_FUNCTION_OVERLOADS(transform_to_pct_overloads,
//                                 transformations::toPCT, 1, 3);
// BOOST_PYTHON_FUNCTION_OVERLOADS(transform_to_grct_overloads,
//                                 transformations::toGRCT, 1, 3);
// BOOST_PYTHON_FUNCTION_OVERLOADS(transform_to_lct_overloads,
//                                 transformations::toLCT, 1, 2);

BOOST_PYTHON_MODULE(transform) {
  namespace bp = boost::python;

  // Example from here:
  // https://www.boost.org/doc/libs/1_68_0/libs/python/doc/html/reference/function_invocation_and_creation/boost_python_overloads_hpp.html
  // bp::def("to_pct", &transformations::toPCT,
  //         transform_to_pct_overloads()[bp::return_internal_reference<1>()]);
  // bp::def("to_grct", &transformations::toGRCT,
  //         transform_to_grct_overloads()[bp::return_internal_reference<1>()]);
  // bp::def("to_lct", &transformations::toLCT,
  //         transform_to_lct_overloads()[bp::return_internal_reference<1>()]);

  bp::def("to_pct", &transformations::toPCT,
          (bp::arg("node"), bp::arg("includeForm") = true,
           bp::arg("includeFeats") = false),
          bp::return_internal_reference<1>());
  bp::def("to_grct", &transformations::toGRCT,
          (bp::arg("node"), bp::arg("includeForm") = true,
           bp::arg("includeFeats") = false),
          bp::return_internal_reference<1>());
  bp::def("to_lct", &transformations::toLCT,
          (bp::arg("node"), bp::arg("includeFeats") = false),
          bp::return_internal_reference<1>());
  bp::def("distort", &transformations::distort,
          bp::return_value_policy<bp::manage_new_object>());
}
