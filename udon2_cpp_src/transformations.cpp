/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "transformations.h"

#include <queue>
#include <map>

namespace transformations {
Node *toPCT(Node *n) {
  std::queue<Node *> nodes;
  NodeList &children = n->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *rootRelNode =
      new Node(n->getId(), n->getDeprel(), "", "", "", "", "", "", NULL);
  Node *posNode = new Node(n->getId() + 0.1f, n->getUpos(), "", "", "", "", "",
                           "", rootRelNode);
  new Node(n->getId() + 0.2f, n->getForm(), "", "", "", "", "", "", posNode);

  std::map<float, Node *> nodesMap;
  nodesMap[n->getId()] = posNode;

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
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return rootRelNode;
}

Node *toGRCT(Node *n) {
  std::queue<Node *> nodes;
  NodeList &children = n->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *relNode =
      new Node(n->getId(), n->getDeprel(), "", "", "", "", "", "", NULL);
  Node *posNode = new Node(n->getId() + 0.1f, n->getUpos(), "", "", "", "", "",
                           "", relNode);
  new Node(n->getId() + 0.2f, n->getForm(), "", "", "", "", "", "", posNode);

  std::map<float, Node *> nodesMap;
  nodesMap[n->getId()] = relNode;

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
    NodeList &ch = nodes.front()->getChildren();
    for (int i = 0, len = ch.size(); i < len; i++) {
      nodes.push(ch[i]);
    }
    nodes.pop();
  }
  return nodesMap[n->getId()];
}

Node *toLCT(Node *n) {
  std::queue<Node *> nodes;
  NodeList &children = n->getChildren();
  for (int i = 0, len = children.size(); i < len; i++) {
    nodes.push(children[i]);
  }

  Node *formNode =
      new Node(n->getId() + 0.2f, n->getForm(), "", "", "", "", "", "", NULL);
  new Node(n->getId(), n->getDeprel(), "", "", "", "", "", "", formNode);
  new Node(n->getId() + 0.1f, n->getUpos(), "", "", "", "", "", "", formNode);

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
}  // namespace transformations
