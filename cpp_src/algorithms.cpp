#include <vector>
#include <map>
#include <iostream>
#include "algorithms.h"

#include <boost/python.hpp>

using namespace std;

namespace algorithms {
    ConvPartialTreeKernel::ConvPartialTreeKernel(string repr, float mu, float lambda) : treeRepresentation(repr), mu(mu), lambda(lambda) {
        lambda2 = lambda * lambda;
    }

    float ConvPartialTreeKernel::ptkSumDeltaP(NodeList ch1, NodeList ch2) {
        float S = 0;
        int ch1_size = ch1.size();
        int ch2_size = ch2.size();

        int lmin = ch1_size < ch2_size ? ch1_size : ch2_size;

        map<int, map<int,float>> DPS;
        map<int, map<int,float>> DP;
        float* kernel_mat = new float[lmin];

        kernel_mat[0] = 0;
        for (int i = 1; i < ch1_size; i++) {
            for (int j = 1; j < ch2_size; j++) {
                if (ch1[i]->getText() == ch2[j]->getText()) {
                    DPS[i][j] = ptkDelta(ch1[i], ch2[j]);
                    kernel_mat[0] += DPS[i][j];
                } else {
                    DPS[i][j] = 0;
                }
            }
        }

        for (int j = 0; j < ch2_size; j++)
            DP[0][j] = 0;

        for (int i = 0; i < ch1_size; i++)
            DP[i][0] = 0;

        for (int l = 1; l < lmin; l++) {
            kernel_mat[l] = 0;

            for (int i = l; i < ch1_size; i++) {
                for (int j = l; j < ch2_size; j++) {
                    DP[i][j] = DPS[i][j] + lambda * DP[i][j-1] + lambda * DP[i-1][j] - lambda2 * DP[i-1][j-1];

                    if (ch1[i]->getText() == ch2[j]->getText()) {
                        DPS[i][j] = ptkDelta(ch1[i], ch2[j]) * DP[i-1][j-1];
                        kernel_mat[l] += DPS[i][j];
                    }
                }
            }

        }

        for (int i = 0; i < lmin; i++) {
            S += kernel_mat[i];
        }

        return S;
    }

    float ConvPartialTreeKernel::ptkDelta(Node* n1, Node* n2) {
        if (deltas.count(n1->getId()) > 0 && deltas[n1->getId()].count(n2->getId()) > 0) {
            return deltas[n1->getId()][n2->getId()];
        }

        if (n1->getText() != n2->getText()) {
            deltas[n1->getId()][n2->getId()] = 0;
        } else if (!n1->hasChildren() || !n2->hasChildren()) {
            deltas[n1->getId()][n2->getId()] = mu * lambda2;
        } else {
            deltas[n1->getId()][n2->getId()] = mu * (lambda2 + ptkSumDeltaP(n1->getChildren(), n2->getChildren()));
        }
        return deltas[n1->getId()][n2->getId()];
    }

    float ConvPartialTreeKernel::eval(Node* root1, Node* root2) {
        /*
         * Implementation of Fast PTK presented in
         * Moschitti, A. (2006, September). Efficient convolution kernels for dependency and constituent syntactic trees.
         * Available at: https://link.springer.com/content/pdf/10.1007/11871842_32.pdf
         * 
         * Needs one of the proposed representations for dependency trees, i.e.:
         * - POS-tag centered tree (PCT)
         * - Grammatical relation centered tree (GRCT)
         * - Lexical centered tree (LCT)
         * 
         * For now we set \mu and \lambda to 1
         */

        deltas.clear();
        float value = 0;

        Node* r1;
        Node* r2;
        if (treeRepresentation == "PCT") {
            r1 = root1->isRoot() ? root1->getChildren()[0]->toPCT() : root1->toPCT();
            r2 = root2->isRoot() ? root2->getChildren()[0]->toPCT() : root2->toPCT();
        } else if (treeRepresentation == "GRCT") {
            r1 = root1->isRoot() ? root1->getChildren()[0]->toGRCT() : root1->toGRCT();
            r2 = root2->isRoot() ? root2->getChildren()[0]->toGRCT() : root2->toGRCT();
        } else if (treeRepresentation == "LCT") {
            r1 = root1->isRoot() ? root1->getChildren()[0]->toLCT() : root1->toLCT();
            r2 = root2->isRoot() ? root2->getChildren()[0]->toLCT() : root2->toLCT();
        } else {
            return -1;
        }

        // The labels are stored in the text field
        NodeList L1 = r1->linearBy(compare_node_by_text());
        NodeList L2 = r2->linearBy(compare_node_by_text());

        auto l1 = L1.begin();
        auto l2 = L2.begin();

        auto l1_end = L1.end();
        auto l2_end = L2.end();


        while (l1 != l1_end && l2 != l2_end) {
            if ((*l1)->getText() > (*l2)->getText()) {
                l2++;
            } else if ((*l1)->getText() < (*l2)->getText()) {
                l1++;
            } else {
                auto l2_old = l2;
                while (l1 != l1_end && (*l1)->getText() == (*l2)->getText()) {
                    while (l2 != l2_end && (*l1)->getText() == (*l2)->getText()) {
                        value += ptkDelta(*l1, *l2);
                        l2++;
                    }
                    l1++;
                    l2 = l2_old;
                }
            }
        }

        // cout << "v -> " << value << endl;
        return value;
    }
}


BOOST_PYTHON_MODULE(algorithms)
{
    using namespace boost::python;

    class_<algorithms::ConvPartialTreeKernel>("ConvPartialTreeKernel", init<std::string, float, float>())
        .def("__call__", &algorithms::ConvPartialTreeKernel::eval)
        ;
}