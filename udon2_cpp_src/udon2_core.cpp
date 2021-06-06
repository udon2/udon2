/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "ConllReader.h"
#include "ConllWriter.h"
#include "Importer.h"
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

  bp::class_<TreeList, TreeList *>("TreeList")
      .def(bp::vector_indexing_suite<TreeList, true>());

  bp::class_<MultiWordNode, MultiWordNode *>("MultiWordNode",
                                             bp::init<int, int, std::string>())
      .def_readonly("min_id", &MultiWordNode::getMinId)
      .def_readonly("max_id", &MultiWordNode::getMaxId)
      .def_readonly("token", &MultiWordNode::getToken)
      .def("__str__", &MultiWordNode::toString);

  bp::class_<Node, Node *, std::shared_ptr<Node>>("Node")
      .def(bp::init<float, std::string, std::string, std::string, std::string,
                    std::string, std::string, std::string, Node *>())
      .add_property("id", &Node::getId, &Node::setId)
      .add_property("form", &Node::getForm, &Node::setForm)
      .add_property("lemma", &Node::getLemma, &Node::setLemma)
      .add_property("xpos", &Node::getXpos, &Node::setXpos)
      .add_property("upos", &Node::getUpos, &Node::setUpos)
      .add_property("deprel", &Node::getDeprel, &Node::setDeprel)
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
      .def("prune", &Node::prune)  // no
      .def("copy", &Node::copy,
           bp::return_value_policy<bp::manage_new_object>())  // no
      .def("make_root", &Node::makeRoot,
           bp::return_value_policy<bp::manage_new_object>())  // no
      .def("__str__", &Node::toString)
      .def("to_charniak_string", &Node::toCharniakString)
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
      .def("group_by", &Node::groupBy)                     // no
      .def("is_projective", &Node::isProjective);

  bp::register_ptr_to_python<std::shared_ptr<Node>>();

  bp::class_<ConllReader>("ConllReader")
      .def("read_file", &ConllReader::readFile,
           bp::return_value_policy<bp::manage_new_object>())
      .staticmethod("read_file");

  void (*writeToFile_TreeList)(TreeList *, std::string) =
      &ConllWriter::writeToFile;
  void (*writeToFile_Node)(Node *, std::string) = &ConllWriter::writeToFile;

  bp::class_<ConllWriter>("ConllWriter")
      .def("write_to_file", writeToFile_Node)
      .def("write_to_file", writeToFile_TreeList)
      .staticmethod("write_to_file");

  bp::class_<Importer>("Importer")
      .def("from_conll_file", &Importer::fromConllFile,
           bp::return_value_policy<bp::manage_new_object>())
      .def("from_stanza", &Importer::fromStanza,
           bp::return_value_policy<bp::manage_new_object>())
      .staticmethod("from_conll_file")
      .staticmethod("from_stanza");
}
