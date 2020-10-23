/*
 * Copyright 2020 Dmytro Kalpakchi
 */

/**
 * \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "ConllReader.h"
#include "ConllWriter.h"
#include "Util.h"
#include "map_indexing_suite_v2.hpp"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(node_select_by_overloads, Node::selectBy,
                                       2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(node_select_having_overloads,
                                       Node::selectHaving, 2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(node_ignore_overloads, Node::ignore, 0,
                                       1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(node_reset_overloads, Node::reset, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(node_ignore_subtree_overloads,
                                       Node::ignoreSubtree, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(node_reset_subtree_overloads,
                                       Node::resetSubtree, 0, 1);

BOOST_PYTHON_MODULE(core) {
  namespace bp = boost::python;

  // specify that this module is actually a package
  bp::object package = bp::scope();
  package.attr("__path__") = "core";

  bp::class_<Util::FeatMap>("FeatMap")
      .def(bp::map_indexing_suite_v2<Util::FeatMap>())
      .def("__str__", &Util::stringifyFeatMap);

  bp::class_<GroupedNodes>("GroupedNodes")
      .def(bp::map_indexing_suite_v2<GroupedNodes>());

  bp::class_<NodeList>("NodeList")
      .def(bp::vector_indexing_suite<NodeList>())
      .def("__str__", &NodeList::toString);

  bp::class_<TreeList>("TreeList")
      .def(bp::vector_indexing_suite<TreeList>())
      .def("__del__", &TreeList::freeMemory);

  bp::class_<MultiWordNode, MultiWordNode *>("MultiWordNode",
                                             bp::init<int, int, std::string>())
      .def_readonly("min_id", &MultiWordNode::getMinId)
      .def_readonly("max_id", &MultiWordNode::getMaxId)
      .def_readonly("token", &MultiWordNode::getToken)
      .def("__str__", &MultiWordNode::toString);

  bp::class_<Node, Node *>("Node")
      .def(bp::init<float, std::string, std::string, std::string, std::string,
                    std::string, std::string, std::string, Node *>())
      .add_property("id", &Node::getId, &Node::setId)
      .add_property("form",
                    bp::make_function(
                        &Node::getForm,
                        bp::return_value_policy<bp::copy_const_reference>()),
                    &Node::setForm)
      .add_property("lemma",
                    bp::make_function(
                        &Node::getLemma,
                        bp::return_value_policy<bp::copy_const_reference>()),
                    &Node::setLemma)
      .add_property("xpos",
                    bp::make_function(
                        &Node::getXpos,
                        bp::return_value_policy<bp::copy_const_reference>()),
                    &Node::setXpos)
      .add_property("upos",
                    bp::make_function(
                        &Node::getUpos,
                        bp::return_value_policy<bp::copy_const_reference>()),
                    &Node::setUpos)
      .add_property("deprel",
                    bp::make_function(
                        &Node::getDeprel,
                        bp::return_value_policy<bp::copy_const_reference>()),
                    &Node::setDeprel)
      // make_getter doesn't seem to work here, why?
      .add_property(
          "misc",
          bp::make_function(&Node::getMisc, bp::return_internal_reference<1>()),
          &Node::setMisc)
      .add_property("feats",
                    bp::make_function(&Node::getFeats,
                                      bp::return_internal_reference<1>()),
                    &Node::setFeats)
      .add_property("children",
                    bp::make_function(&Node::getChildren,
                                      bp::return_internal_reference<1>()))
      .add_property("parent",
                    bp::make_function(&Node::getParent,
                                      bp::return_internal_reference<1>()),
                    &Node::setParent)
      .add_property("multi_word",
                    bp::make_function(&Node::getMultiWord,
                                      bp::return_internal_reference<1>()))
      .def("get_subtree_text", &Node::getSubtreeText)
      .def("has", &Node::has)
      .def("has_all", &Node::hasAll)
      .def("has_any", &Node::hasAny)
      .def("copy_children", &Node::copyChildren)  // no
      .def("add_child", &Node::addChild)
      .def("remove_child", &Node::removeChild)
      .def("is_root", &Node::isRoot)            // no
      .def("is_leaf", &Node::isLeaf)            // no
      .def("subtree_size", &Node::subtreeSize)  // no
      .def("select_by", &Node::selectBy, node_select_by_overloads())
      .def("get_by", &Node::getBy)
      .def("select_having", &Node::selectHaving, node_select_having_overloads())
      .def("select_by_deprel_chain", &Node::selectByDeprelChain)
      .def("get_by_deprel_chain", &Node::getByDeprelChain)
      .def("is_identical", &Node::isIdentical)                       // no
      .def("select_identical", &Node::selectIdentical)               // no
      .def("select_identical_except", &Node::selectIdenticalExcept)  // no
      .def("prop_exists", &Node::propExists)                         // no
      .def("child_has_prop", &Node::childHasProp)                    // no
      .def("linear", &Node::linear)
      .def("linear_sorted", &Node::linearSorted)
      .def("prune", &Node::prune)                                    // no
      .def("copy", &Node::copy, bp::return_internal_reference<1>())  // no
      .def("make_root", &Node::makeRoot)                             // no
      .def("__str__", &Node::toString)
      .def("textual_intersect", &Node::textualIntersect,
           bp::return_internal_reference<1>())                // no
      .def("ignore", &Node::ignore, node_ignore_overloads())  // no
      .def("ignore_subtree", &Node::ignoreSubtree,
           node_ignore_subtree_overloads())                // no
      .def("reset", &Node::reset, node_reset_overloads())  // no
      .def("reset_subtree", &Node::resetSubtree,
           node_reset_subtree_overloads())                 // no
      .def("hard_reset", &Node::hardReset)                 // no
      .def("hard_reset_subtree", &Node::hardResetSubtree)  // no
      .def("is_ignored", &Node::isIgnored)                 // no
      .def("get_ignore_label", &Node::getIgnoreLabel)      // no
      .def("group_by", &Node::groupBy);                    // no

  bp::class_<ConllReader>("ConllReader")
      .def("read_file", &ConllReader::readFile)
      .staticmethod("read_file");

  void (*writeToFile_TreeList)(TreeList, std::string) =
      &ConllWriter::writeToFile;
  void (*writeToFile_Node)(Node *, std::string) = &ConllWriter::writeToFile;

  bp::class_<ConllWriter>("ConllWriter")
      .def("write_to_file", writeToFile_Node)
      .def("write_to_file", writeToFile_TreeList)
      .staticmethod("write_to_file");
}
