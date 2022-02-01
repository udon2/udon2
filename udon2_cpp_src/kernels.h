/*
 * Copyright 2021 Dmytro Kalpakchi
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
  long double p_mu;

  // the decay factor associated with the length of the child sequences
  // we use lambda_ and not lambda, since "lambda" is a Python keyword
  long double p_lambda;

  // lambda ^ 2
  long double lambda2;

  // whether to include lexicals in the transformation
  bool includeForm;

  // whether to include universal features in the transformation
  bool includeFeats;

  // cache for calculated delta values
  // id1 -> id2 -> delta
  // NOTE: each ID is long double to accommodate GRCT, PCT and LCT,
  // where each rel node has ID equivalent to the original node
  //       each POS node has ID of the original node + 0.1
  //       each text node has ID of the original node + 0.2
  std::map<long double, std::map<long double, long double> > deltas;

  long double ptkDelta(Node *n1, Node *n2);
  long double ptkSumDeltaP(NodeList ch1, NodeList ch2);

 public:
  ConvPartialTreeKernel(std::string, long double p_mu = 1.0f,
                        long double p_lambda = 1.0f, bool includeForm = true,
                        bool includeFeats = false);

  // TODO(dmytro): Sometimes can get infinite?
  long double eval(Node *root1, Node *root2);
};
}  // namespace kernels

#endif  // UDON2_CPP_SRC_KERNELS_H_
