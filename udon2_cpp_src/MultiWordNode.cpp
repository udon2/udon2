/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "MultiWordNode.h"

MultiWordNode::MultiWordNode(int minId, int maxId, std::string token)
    : minId(minId), maxId(maxId), token(token) {
  ref = maxId - minId + 1;
}

MultiWordNode::MultiWordNode(MultiWordNode* mw) {
  minId = mw->getMinId();
  maxId = mw->getMaxId();
  token = mw->getToken();
  ref = mw->getRef();
}
