/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include "transformations.h"
#include "Util.h"

#include <queue>
#include <map>
#include <vector>

namespace transformations {
Node *toPCT(Node *n, bool includeForm, bool includeFeats) {
  std::queue<Node *> nodes;
  NodeList &children = n->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *rootRelNode =
      new Node(n->getId(), n->getDeprel(), "", "", "", "", "", "", NULL);
  Node *posNode = new Node(n->getId() + 0.1f, n->getUpos(), "", "", "", "", "",
                           "", rootRelNode);
  if (includeForm) {
    new Node(n->getId() + 0.2f, n->getForm(), "", "", "", "", "", "", posNode);
  }

  if (includeFeats) {
    new Node(n->getId() + 0.3f, Util::stringifyFeatMap(n->getFeats()), "", "",
             "", "", "", "", posNode);
  }

  std::map<float, Node *> nodesMap;
  nodesMap[n->getId()] = posNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *relNode =
        new Node(frontId, nodes.front()->getDeprel(), "", "", "", "", "", "",
                 nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1f, nodes.front()->getUpos(), "", "",
                             "", "", "", "", relNode);
    if (includeForm) {
      new Node(frontId + 0.2f, nodes.front()->getForm(), "", "", "", "", "", "",
               posNode);
    }

    if (includeFeats) {
      new Node(frontId + 0.3f,
               Util::stringifyFeatMap(nodes.front()->getFeats()), "", "", "",
               "", "", "", posNode);
    }

    nodesMap[frontId] = posNode;

    // add children of the head node to the stack
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return rootRelNode;
}

Node *toGRCT(Node *n, bool includeForm, bool includeFeats) {
  std::queue<Node *> nodes;
  NodeList &children = n->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *relNode =
      new Node(n->getId(), n->getDeprel(), "", "", "", "", "", "", NULL);
  Node *posNode = new Node(n->getId() + 0.1f, n->getUpos(), "", "", "", "", "",
                           "", relNode);

  if (includeForm) {
    new Node(n->getId() + 0.2f, n->getForm(), "", "", "", "", "", "", posNode);
  }

  if (includeFeats) {
    new Node(n->getId() + 0.3f, Util::stringifyFeatMap(n->getFeats()), "", "",
             "", "", "", "", posNode);
  }

  std::map<float, Node *> nodesMap;
  nodesMap[n->getId()] = relNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *relNode =
        new Node(frontId, nodes.front()->getDeprel(), "", "", "", "", "", "",
                 nodesMap[nodes.front()->getParent()->getId()]);
    Node *posNode = new Node(frontId + 0.1f, nodes.front()->getUpos(), "", "",
                             "", "", "", "", relNode);
    if (includeForm) {
      new Node(frontId + 0.2f, nodes.front()->getForm(), "", "", "", "", "", "",
               posNode);
    }

    if (includeFeats) {
      new Node(frontId + 0.3f,
               Util::stringifyFeatMap(nodes.front()->getFeats()), "", "", "",
               "", "", "", posNode);
    }

    nodesMap[frontId] = relNode;

    // add children of the head node to the stack
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return nodesMap[n->getId()];
}

Node *toLCT(Node *n, bool includeFeats) {
  std::queue<Node *> nodes;
  NodeList &children = n->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *formNode =
      new Node(n->getId() + 0.2f, n->getForm(), "", "", "", "", "", "", NULL);
  new Node(n->getId(), n->getDeprel(), "", "", "", "", "", "", formNode);
  new Node(n->getId() + 0.1f, n->getUpos(), "", "", "", "", "", "", formNode);

  if (includeFeats) {
    new Node(n->getId() + 0.3f, Util::stringifyFeatMap(n->getFeats()), "", "",
             "", "", "", "", formNode);
  }

  std::map<float, Node *> nodesMap;
  nodesMap[n->getId()] = formNode;

  while (!nodes.empty()) {
    float frontId = nodes.front()->getId();

    Node *formNode =
        new Node(frontId + 0.2f, nodes.front()->getForm(), "", "", "", "", "",
                 "", nodesMap[nodes.front()->getParent()->getId()]);
    new Node(frontId + 0.1f, nodes.front()->getUpos(), "", "", "", "", "", "",
             formNode);
    new Node(frontId, nodes.front()->getDeprel(), "", "", "", "", "", "",
             formNode);

    if (includeFeats) {
      new Node(frontId + 0.3f,
               Util::stringifyFeatMap(nodes.front()->getFeats()), "", "", "",
               "", "", "", formNode);
    }

    nodesMap[frontId] = formNode;

    // add children of the head node to the stack
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return nodesMap[n->getId()];
}

Node *distort(Node *node, float prob, std::string csvProps) {
  Node *clone = node->copy();
  float rnd;
  std::vector<std::string> props = Util::stringSplit(csvProps, ',');

  std::queue<Node *> nodes;
  NodeList &children = clone->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  while (!nodes.empty()) {
    if (!nodes.front()->isIgnored()) {
      rnd = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
      if (rnd <= prob) {
        // distort
        for (std::string p : props) {
          rnd = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
          if (rnd <= prob) {
            getterptr getterFn = Node::getterByProp(p);
            setterptr setterFn = Node::setterByProp(p);
            std::string taboo = (nodes.front()->*getterFn)();
            std::string value = Util::getRandomProp(p);
            while (value == taboo) {
              value = Util::getRandomProp(p);
            }
            (nodes.front()->*setterFn)(value);
          }
        }
      }

      // add children of the head node to the stack
      NodeList &ch = nodes.front()->getChildren();
      for (int i = 0, len = ch.size(); i < len; i++) {
        nodes.push(ch[i]);
      }
    }
    nodes.pop();
  }
  return clone;
}
}  // namespace transformations
