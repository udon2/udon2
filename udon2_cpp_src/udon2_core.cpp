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

  bp::class_<Util::FeatMap>("FeatMap").def(
      bp::map_indexing_suite_v2<Util::FeatMap>());

  bp::class_<GroupedNodes>("GroupedNodes")
      .def(bp::map_indexing_suite_v2<GroupedNodes>());

  bp::class_<NodeList>("NodeList").def(bp::vector_indexing_suite<NodeList>());

  bp::class_<TreeList>("TreeList")
      .def(bp::vector_indexing_suite<TreeList>())
      .def("__del__", &TreeList::freeMemory);

  bp::class_<MultiWordNode, MultiWordNode *>("MultiWordNode",
                                             bp::init<int, int, std::string>())
      .def("get_min_id", &MultiWordNode::getMinId)
      .def("get_max_id", &MultiWordNode::getMaxId)
      .def("get_token", &MultiWordNode::getToken)
      .def("__str__", &MultiWordNode::toString);

  bp::class_<Node, Node *>("Node")
      .def(bp::init<float, std::string, std::string, std::string, std::string,
                    std::string, std::string, std::string, Node *>())
      .add_property("id", &Node::getId, &Node::setId)
      // make_getter doesn't seem to work here, why?
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
      .add_property("misc", &Node::getMisc, &Node::setMisc)
      .add_property("feats", &Node::getFeats, &Node::setFeats)
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
      .def("get_feats_as_string", &Node::getFeatsAsString)
      .def("has_all_feats", &Node::hasAllFeats)
      .def("has_feat", &Node::hasFeat)
      .def("alter_feat", &Node::alterFeat)
      .def("copy_children", &Node::copyChildren)
      .def("add_child", &Node::addChild)
      .def("has_children", &Node::hasChildren)
      .def("get_subtree_nodes", &Node::getSubtreeNodes)
      .def("remove_child", &Node::removeChild)
      .def("is_root", &Node::isRoot)
      .def("subtree_size", &Node::subtreeSize)
      .def("select_by", &Node::selectBy)
      .def("get_by", &Node::getBy)
      .def("select_by_deprel_chain", &Node::selectByDeprelChain)
      .def("get_by_deprel_chain", &Node::getByDeprelChain)
      .def("select_having_feats", &Node::selectHavingFeats)
      .def("select_missing_feats", &Node::selectMissingFeats)
      .def("is_identical", &Node::isIdentical)
      .def("select_identical", &Node::selectIdentical)
      .def("select_identical_except", &Node::selectIdenticalExcept)
      .def("prop_exists", &Node::propExists)
      .def("child_has_prop", &Node::childHasProp)
      .def("linear", &Node::linear)
      .def("linear_sorted", &Node::linearSorted)
      .def("prune", &Node::prune)
      .def("copy", &Node::copy, bp::return_internal_reference<1>())
      .def("make_root", &Node::makeRoot)
      .def("__str__", &Node::toString)
      .def("textual_intersect", &Node::textualIntersect,
           bp::return_internal_reference<1>())
      .def("ignore", &Node::ignore, node_ignore_overloads())
      .def("ignore_subtree", &Node::ignoreSubtree,
           node_ignore_subtree_overloads())
      .def("reset", &Node::reset, node_reset_overloads())
      .def("reset_subtree", &Node::resetSubtree, node_reset_subtree_overloads())
      .def("hard_reset", &Node::hardReset)
      .def("hard_reset_subtree", &Node::hardResetSubtree)
      .def("is_ignored", &Node::isIgnored)
      .def("get_ignore_label", &Node::getIgnoreLabel)
      .def("group_by", &Node::groupBy);

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
