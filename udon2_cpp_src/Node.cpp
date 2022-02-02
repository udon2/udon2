/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "Node.h"

#include <stdlib.h> /* srand, rand */

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

/*
 * NodeList methods
 */

NodeList::iterator NodeList::findById(Node *n) {
  return find_if(begin(), end(), compare_to_node_by_id(n));
}

std::string NodeList::toString() {
  std::string res = "";
  for (int i = 0, len = size(); i < len; i++) {
    res += at(i)->toString();
    if (i < len - 1) {
      res += " ";
    }
  }
  return res;
}

/*
 * TreeList methods
 */

/*
 * Node methods
 */
Node::Node()
    : id(0),
      form("root"),
      lemma("_"),
      upos("_"),
      xpos("_"),
      deprel("_"),
      parent(NULL) {}

Node::Node(float id, std::string form, std::string lemma, std::string upos,
           std::string xpos, std::string feats, std::string deprel,
           std::string misc, Node *parent)
    : id(id),
      form(form),
      lemma(lemma),
      upos(upos),
      xpos(xpos),
      deprel(deprel),
      parent(parent) {
  this->feats = Util::parseUniversalFormat(feats);
  this->misc = Util::parseUniversalFormat(misc);
  if (parent != NULL) parent->addChild(this);
}

Node::Node(Node *n, std::map<std::string, MultiWordNode *> *mw) {
  // Multi-word nodes are not copied, since this node is only a part of the
  // multi-word one
  this->id = n->getId();
  this->form = n->getForm();
  this->lemma = n->getLemma();
  this->upos = n->getUpos();
  this->xpos = n->getXpos();
  this->deprel = n->getDeprel();
  this->parent = n->getParent();
  this->feats = n->getFeats();
  this->misc = n->getMisc();
  this->copyChildren(n, mw);
  this->mIgnoreLabel = n->getIgnoreLabel();
}

Node *Node::copy() {
  std::map<std::string, MultiWordNode *> *mw =
      new std::map<std::string, MultiWordNode *>();
  Node *n = new Node(this, mw);
  delete mw;
  return n;
}

// void Node::freeMemory() {
//   if (mwNode != NULL && id == mwNode->getMaxId()) delete mwNode;
// }

Node::~Node() {
  // std::cout << "~Node -- " << form << std::endl;
  for (int i = 0, sz = this->children.size(); i < sz; i++)
    delete this->children[i];
  if (mwNode != NULL) {
    mwNode->decRef();
    // std::cout << mwNode->getMaxId() << " " << mwNode->isReadyToDelete() <<
    // std::endl;
    if (mwNode->isReadyToDelete()) {
      delete mwNode;
      mwNode = NULL;
    }
  }
}

void Node::init(float id, std::string form, std::string lemma, std::string upos,
                std::string xpos, std::string feats, std::string deprel,
                std::string misc, Node *parent) {
  this->id = id;
  this->form = form;
  this->lemma = lemma;
  this->upos = upos;
  this->xpos = xpos;
  this->deprel = deprel;
  this->parent = parent;
  this->misc = Util::parseUniversalFormat(misc);
  if (parent != NULL) parent->addChild(this);
  this->feats = Util::parseUniversalFormat(feats);
}

void Node::copyChildren(Node *node,
                        std::map<std::string, MultiWordNode *> *mw) {
  /**
   * Copy children of Node `node` to the current Node.
   */
  children = NodeList();
  for (Node *n : node->getChildren()) {
    children.push_back(new Node(n, mw));
    // std::cout << children[children.size() - 1]->toString() << std::endl;
    if (n->getMultiWord() != NULL) {
      std::string key = n->getMultiWord()->toString();
      if (mw->count(key) == 0)
        (*mw)[key] = new MultiWordNode(n->getMultiWord());
      children[children.size() - 1]->setMultiWord((*mw)[key]);
    }
  }
}

void Node::addChild(Node *node) {
  /**
   * Add `node` as a child of the current Node. Note that all children are
   * stored in a lexicographical order of Node's string representations, i.e.
   * UPOS|DEPREL|FORM
   */
  children.push_sorted(node, compare_node_by_string());
}

void Node::removeChild(Node *node) {
  std::vector<Node *>::iterator it =
      find(children.begin(), children.end(), node);
  if (it != children.end()) children.erase(it);
}

void Node::setParent(Node *n) {
  /**
   * Set a parent of a Node to be the Node `n`, allowing to change the
   * dependency structure of a tree. If the function called for the dependent of
   * the root pseudonode, the root is reassigned to be the parent of the node
   * `n`.
   */

  if (this->parent == NULL) {
    // a pseudoroot node
  } else if (this->parent->isRoot()) {
    // a dependent of a pseudoroot node
    // TODO(dmytro) maybe exchange the root relation with others,
    // since we know for sure that only root relation can be used
    // for arcs from root pseudonode
    Node *root = this->parent;
    this->parent = n;
    n->setParent(root);
  } else {
    this->parent = n;
  }
}

bool Node::has(std::string prop, std::string key, std::string value) {
  /**
   * Check if the Node's key-value property specified by `prop` (either "feats"
   * or "misc") contains a feature `key`=`value`.
   */
  kvgetterptr getterFn = kvgetterByProp(prop);
  if (getterFn == NULL) return false;

  Util::FeatMap nodeFeats = (this->*getterFn)();
  if (nodeFeats.count(key) > 0) {
    return nodeFeats[key] == value;
  } else {
    return false;
  }
}

bool Node::hasAll(std::string prop, std::string value) {
  /**
   * Check if the Node's key-value property specified by `prop` (either "feats"
   * or "misc") contains all features specified in `value`, e.g. \code{.cpp}
   * node->hasAll("feats", "Case=Nom|PronType=Prs")
   * \endcode
   *
   * Note that the check is not exclusive, e.g. consider the node with
   * FEATS equal to Case=Nom|Person=2|PronType=Prs, then the following
   * holds.
   * \code{.cpp}
   * node->hasAll("feats", "Case=Nom|Person=2") // returns true
   * node->hasAll("feats", "Case=Nom")          // returns true
   * node->hasAll("feats", "Case=Nom|Person=3") // returns false
   * \endcode
   */
  Util::FeatMap givenFeats = Util::parseUniversalFormat(value);

  kvgetterptr getterFn = kvgetterByProp(prop);
  if (getterFn == NULL) return false;

  Util::FeatMap nodeFeats = (this->*getterFn)();

  for (auto it = givenFeats.begin(); it != givenFeats.end(); it++) {
    if (nodeFeats.find(it->first) == nodeFeats.end() ||
        nodeFeats[it->first] != it->second) {
      return false;
    }
  }
  return true;
}

bool Node::hasAny(std::string prop, std::string value) {
  /**
   * Check if the Node's key-value property specified by `prop` (either "feats"
   * or "misc") contains any of the features specified in `value`, e.g.
   * \code{.cpp} node->hasAll("feats", "Case=Nom|PronType=Prs") \endcode
   */
  Util::FeatMap givenFeats = Util::parseUniversalFormat(value);

  kvgetterptr getterFn = kvgetterByProp(prop);
  if (getterFn == NULL) return false;

  Util::FeatMap nodeFeats = (this->*getterFn)();

  for (auto it = givenFeats.begin(); it != givenFeats.end(); it++) {
    if (nodeFeats.find(it->first) != nodeFeats.end() ||
        nodeFeats[it->first] == it->second) {
      return true;
    }
  }
  return false;
}

bool Node::isIgnored() {
  /**
   * Check if the node is ignored with any label
   */
  return mIgnoreLabel >= 0;
}

void Node::ignore(int label) {
  /**
   * Ignore the node during queries and subtree text generation and label it
   * with `label` (0 by default). The label can be used for hierarchical
   * ignoring in subprograms.
   */
  if (this->mIgnoreLabel < 0) {
    this->mIgnoreLabel = label >= 0 ? label : 0;
  }
}

void Node::reset(int label) {
  /**
   * Reset an ignored node labeled with `label` (0 by default) to be available
   * again for queries and subtree text generation. Resetting only a specific
   * label can be used for hierarchical ignoring in subprograms.
   */
  if (this->mIgnoreLabel == label || label < 0) {
    this->mIgnoreLabel = -1;
  }
}

void Node::hardReset() {
  /**
   * Reset an ignored node no matter which label it has.
   */
  reset(-1);
}

void Node::ignoreSubtree(int label) {
  /**
   * Ignore all nodes in the subtree induced by the current node during queries
   * and subtree text generation. All ignored nodes will be labeled with `label`
   * (0 by default).
   */
  this->ignore(label);
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // add children of the head node to the stack
    nodes.front()->ignore(label);

    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
}

void Node::resetSubtree(int label) {
  /**
   * Reset all ignored nodes in the subtree induced by the current node to be
   * available again during queries and subtree text generation. Only nodes
   * labeled with `label` (0 by default) will be reset.
   */
  this->reset(label);
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // add children of the head node to the stack
    nodes.front()->reset(label);

    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
}

void Node::hardResetSubtree() {
  /**
   * Reset all ignored nodes (no matter the label) in the subtree induced by the
   * current node to be available again during queries and subtree text
   * generation.
   */
  resetSubtree(-1);
}

int Node::_subtreeSize(Node *n) {
  NodeList &ch = n->getChildren();
  int len = ch.size();
  int sz = len;
  // std::cout << "S " << sz << " " << n->getForm() << std::endl;
  for (int i = 0; i < len; i++) {
    sz += _subtreeSize(ch[i]);
  }
  // std::cout << "E " << sz << std::endl;
  return sz;
}

int Node::subtreeSize() {
  /**
   * Get the number of nodes in the subtree induced by the current node.
   */
  return _subtreeSize(this);
}

std::string Node::getSubtreeText() {
  /**
   * Generate a text of the subtree induced by its node using the current FORM
   * values of all nodes in the subtree except the ignored nodes.
   */
  NodeList linear = this->linear();
  size_t sz = linear.size();

  std::ostringstream os;
  for (size_t j = 0; j < sz; j++) {
    if (linear[j]->isRoot() || linear[j]->isIgnored()) continue;
    MultiWordNode *mw = linear[j]->getMultiWord();
    if (mw != NULL) {
      if (linear[j]->getId() == mw->getMinId()) {
        os << mw->getToken();
      } else if (linear[j]->getId() == mw->getMaxId() && j < sz - 1) {
        Util::FeatMap misc = linear[j]->getMisc();
        if (misc.count("SpaceAfter") == 0 || misc["SpaceAfter"] != "No")
          os << " ";
      }
      continue;
    }

    Util::FeatMap misc = linear[j]->getMisc();
    os << linear[j]->getForm();
    if (j < sz - 1 &&
        (misc.count("SpaceAfter") == 0 || misc["SpaceAfter"] != "No"))
      os << " ";
  }
  return os.str();
}

template <class Compare>
void Node::_linear(Node *node, NodeList *nodes, Compare cmp) {
  NodeList &ch = node->getChildren();
  int len = ch.size();
  if (len > 0) {
    for (int i = 0; i < len; i++) {
      nodes->push_sorted(ch[i], cmp);
      _linear(ch[i], nodes, cmp);
    }
  }
}

NodeList Node::linear() {
  /**
   * Represent a subtree induced by the current node in a linear order, as they
   * appear in the original sentence.
   */
  NodeList lin;
  lin.reserve(this->subtreeSize());
  lin.push_back(this);
  _linear(this, &lin, compare_node_by_id());
  return lin;
}

NodeList Node::linearSorted() {
  /**
   * Represent a subtree induced by the current node in a linear order, sorted
   * lexicographically based on their string representation.
   */
  NodeList lin;
  lin.reserve(this->subtreeSize());
  lin.push_back(this);
  _linear(this, &lin, compare_node_by_string());
  return lin;
}

getterptr Node::getterByProp(std::string prop) {
  getterptr getterFn = NULL;
  if (prop == "upos")
    getterFn = &Node::getUpos;
  else if (prop == "xpos")
    getterFn = &Node::getXpos;
  else if (prop == "lemma")
    getterFn = &Node::getLemma;
  else if (prop == "deprel")
    getterFn = &Node::getDeprel;
  else if (prop == "form")
    getterFn = &Node::getForm;
  else if (prop == "id")
    getterFn = &Node::getIdAsString;
  return getterFn;
}

setterptr Node::setterByProp(std::string prop) {
  setterptr setterFn = NULL;
  if (prop == "upos")
    setterFn = &Node::setUpos;
  else if (prop == "xpos")
    setterFn = &Node::setXpos;
  else if (prop == "lemma")
    setterFn = &Node::setLemma;
  else if (prop == "deprel")
    setterFn = &Node::setDeprel;
  else if (prop == "form")
    setterFn = &Node::setForm;
  return setterFn;
}

kvgetterptr Node::kvgetterByProp(std::string prop) {
  kvgetterptr getterFn = NULL;
  if (prop == "feats")
    getterFn = &Node::getFeats;
  else if (prop == "misc")
    getterFn = &Node::getMisc;
  return getterFn;
}

NodeList Node::selectBy(std::string prop, std::string value, bool negate) {
  /**
   * Select all nodes from the subtree induced by the current node (including,
   * possibley, the node itself) having a property `prop` being equal to
   * `value`. If `negate` is true, then the exact opposite happens, i.e. only
   * nodes with a property `prop` being NOT equal to `value` will be selected.
   */
  // TODO(dmytro): benchmark if recursive is faster
  getterptr getterFn = Node::getterByProp(prop);
  if (getterFn == NULL) return NodeList();

  std::queue<Node *> nodes;
  NodeList result;

  std::string nodeValue = (this->*getterFn)();
  if ((!negate && nodeValue == value) || (negate && nodeValue != value)) {
    result.push_back(this);
  }

  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    if (!nodes.front()->isIgnored()) {
      std::string nodeValue = (nodes.front()->*getterFn)();

      if ((!negate && nodeValue == value) || (negate && nodeValue != value)) {
        result.push_back(nodes.front());
      }

      // add children of the head node to the stack
      NodeList &ch = nodes.front()->getChildren();
      for (int i = 0, len = ch.size(); i < len; i++) {
        nodes.push(ch[i]);
      }
    }
    nodes.pop();
  }
  return result;
}

void Node::accumulateByDeprelChain(std::string value, NodeList *res,
                                   int depth) {
  if (value.empty()) return;

  std::vector<std::string> chain = Util::stringSplit(value, '.');

  NodeList list = depth > 0 ? children : selectBy("deprel", chain[0], false);

  std::vector<Node *>::iterator node = list.begin();
  while (node != list.end()) {
    if (!(*node)->isIgnored() && (*node)->getDeprel() == chain[0]) {
      if (chain.size() == 1) {
        // TODO(dmytro): do not erase directly, but mark it instead
        //                should be able to reset it later
        res->push_back(*node);
      } else {
        std::vector<std::string> subChain(chain.begin() + 1, chain.end());
        (*node)->accumulateByDeprelChain(Util::stringJoin(subChain, "."), res,
                                         depth + 1);
      }
    }
    ++node;
  }
  return;
}

NodeList Node::selectByDeprelChain(std::string value) {
  // return all possible nodes satisfying the relchain
  NodeList res;
  accumulateByDeprelChain(value, &res, 0);
  return res;
}

NodeList Node::getByDeprelChain(std::string value) {
  NodeList res;
  // kinda dirty hack: the depth will be positive, meaning we'll take children
  // all the time
  accumulateByDeprelChain(value, &res, 1);
  return res;
}

GroupedNodes Node::groupBy(std::string prop) {
  getterptr getterFn = Node::getterByProp(prop);
  if (getterFn == NULL) return GroupedNodes();

  GroupedNodes gn = GroupedNodes();
  // (nodes.front()->*getterFn)()
  NodeList linearOrder = linear();

  for (Node *n : linearOrder) {
    std::string val = (n->*getterFn)();
    if (gn.find(val) == gn.end()) {
      gn[val] = NodeList();
    }
    gn[val].push_sorted(n, compare_node_by_string());
  }
  return gn;
}

NodeList Node::selectHaving(std::string prop, std::string value, bool negate) {
  kvgetterptr getterFn = Node::kvgetterByProp(prop);
  if (getterFn == NULL) return NodeList();

  std::queue<Node *> nodes;
  NodeList result;

  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Util::FeatMap feats = Util::parseUniversalFormat(value);

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    Util::FeatMap nodeFeats = (nodes.front()->*getterFn)();

    bool match = true;
    for (auto it = feats.begin(); it != feats.end(); it++) {
      if (nodeFeats.find(it->first) == nodeFeats.end() ||
          nodeFeats[it->first] != it->second) {
        match = false;
        break;
      }
    }

    if ((match && !negate) || (!match && negate)) {
      result.push_back(nodes.front());
    }

    // add children of the head node to the stack
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return result;
}

bool Node::isIdentical(Node *node, std::string excludeProps) {
  bool uposIncluded = true;
  bool deprelIncluded = true;
  bool formIncluded = true;
  bool lemmaIncluded = true;

  if (!excludeProps.empty()) {
    std::vector<std::string> banned = Util::stringSplit(excludeProps, ',');

    for (std::vector<std::string>::size_type i = 0; i < banned.size(); i++) {
      if (banned[i] == "upos") {
        uposIncluded = false;
      } else if (banned[i] == "deprel") {
        deprelIncluded = false;
      } else if (banned[i] == "lemma") {
        lemmaIncluded = false;
      } else if (banned[i] == "form") {
        formIncluded = false;
      }
    }
  }
  return (!uposIncluded || upos == node->getUpos()) &&
         (!deprelIncluded || deprel == node->getDeprel()) &&
         (!lemmaIncluded || lemma == node->getLemma()) &&
         (!formIncluded || form == node->getForm());
}

/*
 * Query should be a csv list of properties to exclude
 * if all properties should be included, pass an empty std::string
 */
NodeList Node::selectIdenticalExcept(Node *node, std::string excludeProps) {
  std::queue<Node *> nodes;
  NodeList result;

  bool uposIncluded = true;
  bool deprelIncluded = true;
  bool formIncluded = true;
  bool lemmaIncluded = true;

  if (!excludeProps.empty()) {
    std::vector<std::string> banned = Util::stringSplit(excludeProps, ',');

    for (std::vector<std::string>::size_type i = 0; i < banned.size(); i++) {
      if (banned[i] == "upos") {
        uposIncluded = false;
      } else if (banned[i] == "deprel") {
        deprelIncluded = false;
      } else if (banned[i] == "lemma") {
        lemmaIncluded = false;
      } else if (banned[i] == "form") {
        formIncluded = false;
      }
    }
  }

  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    Node *front = nodes.front();

    if ((!uposIncluded || front->getUpos() == node->getUpos()) &&
        (!deprelIncluded || front->getDeprel() == node->getDeprel()) &&
        (!lemmaIncluded || front->getLemma() == node->getLemma()) &&
        (!formIncluded || front->getForm() == node->getForm())) {
      result.push_back(front);
    }

    // add children of the head node to the stack
    NodeList &ch = front->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return result;
}

NodeList Node::selectIdentical(Node *node) {
  return selectIdenticalExcept(node, "");
}

/*
 * Query language is a pretty simple boolean language. Possible properties:
 * pos, rel, lemma. Example queries: > pos = ADJ > pos = AUX && rel = cop >
 * (pos = ADJ || pos = NOUN) && rel = cop
 */
NodeList Node::select(std::string query) {
  // TODO(dmytro): implement this later
  return NodeList();
}

/*
 *  Select an immediate child of the node with the property `prop` equal to
 * `value`
 */
NodeList Node::getBy(std::string prop, std::string value) {
  getterptr getterFn = Node::getterByProp(prop);
  if (getterFn == NULL) return NodeList();

  NodeList result;

  for (int i = 0, len = children.size(); i < len; i++) {
    if ((children[i]->*getterFn)() == value && !children[i]->isIgnored()) {
      result.push_back(children[i]);
    }
  }
  return result;
}

void Node::prune(std::string rel) {
  // can input chains like .conj.obl
  if (rel.empty()) return;

  std::vector<std::string> chain = Util::stringSplit(rel, '.');

  std::vector<Node *>::iterator node = children.begin();
  while (node != children.end()) {
    if ((*node)->getDeprel() == chain[0]) {
      if (chain.size() == 1) {
        // TODO(dmytro): do not erase directly, but mark it instead
        //                should be able to reset it later?
        delete *node;
        node = children.erase(node);
      } else {
        std::vector<std::string> subChain(chain.begin() + 1, chain.end());
        (*node)->prune(Util::stringJoin(subChain, "."));
        ++node;
      }
    } else {
      ++node;
    }
  }
}

/*
 * Check if the subtree induced by the node has a node with the prop
 * NOTE: excluding the node itself
 */
bool Node::propExists(std::string prop, std::string value) {
  getterptr getterFn = Node::getterByProp(prop);
  if (getterFn == NULL) return false;

  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    if ((nodes.front()->*getterFn)() == value) {
      return true;
    }

    // add children of the head node to the stack
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return false;
}

/*
 * Check if the subtree induced by the node has a node with the prop
 * NOTE: excluding the node itself
 */
bool Node::childHasProp(std::string prop, std::string value) {
  getterptr getterFn = Node::getterByProp(prop);
  if (getterFn == NULL) return false;

  for (int i = 0, len = children.size(); i < len; i++) {
    if ((children[i]->*getterFn)() == value) {
      return true;
    }
  }
  return false;
}

Node *Node::makeRoot() {
  Node *newRoot = new Node();
  this->deprel = "root";
  this->parent = newRoot;
  return newRoot;
}

std::string Node::toString() {
  std::string res;
  if (!upos.empty()) res += upos;
  if (!deprel.empty()) {
    if (!res.empty()) res += "|";
    res += deprel;
  }
  if (!form.empty()) {
    if (!res.empty()) res += "|";
    res += form;
  }
  return res;
}

std::string Node::_subtreeToString(int depth) {
  int N = children.size();
  if (N > 0) {
    // to avoid Error C2131 on Windows
    std::string *parts = new std::string[N];
    for (int i = 0; i < N; i++) {
      parts[i] = children[i]->_subtreeToString(depth + 1);
    }
    return toString().insert(0, depth, ' ') + '\n' +
           Util::stringJoin(parts, N, '\n');
  } else {
    return toString().insert(0, depth, ' ');
  }
}

std::string Node::subtreeToString() { return _subtreeToString(0); }

std::string Node::toCharniakString(std::string prop) {
  getterptr getterFn = Node::getterByProp(prop);
  if (getterFn == NULL) return prop + " is an invalid property";
  std::string res = "(" + (this->*getterFn)();
  int N = children.size();
  if (N > 0) {
    for (int i = 0; i < N; i++) {
      res += children[i]->toCharniakString(prop);
    }
  }
  res += ")";
  return res;
}

// TODO(dmytro): rename method - not really an intersect
Node *Node::textualIntersect(std::string text) {
  // TODO(dmytro): maybe return a NodeList instead?
  std::vector<std::string> words = Util::stringSplit(text);

  NodeList res;

  for (std::string word : words) {
    NodeList nodes = selectBy("form", word, false);
    for (auto n : nodes) {
      while (n->getParent() != NULL) {
        // NOTE: this finds only contiguous pieces of text
        if (n->getSubtreeText().find(text) != std::string::npos) {
          if (res.findById(n) == res.end()) res.push_back(n);
          break;
        } else {
          n = n->getParent();
        }
      }
    }
  }

  int N = res.size();
  if (N > 0) {
    if (N == 1) {
      return res[0];
    } else {
      std::vector<Node *>::iterator m =
          min_element(res.begin(), res.end(), compare_node_by_subtree_size());
      return *m;
    }
  } else {
    return NULL;
  }
}

void Node::accumulateHeads(Node *node, std::map<int, int> *heads) {
  NodeList &ch = node->getChildren();
  int len = ch.size();
  // std::cout << "S " << sz << " " << n->getForm() << std::endl;
  for (int i = 0; i < len; i++) {
    (*heads)[ch[i]->getId()] = node->getId();
    accumulateHeads(ch[i], heads);
  }
}

bool Node::isProjective() {
  std::map<int, int> heads;

  accumulateHeads(this, &heads);

  // for(auto it = heads.cbegin(); it != heads.cend(); ++it) {
  //   std::cout << it->first << " " << it->second << "\n";
  // }

  for (auto it = heads.cbegin(); it != heads.cend(); ++it) {
    bool startFirst = it->first < it->second;
    int i = startFirst ? it->first : it->second;
    int j = startFirst ? it->second : it->first;

    for (int k = i + 1; k < j; k++) {
      if (heads.find(k) != heads.end()) {
        if (heads[k] < i || heads[k] > j) {
          return false;
        }
      }
    }
  }

  return true;
}
