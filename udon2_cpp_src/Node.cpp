/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "Node.h"

#include <bits/stdc++.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

#include "Util.h"

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
    : text("root"), pos("ROOT"), rel(""), id(0), lemma(""), parent(NULL) {
  this->mIgnore = false;
}

Node::Node(float id, std::string text, std::string lemma, std::string pos,
           std::string morph, std::string rel, Node *parent)
    : text(text), pos(pos), rel(rel), id(id), lemma(lemma), parent(parent) {
  this->ufeats = parseMorphFeatures(morph);
  this->mIgnore = false;
  if (parent != NULL) parent->addChild(this);
}

Node::Node(Node *n) {
  this->id = n->getId();
  this->text = n->getText();
  this->lemma = n->getLemma();
  this->pos = n->getPos();
  this->rel = n->getRel();
  this->parent = n->getParent();
  this->ufeats = n->getUfeats();
  this->copyChildren(n);
  this->mIgnore = n->isIgnored();
}

void Node::init(float id, std::string text, std::string lemma, std::string pos,
                std::string morph, std::string rel, Node *parent) {
  this->id = id;
  this->text = text;
  this->lemma = lemma;
  this->pos = pos;
  this->rel = rel;
  this->parent = parent;
  if (parent != NULL) parent->addChild(this);
  this->ufeats = parseMorphFeatures(morph);
  this->mIgnore = false;
}

bool Node::isRoot() { return parent == NULL; }

std::string Node::getText() { return text; }

float Node::getId() { return id; }

std::string Node::getLemma() { return lemma; }

std::string Node::getPos() { return pos; }

FeatMap Node::parseMorphFeatures(std::string morph) {
  FeatMap ufeats = FeatMap();
  std::vector<std::string> feats = Util::stringSplit(morph, '|');
  for (std::string f : feats) {
    std::vector<std::string> kv = Util::stringSplit(f, '=');
    if (kv.size() == 2) {
      ufeats[kv[0]] = kv[1];
    }
  }
  return ufeats;
}

FeatMap Node::getUfeats() { return ufeats; }

std::string Node::getMorph() {
  // Create a map iterator and point to beginning of map
  std::map<std::string, std::string>::iterator it = ufeats.begin();

  std::vector<std::string> feats;

  // Iterate over the map using Iterator till end.
  while (it != ufeats.end()) {
    feats.push_back(it->first + "=" + it->second);

    // Increment the Iterator to point to next entry
    it++;
  }

  std::sort(feats.begin(), feats.end());

  return Util::stringJoin(feats, '|');
}

bool Node::hasMorph(std::string key, std::string value) {
  if (ufeats.count(key) > 0) {
    return ufeats[key] == value;
  } else {
    return false;
  }
}

std::string Node::getRel() { return rel; }

Node *Node::getParent() { return parent; }

bool Node::hasChildren() { return children.size() > 0; }

NodeList Node::getChildren() {
  // children are sorted by means of NodeComparator
  return children;
}

NodeList Node::getSubtreeNodes() {
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

bool Node::isIgnored() { return mIgnore; }

void Node::ignore() { this->mIgnore = true; }

void Node::reset() {
  // TODO(dmytro): a more generic reset from pruning?
  this->mIgnore = false;
}

void Node::ignoreSubtree() {
  this->ignore();
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // add children of the head node to the stack
    nodes.front()->ignore();

    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
}

void Node::resetSubtree() {
  this->reset();
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // add children of the head node to the stack
    nodes.front()->reset();

    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
}

int Node::subtreeSize() {
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
  children = NodeList();
  for (Node *n : node->getChildren()) {
    children.push_sorted(new Node(n));
  }
}

void Node::addChild(Node *node) { children.push_sorted(node); }

std::string Node::getSubtreeText() {
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  std::map<float, std::string> words;
  std::map<float, bool> isPunct;
  int wordsNum;
  if (!isIgnored()) {
    words[id] = text;
    wordsNum = 1;
  } else {
    wordsNum = 0;
  }

  while (!nodes.empty()) {
    if (!nodes.front()->isIgnored() &&
        words.find(nodes.front()->getId()) == words.end()) {
      words[nodes.front()->getId()] = nodes.front()->getText();
      isPunct[nodes.front()->getId()] = nodes.front()->getPos() == "PUNCT";
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

// turn a subtree into a linear structure
NodeList Node::linear() {
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
  if (prop == "pos")
    getterFn = &Node::getPos;
  else if (prop == "lemma")
    getterFn = &Node::getLemma;
  else if (prop == "rel")
    getterFn = &Node::getRel;
  else if (prop == "morph")
    getterFn = &Node::getMorph;
  else if (prop == "text")
    getterFn = &Node::getText;
  return getterFn;
}

/*
 * Select by a node by a property from the subtree induced by the node
 * NOTE: including the node itself
 */
NodeList Node::selectBy(std::string prop, std::string value, bool negate) {
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

NodeList Node::selectByPos(std::string value) {
  return selectBy("pos", value, false);
}

NodeList Node::selectByLemma(std::string value) {
  return selectBy("lemma", value, false);
}

NodeList Node::selectByRel(std::string value) {
  return selectBy("rel", value, false);
}

NodeList Node::selectByMorph(std::string value) {
  std::vector<std::string> v = Util::stringSplit(value, '|');
  sort(v.begin(), v.end());
  return selectBy("morph", Util::stringJoin(v, '|'), false);
}

NodeList Node::selectByText(std::string value) {
  return selectBy("text", value, false);
}

NodeList Node::selectExceptPos(std::string value) {
  return selectBy("pos", value, true);
}

NodeList Node::selectExceptLemma(std::string value) {
  return selectBy("lemma", value, true);
}

NodeList Node::selectExceptRel(std::string value) {
  return selectBy("rel", value, true);
}

NodeList Node::selectExceptText(std::string value) {
  return selectBy("text", value, true);
}

void Node::accumulateByRelChain(std::string value, NodeList *res, int depth) {
  if (value.empty()) return;

  std::vector<std::string> chain = Util::stringSplit(value, '.');

  NodeList list = depth > 0 ? children : selectByRel(chain[0]);

  std::vector<Node *>::iterator node = list.begin();
  while (node != list.end()) {
    if (!(*node)->isIgnored() && (*node)->getRel() == chain[0]) {
      if (chain.size() == 1) {
        // TODO(dmytro): do not erase directly, but mark it instead
        //                should be able to reset it later
        res->push_back(*node);
      } else {
        std::vector<std::string> subChain(chain.begin() + 1, chain.end());
        (*node)->accumulateByRelChain(Util::stringJoin(subChain, '.'), res,
                                      depth + 1);
      }
    }
    ++node;
  }
  return;
}

NodeList Node::selectByRelChain(std::string value) {
  // return all possible nodes satisfying the relchain
  NodeList res;
  accumulateByRelChain(value, &res, 0);
  return res;
}

NodeList Node::getByRelChain(std::string value) {
  NodeList res;
  // kinda dirty hack: the depth will be positive, meaning we'll take children
  // all the time
  accumulateByRelChain(value, &res, 1);
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

bool Node::hasAllMorph(std::string value) {
  FeatMap feats = parseMorphFeatures(value);
  for (auto it = feats.begin(); it != feats.end(); it++) {
    if (ufeats.find(it->first) == ufeats.end() ||
        ufeats[it->first] != it->second) {
      return false;
    }
  }
  return true;
}

NodeList Node::selectHaving(std::string value, bool negate) {
  std::queue<Node *> nodes;
  NodeList result;

  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  FeatMap feats = parseMorphFeatures(value);

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    FeatMap nodeFeats = nodes.front()->getUfeats();

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

NodeList Node::selectHavingMorph(std::string value) {
  return selectHaving(value, false);
}

NodeList Node::selectMissingMorph(std::string value) {
  return selectHaving(value, true);
}

bool Node::isIdentical(Node *node, std::string excludeProps) {
  bool posIncluded = true;
  bool relIncluded = true;
  bool textIncluded = true;
  bool lemmaIncluded = true;

  if (!excludeProps.empty()) {
    std::vector<std::string> banned = Util::stringSplit(excludeProps, ',');

    for (std::vector<std::string>::size_type i = 0; i < banned.size(); i++) {
      if (banned[i] == "pos") {
        posIncluded = false;
      } else if (banned[i] == "rel") {
        relIncluded = false;
      } else if (banned[i] == "lemma") {
        lemmaIncluded = false;
      } else if (banned[i] == "text") {
        textIncluded = false;
      }
    }
  }
  return (!posIncluded || pos == node->getPos()) &&
         (!relIncluded || rel == node->getRel()) &&
         (!lemmaIncluded || lemma == node->getLemma()) &&
         (!textIncluded || text == node->getText());
}

/*
 * Query should be a csv list of properties to exclude
 * if all properties should be included, pass an empty std::string
 */
NodeList Node::selectIdenticalExcept(Node *node, std::string excludeProps) {
  std::queue<Node *> nodes;
  NodeList result;

  bool posIncluded = true;
  bool relIncluded = true;
  bool textIncluded = true;
  bool lemmaIncluded = true;

  if (!excludeProps.empty()) {
    std::vector<std::string> banned = Util::stringSplit(excludeProps, ',');

    for (std::vector<std::string>::size_type i = 0; i < banned.size(); i++) {
      if (banned[i] == "pos") {
        posIncluded = false;
      } else if (banned[i] == "rel") {
        relIncluded = false;
      } else if (banned[i] == "lemma") {
        lemmaIncluded = false;
      } else if (banned[i] == "text") {
        textIncluded = false;
      }
    }
  }

  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    // check the POS-tag and if matches add to the result
    Node *front = nodes.front();

    if ((!posIncluded || front->getPos() == node->getPos()) &&
        (!relIncluded || front->getRel() == node->getRel()) &&
        (!lemmaIncluded || front->getLemma() == node->getLemma()) &&
        (!textIncluded || front->getText() == node->getText())) {
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

NodeList Node::getByPos(std::string value) { return getBy("pos", value); }

NodeList Node::getByLemma(std::string value) { return getBy("lemma", value); }

NodeList Node::getByRel(std::string value) { return getBy("rel", value); }

void Node::prune(std::string rel) {
  // can input chains like .conj.obl
  if (rel.empty()) return;

  std::vector<std::string> chain = Util::stringSplit(rel, '.');

  std::vector<Node *>::iterator node = children.begin();
  while (node != children.end()) {
    if ((*node)->getRel() == chain[0]) {
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

bool Node::posExists(std::string value) { return propExists("pos", value); }

bool Node::relExists(std::string value) { return propExists("rel", value); }

bool Node::lemmaExists(std::string value) { return propExists("lemma", value); }

bool Node::textExists(std::string value) { return propExists("text", value); }

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

bool Node::childHasPos(std::string value) { return childHasProp("pos", value); }

bool Node::childHasRel(std::string value) { return childHasProp("rel", value); }

bool Node::childHasLemma(std::string value) {
  return childHasProp("lemma", value);
}

bool Node::childHasText(std::string value) {
  return childHasProp("text", value);
}

Node *Node::copy() { return new Node(this); }

void Node::makeRoot() {
  Node *newRoot = new Node();
  this->rel = "root";
  this->parent = newRoot;
}

std::string Node::toString() {
  std::string res;
  if (!pos.empty()) res += pos;
  if (!rel.empty()) {
    if (!res.empty()) res += "|";
    res += rel;
  }
  if (!text.empty()) {
    if (!res.empty()) res += "|";
    res += text;
  }
  return res;
}

std::string Node::_subtreeToString(int depth) {
  int N = children.size();
  if (N > 0) {
    std::string parts[N];
    for (int i = 0; i < N; i++) {
      parts[i] = children[i]->_subtreeToString(depth + 1);
    }
    return toString().insert(0, depth, ' ') + '\n' +
           Util::stringJoin(parts, '\n');
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
    NodeList nodes = selectByText(word);
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

  Node *rootRelNode = new Node(id, rel, "", "", "", "", NULL);
  Node *posNode = new Node(id + 0.1, pos, "", "", "", "", rootRelNode);
  Node *textNode = new Node(id + 0.2, text, "", "", "", "", posNode);

  std::map<float, Node *> nodesMap;
  nodesMap[id] = posNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *relNode = new Node(frontId, nodes.front()->getRel(), "", "", "", "",
                             nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1, nodes.front()->getPos(), "", "", "",
                             "", relNode);
    Node *textNode = new Node(frontId + 0.2, nodes.front()->getText(), "", "",
                              "", "", posNode);

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

  Node *relNode = new Node(id, rel, "", "", "", "", NULL);
  Node *posNode = new Node(id + 0.1, pos, "", "", "", "", relNode);
  Node *textNode = new Node(id + 0.2, text, "", "", "", "", posNode);

  std::map<float, Node *> nodesMap;
  nodesMap[id] = relNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *relNode = new Node(frontId, nodes.front()->getRel(), "", "", "", "",
                             nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1, nodes.front()->getPos(), "", "", "",
                             "", relNode);
    Node *textNode = new Node(frontId + 0.2, nodes.front()->getText(), "", "",
                              "", "", posNode);

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

  Node *textNode = new Node(id + 0.2, text, "", "", "", "", NULL);
  Node *relNode = new Node(id, rel, "", "", "", "", textNode);
  Node *posNode = new Node(id + 0.1, pos, "", "", "", "", textNode);

  std::map<float, Node *> nodesMap;
  nodesMap[id] = textNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *textNode =
        new Node(frontId + 0.2, nodes.front()->getText(), "", "", "", "",
                 nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1, nodes.front()->getPos(), "", "", "",
                             "", textNode);
    Node *relNode =
        new Node(frontId, nodes.front()->getRel(), "", "", "", "", textNode);

    nodesMap[frontId] = textNode;

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return nodesMap[id];
}
