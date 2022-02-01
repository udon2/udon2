/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include "kernels.h"

#include <iostream>
#include <map>
#include <vector>

#include "transformations.h"

namespace kernels {
ConvPartialTreeKernel::ConvPartialTreeKernel(std::string repr, long double p_mu,
                                             long double p_lambda,
                                             bool includeForm,
                                             bool includeFeats)
    : treeRepresentation(repr),
      p_mu(p_mu),
      p_lambda(p_lambda),
      includeForm(includeForm),
      includeFeats(includeFeats) {
  lambda2 = p_lambda * p_lambda;
}

long double ConvPartialTreeKernel::ptkSumDeltaP(NodeList ch1, NodeList ch2) {
  long double S = 0;
  int ch1_size = ch1.size();
  int ch2_size = ch2.size();

  int lmin = ch1_size < ch2_size ? ch1_size : ch2_size;

  std::map<int, std::map<int, long double> > DPS;
  std::map<int, std::map<int, long double> > DP;
  long double *kernel_mat = new long double[lmin];

  kernel_mat[0] = 0;
  for (int i = 1; i <= ch1_size; i++) {
    for (int j = 1; j <= ch2_size; j++) {
      if (ch1[i - 1]->getForm() == ch2[j - 1]->getForm()) {
        DPS[i][j] = ptkDelta(ch1[i - 1], ch2[j - 1]);
        kernel_mat[0] += DPS[i][j];
      } else {
        DPS[i][j] = 0;
      }
    }
  }

  // for (int j = 0; j <= ch2_size; j++) DP[0][j] = 0;

  // for (int i = 0; i <= ch1_size; i++) DP[i][0] = 0;

  for (int l = 1; l < lmin; l++) {
    kernel_mat[l] = 0;

    for (int j = 0; j <= ch2_size; j++) DP[l - 1][j] = 0;

    for (int i = 0; i <= ch1_size; i++) DP[i][l - 1] = 0;

    for (int i = l; i <= ch1_size; i++) {
      for (int j = l; j <= ch2_size; j++) {
        DP[i][j] = DPS[i][j] + p_lambda * DP[i][j - 1] +
                   p_lambda * DP[i - 1][j] - lambda2 * DP[i - 1][j - 1];

        if (ch1[i - 1]->getForm() == ch2[j - 1]->getForm()) {
          DPS[i][j] = ptkDelta(ch1[i - 1], ch2[j - 1]) * DP[i - 1][j - 1];
          kernel_mat[l] += DPS[i][j];
        }
      }
    }
  }

  for (int i = 0; i < lmin; i++) {
    S += kernel_mat[i];
  }

  delete[] kernel_mat;

  return S;
}

long double ConvPartialTreeKernel::ptkDelta(Node *n1, Node *n2) {
  if (deltas.count(n1->getId()) > 0 &&
      deltas[n1->getId()].count(n2->getId()) > 0) {
    return deltas[n1->getId()][n2->getId()];
  }

  if (n1->getForm() != n2->getForm()) {
    deltas[n1->getId()][n2->getId()] = 0;
  } else if (!n1->isLeaf() || !n2->isLeaf()) {
    deltas[n1->getId()][n2->getId()] = p_mu * lambda2;
  } else {
    deltas[n1->getId()][n2->getId()] =
        p_mu * (lambda2 + ptkSumDeltaP(n1->getChildren(), n2->getChildren()));
  }
  return deltas[n1->getId()][n2->getId()];
}

long double ConvPartialTreeKernel::eval(Node *root1, Node *root2) {
  /*
   * Implementation of Fast PTK presented in
   * Moschitti, A. (2006, September). Efficient convolution kernels for
   * dependency and constituent syntactic trees. Available at:
   * https://link.springer.com/content/pdf/10.1007/11871842_32.pdf
   *
   * Needs one of the proposed representations for dependency trees, i.e.:
   * - POS-tag centered tree (PCT)
   * - Grammatical relation centered tree (GRCT)
   * - Lexical centered tree (LCT)
   * - Syntax-only GRCT or PCT, i.e., excluding forms (soGRCT and soPCT
   * respectively)
   *
   * For now we set \mu and \lambda to 1
   */

  deltas.clear();
  long double value = 0;

  Node *r1;
  Node *r2;
  if (treeRepresentation == "PCT") {
    r1 = root1->isRoot()
             ? transformations::toPCT(root1->getChildren()[0], includeForm,
                                      includeFeats)
             : transformations::toPCT(root1, includeForm, includeFeats);
    r2 = root2->isRoot()
             ? transformations::toPCT(root2->getChildren()[0], includeForm,
                                      includeFeats)
             : transformations::toPCT(root2, includeForm, includeFeats);
  } else if (treeRepresentation == "GRCT") {
    r1 = root1->isRoot()
             ? transformations::toGRCT(root1->getChildren()[0], includeForm,
                                       includeFeats)
             : transformations::toGRCT(root1, includeForm, includeFeats);
    r2 = root2->isRoot()
             ? transformations::toGRCT(root2->getChildren()[0], includeForm,
                                       includeFeats)
             : transformations::toGRCT(root2, includeForm, includeFeats);
  } else if (treeRepresentation == "LCT") {
    r1 = root1->isRoot()
             ? transformations::toLCT(root1->getChildren()[0], includeFeats)
             : transformations::toLCT(root1, includeFeats);
    r2 = root2->isRoot()
             ? transformations::toLCT(root2->getChildren()[0], includeFeats)
             : transformations::toLCT(root2, includeFeats);
  } else {
    return -1;
  }

  // The labels are stored in the form field
  NodeList L1 = r1->linearBy(compare_node_by_form());
  NodeList L2 = r2->linearBy(compare_node_by_form());

  auto l1 = L1.begin();
  auto l2 = L2.begin();

  auto l1_end = L1.end();
  auto l2_end = L2.end();

  while (l1 != l1_end && l2 != l2_end) {
    if ((*l1)->getForm() > (*l2)->getForm()) {
      l2++;
    } else if ((*l1)->getForm() < (*l2)->getForm()) {
      l1++;
    } else {
      auto l2_old = l2;
      while (l1 != l1_end && (*l1)->getForm() == (*l2)->getForm()) {
        while (l2 != l2_end && (*l1)->getForm() == (*l2)->getForm()) {
          value += ptkDelta(*l1, *l2);
          l2++;
        }
        l1++;
        l2 = l2_old;
      }
    }
  }

  delete r1;
  delete r2;

  return value;
}
}  // namespace kernels
