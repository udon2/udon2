/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_NODE_H_
#define UDON2_CPP_SRC_NODE_H_

#include <cmath>
#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "MultiWordNode.h"

#include "Util.h"

class Node;
struct NodeComparator;

typedef std::string (Node::*getterptr)();
typedef void (Node::*setterptr)(std::string);

// if linting is enabled, cpplint thinks we try to take the address
typedef Util::FeatMap &(Node::*kvgetterptr)();  // NOLINT

class NodeList : public std::vector<Node *> {
 public:
  NodeList() : std::vector<Node *>() {}
  NodeList(iterator a, iterator b) : std::vector<Node *>(a, b) {}

  template <class Compare>
  void push_sorted(Node *node, Compare cmp) {
    insert(upper_bound(begin(), end(), node, cmp), node);
  }

  iterator findById(Node *n);

  std::string toString();
};

// TODO(dmytro): list manipulation operations (remove, replace, etc)
class TreeList : public std::vector<std::shared_ptr<Node>> {
 public:
  TreeList() : std::vector<std::shared_ptr<Node>>() {}
  TreeList(iterator a, iterator b) : std::vector<std::shared_ptr<Node>>(a, b) {}
};

typedef std::map<std::string, NodeList> GroupedNodes;

// represents token
class Node {
  float id;
  std::string form;
  std::string lemma;
  std::string upos;  // 5 chars at most
  std::string xpos;
  std::string deprel;

  int mIgnoreLabel = -1;

  Node *parent;
  NodeList children;
  Util::FeatMap feats;  // m
  Util::FeatMap misc;

  void accumulateByDeprelChain(std::string value, NodeList *res, int depth);

  std::string _subtreeToString(int);
  int _subtreeSize(Node *n);

  MultiWordNode *mwNode = NULL;

  template <class Compare>
  void _linear(Node *node, NodeList *nodes, Compare cmp);

  void accumulateHeads(Node *node, std::map<int, int> *heads);

  std::string getIdAsString() {
    if (fabs(ceilf(id) - id) < 1e-7) {
      return std::to_string(static_cast<int>(id));
    } else {
      std::string repr = std::to_string(id);
      repr.erase(repr.find_last_not_of('0') + 1, std::string::npos);
      return repr;
    }
  }

 public:
  Node();
  Node(float id, std::string form, std::string lemma, std::string upos,
       std::string xpos, std::string feats, std::string deprel,
       std::string misc, Node *parent);
  ~Node();
  Node(Node *n, std::map<std::string, MultiWordNode *> *mw);
  void init(float id, std::string form, std::string lemma, std::string upos,
            std::string xpos, std::string feats, std::string deprel,
            std::string misc, Node *parent);
  // void freeMemory();

  float getId() { return id; }
  // tried: const std::string &getForm() { return form; }
  // don't think it makes any difference performance-wise
  std::string getForm() { return form; }
  std::string getLemma() { return lemma; }
  std::string getXpos() { return xpos; }
  std::string getUpos() { return upos; }
  std::string getDeprel() { return deprel; }
  Util::FeatMap &getFeats() { return feats; }
  Util::FeatMap &getMisc() { return misc; }
  Node *getParent() { return parent; }
  NodeList &getChildren() {
    return children;
  }  // children are sorted by means of NodeComparator
  MultiWordNode *getMultiWord() { return mwNode; }
  std::string getSubtreeText();

  void setId(float idx) { id = idx; }
  void setForm(std::string newForm) { form = newForm; }
  void setLemma(std::string l) { lemma = l; }
  void setXpos(std::string x) { xpos = x; }
  void setUpos(std::string u) { upos = u; }
  void setDeprel(std::string rel) { deprel = rel; }
  void setMisc(std::string miscstr) {
    misc = Util::parseUniversalFormat(miscstr);
  }
  void setMultiWord(MultiWordNode *n) { mwNode = n; }
  void setParent(Node *n);

  void setFeats(std::string fstr) { feats = Util::parseUniversalFormat(fstr); }

  static getterptr getterByProp(std::string prop);
  static kvgetterptr kvgetterByProp(std::string prop);
  static setterptr setterByProp(std::string prop);

  bool has(std::string prop, std::string key, std::string val);
  bool hasAll(std::string prop, std::string value);
  bool hasAny(std::string prop, std::string value);

  bool isRoot() {
    /**
     * Check if the Node is a root pseudonode by checking if its parent is NULL.
     */
    return parent == NULL;
  }
  bool isLeaf() { return children.size() > 0; }

  bool isIgnored();
  int getIgnoreLabel() { return mIgnoreLabel; }
  void ignore(int label = 0);
  void ignoreSubtree(int label = 0);
  void reset(int label = 0);
  void resetSubtree(int label = 0);
  void hardReset();
  void hardResetSubtree();

  Node *copy();
  Node *makeRoot();

  int subtreeSize();

  NodeList linear();
  NodeList linearSorted();

  template <class Compare>
  NodeList linearBy(Compare cmp);

  void copyChildren(Node *node, std::map<std::string, MultiWordNode *> *mw);
  void addChild(Node *node);
  void removeChild(Node *node);

  // TODO(dmytro): fuzzy select, i.e. by form containing the string "2014"
  // TODO(dmytro): get string indices of the matches returned by "select"
  NodeList selectBy(std::string prop, std::string value, bool negate = false);
  NodeList selectByDeprelChain(std::string value);

  NodeList getBy(std::string prop, std::string value);
  NodeList getByDeprelChain(std::string value);

  NodeList selectHaving(std::string prop, std::string value,
                        bool negate = false);

  GroupedNodes groupBy(std::string prop);

  // TODO(dmytro): isSubtreeIdentical
  // TODO(dmytro): isIdentical should check FEATS as an option also
  bool isIdentical(Node *node, std::string excludeProps);
  NodeList selectIdentical(Node *node);
  NodeList selectIdenticalExcept(Node *node, std::string excludeProps);
  NodeList select(
      std::string query);  // TODO(dmytro): implement a query language

  void prune(std::string rel);

  Node *textualIntersect(std::string text);

  bool propExists(std::string prop, std::string value);
  bool childHasProp(std::string prop, std::string value);

  std::string toString();
  std::string subtreeToString();
  std::string toCharniakString(std::string prop);

  bool isProjective();
};

struct compare_to_node_by_id : public std::unary_function<Node *, bool> {
  Node *baseline;

  explicit compare_to_node_by_id(Node *baseline) : baseline(baseline) {}

  bool operator()(Node *arg) { return baseline->getId() == arg->getId(); }
};

struct compare_node_by_id {
  bool operator()(Node *left, Node *right) {
    return left->getId() < right->getId();
  }
};

struct compare_node_by_subtree_size {
  bool operator()(Node *left, Node *right) {
    return left->subtreeSize() < right->subtreeSize();
  }
};

struct compare_node_by_string {
  bool operator()(Node *left, Node *right) {
    return left->toString() < right->toString();
  }
};

struct compare_node_by_form {
  bool operator()(Node *left, Node *right) {
    return left->getForm() < right->getForm();
  }
};

// Don't move - should be here to avoid errors at linking time
template <class Compare>
NodeList Node::linearBy(Compare cmp) {
  std::queue<Node *> nodes;
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  NodeList linear;
  linear.push_back(this);

  while (!nodes.empty()) {
    linear.push_sorted(nodes.front(), cmp);

    // add children of the head node to the stack
    NodeList ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }

  return linear;
}

#endif  // UDON2_CPP_SRC_NODE_H_
