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

  bp::class_<NodeList>("NodeList", bp::init<>())
      .def("__getitem__", &NodeList::operator[],
           bp::return_internal_reference<1>())
      .def("__len__", &NodeList::size)
      .def("__iter__", bp::range(&NodeList::begin, &NodeList::end));

  bp::class_<MultiWordNode, MultiWordNode *>("MultiWordNode",
                                             bp::init<int, int, std::string>())
      .def("get_min_id", &MultiWordNode::getMinId)
      .def("get_max_id", &MultiWordNode::getMaxId)
      .def("get_token", &MultiWordNode::getToken)
      .def("__str__", &MultiWordNode::toString);

  bp::class_<Node, Node *>("Node")
      .def(bp::init<float, std::string, std::string, std::string, std::string,
                    std::string, std::string, std::string, Node *>())
      .def("set_id", &Node::setId)
      .def("set_form", &Node::setForm)
      .def("set_lemma", &Node::setLemma)
      .def("set_xpos", &Node::setXpos)
      .def("set_upos", &Node::setUpos)
      .def("set_deprel", &Node::setDeprel)
      .def("set_misc", &Node::setMisc)
      .def("set_parent", &Node::setParent)
      .def("get_id", &Node::getId)
      .def("get_form", &Node::getForm)
      .def("get_lemma", &Node::getLemma)
      .def("get_xpos", &Node::getXpos)
      .def("get_upos", &Node::getUpos)
      .def("get_deprel", &Node::getDeprel)
      .def("get_misc", &Node::getMisc)
      .def("get_parent", &Node::getParent, bp::return_internal_reference<1>())
      .def("get_multi_word", &Node::getMultiWord,
           bp::return_internal_reference<1>())
      .def("get_subtree_text", &Node::getSubtreeText)
      .def("get_feats", &Node::getFeats)
      .def("get_feats_as_string", &Node::getFeatsAsString)
      .def("has_all_feats", &Node::hasAllFeats)
      .def("has_feat", &Node::hasFeat)
      .def("set_feats", &Node::setFeats)
      .def("alter_feat", &Node::alterFeat)
      .def("get_children", &Node::getChildren)
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
      .def("group_by", &Node::groupBy)
      .def("to_grct", &Node::toGRCT, bp::return_internal_reference<1>())
      .def("to_pct", &Node::toPCT, bp::return_internal_reference<1>())
      .def("to_lct", &Node::toLCT, bp::return_internal_reference<1>());

  bp::class_<ConllReader>("ConllReader")
      .def("read_file", &ConllReader::readFile)
      .staticmethod("read_file");

  // bp::class_<ConllWriter>("ConllWriter")
  //     .def("write_to_file", &ConllWriter::writeToFile)
  //     .staticmethod("write_to_file");
}
