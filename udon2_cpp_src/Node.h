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

#include "MultiWordNode.h"

#include "Util.h"

class Node;
struct NodeComparator;

typedef std::string (Node::*getterptr)();

class NodeList {
  std::vector<Node *> nodes;

 public:
  NodeList();
  NodeList(const NodeList &list);

  void push_sorted(Node *node);

  template <class Compare>
  void push_sorted(Node *node, Compare cmp) {
    nodes.insert(upper_bound(nodes.begin(), nodes.end(), node, cmp), node);
  }

  void push_back(Node *node) { nodes.push_back(node); }

  std::vector<Node *>::iterator findById(Node *n);

  int size() { return nodes.size(); }

  Node *operator[](int i) const {
    int N = nodes.size();
    if (i >= 0 && i < N) {
      return nodes[i];
    } else if (i < 0 && N + i >= 0) {
      // negative indices
      return nodes[N + i];
    } else {
      // TODO(dmytro) Should raise IndexError as Python does?
      return NULL;
    }
  }

  std::vector<Node *>::iterator begin() { return nodes.begin(); }

  std::vector<Node *>::iterator end() { return nodes.end(); }

  std::vector<Node *>::iterator erase(std::vector<Node *>::iterator it) {
    return nodes.erase(it);
  }

  std::vector<Node *> getNodes() const { return nodes; }

  std::string toString();
};

typedef std::map<std::string, NodeList> GroupedNodes;

// represents token
class Node {
  std::string form;
  std::string xpos;
  std::string upos;
  std::string deprel;
  float id;
  std::string lemma;

  bool mIgnore = false;
  int mIgnoreLabel = -1;

  Node *parent;
  NodeList children;
  Util::FeatMap feats;  // m
  Util::FeatMap misc;

  getterptr getterByProp(std::string prop);
  NodeList selectHaving(std::string value, bool negate);
  void accumulateByDeprelChain(std::string value, NodeList *res, int depth);

  std::string _subtreeToString(int);

  MultiWordNode *mwNode = NULL;

 public:
  Node();
  Node(float id, std::string form, std::string lemma, std::string upos,
       std::string xpos, std::string feats, std::string deprel,
       std::string misc, Node *parent);
  explicit Node(Node *n);
  void init(float id, std::string form, std::string lemma, std::string upos,
            std::string xpos, std::string feats, std::string deprel,
            std::string misc, Node *parent);

  float getId() { return id; }
  std::string getForm() { return form; }
  std::string getLemma() { return lemma; }
  std::string getXpos() { return xpos; }
  std::string getUpos() { return upos; }
  Util::FeatMap getFeats() { return feats; }
  std::string getDeprel() { return deprel; }
  Util::FeatMap getMisc() { return misc; }
  Node *getParent() { return parent; }
  NodeList getChildren() {
    return children;
  }  // children are sorted by means of NodeComparator
  MultiWordNode *getMultiWord() { return mwNode; }
  std::string getFeatsAsString();
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
  void alterFeat(std::string key, std::string val) { feats[key] = val; }
  bool hasFeat(std::string key, std::string val);
  bool hasAllFeats(std::string value);

  NodeList getSubtreeNodes();

  bool isRoot();

  bool isIgnored();
  int getIgnoredLabel() { return mIgnoreLabel; }
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

  bool hasChildren() { return children.size() > 0; }
  void copyChildren(Node *node);
  void addChild(Node *node);

  NodeList selectBy(std::string prop, std::string value, bool negate);
  NodeList selectByDeprelChain(std::string value);

  NodeList getBy(std::string prop, std::string value);
  NodeList getByDeprelChain(std::string value);

  GroupedNodes groupBy(std::string prop);

  NodeList selectHavingFeats(std::string value);
  NodeList selectMissingFeats(std::string value);

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

  Node *toPCT();
  Node *toGRCT();
  Node *toLCT();
};

struct compare_node_by_id : public std::unary_function<Node *, bool> {
  Node *baseline;

  explicit compare_node_by_id(Node *baseline) : baseline(baseline) {}

  bool operator()(Node *arg) { return baseline->getId() == arg->getId(); }
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
