/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_NODE_H_
#define UDON2_CPP_SRC_NODE_H_

#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <iostream>

#include "MultiWordNode.h"

#include "Util.h"

class Node;
struct NodeComparator;

// if linting is enabled, cpplint thinks we try to take the address
typedef const std::string &(Node::*getterptr)();  // NOLINT
typedef Util::FeatMap &(Node::*kvgetterptr)();    // NOLINT

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

class TreeList : public std::vector<Node *> {
 public:
  TreeList() : std::vector<Node *>() {}
  TreeList(iterator a, iterator b) : std::vector<Node *>(a, b) {}
  void freeMemory();
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

  bool first = false;
  bool last = false;

  Node *parent;
  NodeList children;
  Util::FeatMap feats;  // m
  Util::FeatMap misc;

  getterptr getterByProp(std::string prop);
  kvgetterptr kvgetterByProp(std::string prop);
  void accumulateByDeprelChain(std::string value, NodeList *res, int depth);

  std::string _subtreeToString(int);
  int _subtreeSize(Node *n);

  MultiWordNode *mwNode = NULL;

  template <class Compare>
  void _linear(Node *node, NodeList *nodes, Compare cmp);

 public:
  Node();
  Node(float id, std::string form, std::string lemma, std::string upos,
       std::string xpos, std::string feats, std::string deprel,
       std::string misc, Node *parent);
  explicit Node(Node *n);
  void init(float id, std::string form, std::string lemma, std::string upos,
            std::string xpos, std::string feats, std::string deprel,
            std::string misc, Node *parent);
  void freeMemory();

  float getId() { return id; }
  // tried: const std::string &getForm() { return form; }
  // don't think it makes any difference performance-wise
  const std::string &getForm() { return form; }
  const std::string &getLemma() { return lemma; }
  const std::string &getXpos() { return xpos; }
  const std::string &getUpos() { return upos; }
  const std::string &getDeprel() { return deprel; }
  Util::FeatMap &getFeats() { return feats; }
  Util::FeatMap &getMisc() { return misc; }
  Node *getParent() { return parent; }
  NodeList &getChildren() {
    return children;
  }  // children are sorted by means of NodeComparator
  MultiWordNode *getMultiWord() { return mwNode; }
  std::string getFeatsAsString();
  std::string getSubtreeText();

  void setFirst() {
    first = true;
    last = false;
  }
  void setLast() {
    first = false;
    last = true;
  }

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

  bool has(std::string prop, std::string key, std::string val);
  bool hasAll(std::string prop, std::string value);
  bool hasAny(std::string prop, std::string value);

  NodeList getSubtreeNodes();

  bool isRoot() {
    /**
     * Check if the Node is a root pseudonode by checking if its parent is NULL.
     */
    return parent == NULL;
  }
  bool isLeaf() { return children.size() > 0; }
  bool isFirst() { return first; }
  bool isLast() { return last; }

  bool isIgnored();
  int getIgnoreLabel() { return mIgnoreLabel; }
  void ignore(int label = 0);
  void ignoreSubtree(int label = 0);
  void reset(int label = 0);
  void resetSubtree(int label = 0);
  void hardReset();
  void hardResetSubtree();

  Node *copy();
  void makeRoot();

  int subtreeSize();

  NodeList linear();
  NodeList linearSorted();

  template <class Compare>
  NodeList linearBy(Compare cmp);

  void copyChildren(Node *node);
  void addChild(Node *node);

  NodeList selectBy(std::string prop, std::string value, bool negate = false);
  NodeList selectByDeprelChain(std::string value);

  NodeList getBy(std::string prop, std::string value);
  NodeList getByDeprelChain(std::string value);

  NodeList selectHaving(std::string prop, std::string value,
                        bool negate = false);

  GroupedNodes groupBy(std::string prop);

  bool isIdentical(Node *node, std::string excludeProps);
  NodeList selectIdentical(Node *node);
  NodeList selectIdenticalExcept(Node *node, std::string excludeProps);
  NodeList select(
      std::string query);  // TODO(dmytro): implement a query language

  void prune(std::string rel);
  void removeChild(Node *node);

  Node *textualIntersect(std::string text);

  bool propExists(std::string prop, std::string value);
  bool childHasProp(std::string prop, std::string value);

  std::string toString();
  std::string subtreeToString();
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
