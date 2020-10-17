/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "Node.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

/*
 * NodeList methods
 */

NodeList::NodeList() {}

NodeList::NodeList(const NodeList &list) { this->nodes = list.getNodes(); }

void NodeList::push_sorted(Node *node) {
  // inserts in a lexicographical order
  nodes.insert(
      upper_bound(nodes.begin(), nodes.end(), node, compare_node_by_string()),
      node);
}

std::vector<Node *>::iterator NodeList::findById(Node *n) {
  return find_if(nodes.begin(), nodes.end(), compare_node_by_id(n));
}

std::string NodeList::toString() {
  std::string res = "";
  for (int i = 0, len = nodes.size(); i < len; i++) {
    res += nodes[i]->toString();
    if (i < len - 1) {
      res += " ";
    }
  }
  return res;
}

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

Node::Node(Node *n) {
  this->id = n->getId();
  this->form = n->getForm();
  this->lemma = n->getLemma();
  this->upos = n->getUpos();
  this->xpos = n->getXpos();
  this->deprel = n->getDeprel();
  this->parent = n->getParent();
  this->feats = n->getFeats();
  this->misc = n->getMisc();
  this->copyChildren(n);
  this->mIgnoreLabel = n->getIgnoreLabel();
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

bool Node::isRoot() {
  /**
   * Check if the Node is a root pseudonode by checking if its parent is NULL.
   */
  return parent == NULL;
}

std::string Node::getFeatsAsString() {
  /**
   * Get FEATS of a Node as a UD string, e.g. Case=Nom|Person=2|PronType=Prs
   */
  // Create a map iterator and point to beginning of map
  std::map<std::string, std::string>::iterator it = feats.begin();

  std::vector<std::string> featsVec;

  // Iterate over the map using Iterator till end.
  while (it != feats.end()) {
    featsVec.push_back(it->first + "=" + it->second);

    // Increment the Iterator to point to next entry
    it++;
  }

  std::sort(featsVec.begin(), featsVec.end());

  return Util::stringJoin(featsVec, '|');
}

bool Node::hasFeat(std::string key, std::string value) {
  /**
   * Check if the Node's FEATS contain a morphological feature `key`=`value`.
   */
  if (feats.count(key) > 0) {
    return feats[key] == value;
  } else {
    return false;
  }
}

bool Node::hasAllFeats(std::string value) {
  /**
   * Check if the Node's FEATS has all morphological features specified in the
   * string `value`, e.g. \code{.cpp}
   * node->hasAllFeatures("Case=Nom|PronType=Prs")
   * \endcode
   *
   * Note that the check is not exclusive, e.g. consider the node with
   * morphological properties Case=Nom|Person=2|PronType=Prs, then the following
   * holds. \code{.cpp} node->hasAllFeatures("Case=Nom|Person=2") // returns
   * true node->hasAllFeatures("Case=Nom")          // returns true
   * node->hasAllFeatures("Case=Nom|Person=3") // returns false
   * \endcode
   */
  Util::FeatMap givenFeats = Util::parseUniversalFormat(value);
  for (auto it = givenFeats.begin(); it != givenFeats.end(); it++) {
    if (feats.find(it->first) == feats.end() ||
        feats[it->first] != it->second) {
      return false;
    }
  }
  return true;
}

NodeList Node::getSubtreeNodes() {
  /**
   * Get a list of all nodes in the subtree in the BFS order.
   */
  NodeList all;
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
    all.push_back(children[i]);
  }

  while (!nodes.empty()) {
    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
      all.push_back(ch[i]);
    }
    nodes.pop();
  }

  return all;
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

    NodeList ch = nodes.front()->getChildren();
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

    NodeList ch = nodes.front()->getChildren();
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

int Node::subtreeSize() {
  /**
   * Get the number of nodes in the subtree induced by the current node.
   */
  int sz = 0;
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
    sz++;
  }

  while (!nodes.empty()) {
    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
      sz++;
    }
    nodes.pop();
  }
  return sz;
}

void Node::copyChildren(Node *node) {
  /**
   * Copy children of Node `node` to the current Node.
   */
  children = NodeList();
  for (Node *n : node->getChildren()) {
    children.push_sorted(new Node(n));
  }
}

void Node::addChild(Node *node) {
  /**
   * Add `node` as a child of the current Node. Note that all children are
   * stored in a lexicographical order of Node's string representations, i.e.
   * UPOS|DEPREL|FORM
   */
  children.push_sorted(node);
}

std::string Node::getSubtreeText() {
  /**
   * Generate a text of the subtree induced by its node using the current FORM
   * values of all nodes in the subtree except the ignored nodes.
   */
  // TODO(dmytro): Fix SpaceAfter=No thing
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  std::map<float, std::string> words;
  std::map<float, bool> isPunct;
  int wordsNum;
  if (!isIgnored()) {
    words[id] = form;
    wordsNum = 1;
  } else {
    wordsNum = 0;
  }

  while (!nodes.empty()) {
    if (!nodes.front()->isIgnored() &&
        words.find(nodes.front()->getId()) == words.end()) {
      words[nodes.front()->getId()] = nodes.front()->getForm();
      isPunct[nodes.front()->getId()] = nodes.front()->getUpos() == "PUNCT";
      wordsNum++;
    }

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }

  std::ostringstream os;
  int j = 0;
  for (std::map<float, std::string>::iterator it = words.begin();
       it != words.end(); it++) {
    if (j > 0 && j < wordsNum && !isPunct[it->first]) os << " ";
    j++;
    os << it->second;
  }
  return os.str();
}

NodeList Node::linear() {
  /**
   * Represent a subtree induced by the current node in a linear order, as they
   * appear in the original sentence.
   */
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  std::map<float, Node *> words;
  words[id] = this;

  while (!nodes.empty()) {
    if (words.find(nodes.front()->getId()) == words.end()) {
      words[nodes.front()->getId()] = nodes.front();
    }

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }

  NodeList linear;
  for (std::map<float, Node *>::iterator it = words.begin(); it != words.end();
       it++) {
    linear.push_back(it->second);
  }
  return linear;
}

NodeList Node::linearSorted() {
  /**
   * Represent a subtree induced by the current node in a linear order, sorted
   * lexicographically based on their string representation.
   */
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  NodeList linear;
  linear.push_back(this);

  while (!nodes.empty()) {
    linear.push_sorted(nodes.front());

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }

  return linear;
}

getterptr Node::getterByProp(std::string prop) {
  getterptr getterFn = NULL;
  if (prop == "upos")
    getterFn = &Node::getUpos;
  else if (prop == "lemma")
    getterFn = &Node::getLemma;
  else if (prop == "deprel")
    getterFn = &Node::getDeprel;
  else if (prop == "feats")
    getterFn = &Node::getFeatsAsString;
  else if (prop == "form")
    getterFn = &Node::getForm;
  return getterFn;
}

NodeList Node::selectBy(std::string prop, std::string value, bool negate) {
  /**
   * Select all nodes from the subtree induced by the current node (including,
   * possibley, the node itself) having a property `prop` being equal to
   * `value`. If `negate` is true, then the exact opposite happens, i.e. only
   * nodes with a property `prop` being NOT equal to `value` will be selected.
   */
  getterptr getterFn = getterByProp(prop);
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
      NodeList ch = nodes.front()->getChildren();
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
        (*node)->accumulateByDeprelChain(Util::stringJoin(subChain, '.'), res,
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
  getterptr getterFn = getterByProp(prop);
  if (getterFn == NULL) return GroupedNodes();

  GroupedNodes gn = GroupedNodes();
  // (nodes.front()->*getterFn)()
  NodeList linearOrder = linear();

  for (Node *n : linearOrder) {
    std::string val = (n->*getterFn)();
    if (gn.find(val) == gn.end()) {
      gn[val] = NodeList();
    }
    gn[val].push_sorted(n);
  }
  return gn;
}

NodeList Node::selectHaving(std::string value, bool negate) {
  std::queue<Node *> nodes;
  NodeList result;

  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Util::FeatMap feats = Util::parseUniversalFormat(value);

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    Util::FeatMap nodeFeats = nodes.front()->getFeats();

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
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return result;
}

NodeList Node::selectHavingFeats(std::string value) {
  return selectHaving(value, false);
}

NodeList Node::selectMissingFeats(std::string value) {
  return selectHaving(value, true);
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
    NodeList ch = front->getChildren();
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
  getterptr getterFn = getterByProp(prop);
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
        node = children.erase(node);
      } else {
        std::vector<std::string> subChain(chain.begin() + 1, chain.end());
        (*node)->prune(Util::stringJoin(subChain, '.'));
        ++node;
      }
    } else {
      ++node;
    }
  }
}

void Node::removeChild(Node *node) {
  std::vector<Node *>::iterator it =
      find(children.begin(), children.end(), node);
  if (it != children.end()) children.erase(it);
}

/*
 * Check if the subtree induced by the node has a node with the prop
 * NOTE: excluding the node itself
 */
bool Node::propExists(std::string prop, std::string value) {
  getterptr getterFn = getterByProp(prop);
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
    NodeList ch = nodes.front()->getChildren();
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
  getterptr getterFn = getterByProp(prop);
  if (getterFn == NULL) return false;

  for (int i = 0, len = children.size(); i < len; i++) {
    if ((children[i]->*getterFn)() == value) {
      return true;
    }
  }
  return false;
}

Node *Node::copy() { return new Node(this); }

void Node::makeRoot() {
  Node *newRoot = new Node();
  this->deprel = "root";
  this->parent = newRoot;
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

// TODO(dmytro): rename method - not really an intersect
Node *Node::textualIntersect(std::string text) {
  // TODO(dmytro): maybe return a NodeList instead?
  std::vector<std::string> words = Util::stringSplit(text, ' ');

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

Node *Node::toPCT() {
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *rootRelNode = new Node(id, deprel, "", "", "", "", "", "", NULL);
  Node *posNode =
      new Node(id + 0.1f, upos, "", "", "", "", "", "", rootRelNode);
  new Node(id + 0.2f, form, "", "", "", "", "", "", posNode);

  std::map<float, Node *> nodesMap;
  nodesMap[id] = posNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *relNode =
        new Node(frontId, nodes.front()->getDeprel(), "", "", "", "", "", "",
                 nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1f, nodes.front()->getUpos(), "", "",
                             "", "", "", "", relNode);
    new Node(frontId + 0.2f, nodes.front()->getForm(), "", "", "", "", "", "",
             posNode);

    nodesMap[frontId] = posNode;

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return rootRelNode;
}

Node *Node::toGRCT() {
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *relNode = new Node(id, deprel, "", "", "", "", "", "", NULL);
  Node *posNode = new Node(id + 0.1f, upos, "", "", "", "", "", "", relNode);
  new Node(id + 0.2f, form, "", "", "", "", "", "", posNode);

  std::map<float, Node *> nodesMap;
  nodesMap[id] = relNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *relNode =
        new Node(frontId, nodes.front()->getDeprel(), "", "", "", "", "", "",
                 nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1f, nodes.front()->getUpos(), "", "",
                             "", "", "", "", relNode);
    new Node(frontId + 0.2f, nodes.front()->getForm(), "", "", "", "", "", "",
             posNode);

    nodesMap[frontId] = relNode;

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return nodesMap[id];
}

Node *Node::toLCT() {
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *formNode = new Node(id + 0.2f, form, "", "", "", "", "", "", NULL);
  new Node(id, deprel, "", "", "", "", "", "", formNode);
  new Node(id + 0.1f, upos, "", "", "", "", "", "", formNode);

  std::map<float, Node *> nodesMap;
  nodesMap[id] = formNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *formNode =
        new Node(frontId + 0.2f, nodes.front()->getForm(), "", "", "", "", "",
                 "", nodesMap[nodes.front()->getParent()->getId()]);
    new Node(frontId + 0.1f, nodes.front()->getUpos(), "", "", "", "", "", "",
             formNode);
    new Node(frontId, nodes.front()->getDeprel(), "", "", "", "", "", "",
             formNode);

    nodesMap[frontId] = formNode;

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return nodesMap[id];
}
