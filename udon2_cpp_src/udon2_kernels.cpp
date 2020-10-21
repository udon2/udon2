/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include <boost/python.hpp>

#include "kernels.h"

BOOST_PYTHON_MODULE(kernels) {
  namespace bp = boost::python;

  bp::class_<kernels::ConvPartialTreeKernel>(
      "ConvPartialTreeKernel", bp::init<std::string, float, float>())
      .def("__call__", &kernels::ConvPartialTreeKernel::eval);
}
