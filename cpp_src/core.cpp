#include "ConllReader.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


BOOST_PYTHON_MODULE(core)
{
    using namespace boost::python;

    // specify that this module is actually a package
    object package = scope();
    package.attr("__path__") = "core";

    class_<FeatMap>("FeatMap")
        .def(map_indexing_suite<FeatMap>())
        ;

    class_<NodeList>("NodeList", init<>())
        .def("__getitem__", &NodeList::operator[],
            return_internal_reference<1>())
        .def("__len__", &NodeList::size)
        .def("__iter__", range(&NodeList::begin, &NodeList::end))
        ;

    class_<TreeList>("TreeList")
        .def(vector_indexing_suite<TreeList>())
        ;


    class_<Node, Node*>("Node")
        .def(init<int, std::string, std::string, std::string, std::string, std::string, Node*>())
        .def("get_text", &Node::getText)
        .def("get_subtree_text", &Node::getSubtreeText)
        .def("get_id", &Node::getId)
        .def("get_lemma", &Node::getLemma)
        .def("get_pos", &Node::getPos)
        .def("get_rel", &Node::getRel)
        .def("get_ufeats", &Node::getUfeats)
        .def("get_morph", &Node::getMorph)
        .def("get_parent", &Node::getParent,
            return_internal_reference<1>())
        .def("get_children", &Node::getChildren)
        .def("get_subtree_nodes", &Node::getSubtreeNodes)
        .def("subtree_size", &Node::subtreeSize)
        .def("copy_children", &Node::copyChildren)
        .def("add_child", &Node::addChild)
        .def("has_children", &Node::hasChildren)
        .def("select_by_pos", &Node::selectByPos)
        .def("select_by_lemma", &Node::selectByLemma)
        .def("select_by_morph", &Node::selectByMorph)
        .def("select_by_rel", &Node::selectByRel)
        .def("select_by_text", &Node::selectByText)
        .def("has_all_morph", &Node::hasAllMorph)
        .def("select_having_morph", &Node::selectHavingMorph)
        .def("select_missing_morph", &Node::selectMissingMorph)
        .def("select_except_pos", &Node::selectExceptPos)
        .def("select_except_lemma", &Node::selectExceptLemma)
        .def("select_except_rel", &Node::selectExceptRel)
        .def("select_except_text", &Node::selectExceptText)
        .def("is_identical", &Node::isIdentical)
        .def("select_identical", &Node::selectIdentical)
        .def("select_identical_except", &Node::selectIdenticalExcept)
        .def("immediate_by_pos", &Node::immediateByPos)
        .def("immediate_by_lemma", &Node::immediateByLemma)
        .def("immediate_by_rel", &Node::immediateByRel)
        .def("pos_exists", &Node::posExists)
        .def("lemma_exists", &Node::lemmaExists)
        .def("rel_exists", &Node::relExists)
        .def("text_exists", &Node::textExists)
        .def("child_has_pos", &Node::childHasPos)
        .def("child_has_lemma", &Node::childHasLemma)
        .def("child_has_rel", &Node::childHasRel)
        .def("child_has_text", &Node::childHasText)
        .def("linear", &Node::linear)
        .def("linear_sorted", &Node::linearSorted)
        .def("prune", &Node::prune)
        .def("remove_child", &Node::removeChild)
        .def("copy", &Node::copy,
            return_internal_reference<1>())
        .def("make_root", &Node::makeRoot)
        .def("__str__", &Node::toString)
        .def("textual_intersect", &Node::textualIntersect,
            return_internal_reference<1>())
        .def("ignore", &Node::ignore)
        .def("reset", &Node::reset)
        .def("reset_subtree", &Node::resetSubtree)
        .def("get_by_rel_chain", &Node::getByRelChain,
            return_internal_reference<1>())
        ;

    class_<Tree, Tree*>("Tree")
        .def("init_nodes", &Tree::initNodes)
        .def("get_root", &Tree::getRoot,
            return_internal_reference<1>()) // the first argument should be `this` ?
        .def("get_root_word", &Tree::getRootWord,
            return_internal_reference<1>())
        .def("__str__", &Tree::toString)
        ;

    class_<ConllReader>("ConllReader")
        .def("read_file", &ConllReader::readFile)
        .staticmethod("read_file")
        ;

}