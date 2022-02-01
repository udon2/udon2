/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include <boost/python.hpp>

#include "kernels.h"

BOOST_PYTHON_MODULE(kernels) {
  namespace bp = boost::python;

  bp::class_<kernels::ConvPartialTreeKernel>(
      "ConvPartialTreeKernel",
      bp::init<std::string, float, float, bool, bool>(
          (bp::arg("repr"), bp::arg("p_mu") = 1.0, bp::arg("p_lambda") = 1.0,
           bp::arg("includeForm") = true, bp::arg("includeFeats") = false)))
      .def("__call__", &kernels::ConvPartialTreeKernel::eval);
}
