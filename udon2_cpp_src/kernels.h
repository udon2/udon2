/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_KERNELS_H_
#define UDON2_CPP_SRC_KERNELS_H_

#include <map>
#include <string>

#include "Node.h"

namespace kernels {
class ConvPartialTreeKernel {
  std::string treeRepresentation;

  // the decay factor associated with the height of a partial tree
  float mu;

  // the decay factor associated with the length of the child sequences
  float lambda;

  // lambda ^ 2
  float lambda2;

  // cache for calculated delta values
  // id1 -> id2 -> delta
  // NOTE: each ID is float to accommodate GRCT, PCT and LCT,
  // where each rel node has ID equivalent to the original node
  //       each POS node has ID of the original node + 0.1
  //       each text node has ID of the original node + 0.2
  std::map<float, std::map<float, float> > deltas;

  float ptkDelta(Node *n1, Node *n2);
  float ptkSumDeltaP(NodeList ch1, NodeList ch2);

 public:
  ConvPartialTreeKernel(std::string, float, float);

  float eval(Node *root1, Node *root2);
};
}  // namespace kernels

#endif  // UDON2_CPP_SRC_KERNELS_H_
