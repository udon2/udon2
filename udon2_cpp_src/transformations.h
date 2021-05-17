/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_TRANSFORMATIONS_H_
#define UDON2_CPP_SRC_TRANSFORMATIONS_H_

#include "Node.h"

namespace transformations {
Node* toPCT(Node* n, bool includeForm = true);
Node* toGRCT(Node* n, bool includeForm = true);
Node* toLCT(Node* n);
}  // namespace transformations

#endif  // UDON2_CPP_SRC_TRANSFORMATIONS_H_
