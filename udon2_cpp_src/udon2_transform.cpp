/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include <boost/python.hpp>

#include "transformations.h"

BOOST_PYTHON_MODULE(transform) {
  namespace bp = boost::python;

  def("to_pct", &transformations::toPCT, bp::return_internal_reference<1>());
  def("to_grct", &transformations::toGRCT, bp::return_internal_reference<1>());
  def("to_lct", &transformations::toLCT, bp::return_internal_reference<1>());
}
