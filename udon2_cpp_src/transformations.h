/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_TRANSFORMATIONS_H_
#define UDON2_CPP_SRC_TRANSFORMATIONS_H_

#include "Node.h"

namespace transformations {
Node* toPCT(Node* n, bool includeForm = true, bool includeFeats = false);
Node* toGRCT(Node* n, bool includeForm = true, bool includeFeats = false);
Node* toLCT(Node* n, bool includeFeats = false);
}  // namespace transformations

#endif  // UDON2_CPP_SRC_TRANSFORMATIONS_H_
