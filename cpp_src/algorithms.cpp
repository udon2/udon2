#include <vector>
#include <iostream>
#include "algorithms.h"
#include <boost/python.hpp>

using namespace std;

namespace algorithms {
    int convSubTreeKernel(Node* root1, Node* root2) {
        int value = 0;

        // TODO: change to groupBy('rel') because we're interested in the same dep. structures
        //       although maybe applied to different pos tags???
        //       or maybe not really, since we want parallel structures?
        GroupedNodes gnodes1 = root1->groupBy("pos");
        GroupedNodes gnodes2 = root2->groupBy("pos");

        auto gn1 = gnodes1.begin();
        auto gn2 = gnodes2.begin();

        auto gn1_end = gnodes1.end();
        auto gn2_end = gnodes2.end();

        while (gn1 != gn1_end && gn2 != gn2_end) {
            if (gn1->first < gn2->first) {
                gn1++;
            } else if (gn1->first > gn2->first) {
                gn2++;
            } else {
                // means the pos tags are equal, time to check all nodes having them
                NodeList nodes1 = gn1->second;
                NodeList nodes2 = gn2->second;

                auto n1 = nodes1.begin();
                auto n2 = nodes2.begin();

                auto n1_end = nodes1.end();
                auto n2_end = nodes2.end();

                while (n1 != n1_end && n2 != n2_end) {
                    // cout << (*n1)->getText() << " (" << (*n1)->toString() << ") vs " << (*n2)->getText() << " (" << (*n2)->toString() << ") <- " << value << endl;

                    if ((*n1)->getPos() < (*n2)->getPos()) {
                        // cout << "option 1" << endl;
                        n1++;
                    } else if ((*n1)->getPos() > (*n2)->getPos()) {
                        // cout << "option 2" << endl;
                        n2++;
                    } else {
                        // cout << "option 3" << endl;
                        // means equal pos-tags
                        if (!(*n1)->getChildren().size() && !(*n2)->getChildren().size()) {
                            // terminals
                            value++;
                            // if ((*n1)->getText() == (*n2)->getText() || ((*n1)->getLemma() == (*n2)->getLemma())) {
                            //     value++;
                            // }
                            n1++;
                            n2++;
                        } else {
                            // non-terminals
                            auto n2_anchor = n2;
                            while (n1 != n1_end && (*n1)->getPos() == (*n2)->getPos()) {
                                while (n2 != n2_end && (*n1)->getPos() == (*n2)->getPos()) {
                                    NodeList children1 = (*n1)->getChildren();
                                    NodeList children2 = (*n2)->getChildren();

                                    vector<Node*>::iterator ch1 = children1.begin();
                                    vector<Node*>::iterator ch2 = children2.begin();

                                    vector<Node*>::iterator ch1_end = children1.end();
                                    vector<Node*>::iterator ch2_end = children2.end();

                                    int temp = 1;
                                    while (ch1 != ch1_end && ch2 != ch2_end) {
                                        if ((*ch1)->toString() < (*ch2)->toString()) {
                                            ch1++;
                                        } else if ((*ch1)->toString() > (*ch2)->toString()) {
                                            ch2++;
                                        } else {
                                            auto ch2_anchor = ch2;
                                            while (ch1 != ch1_end && (*ch1)->toString() == (*ch2)->toString()) {
                                                while (ch2 != ch2_end && (*ch1)->toString() == (*ch2)->toString()) {
                                                    // means the chains (productions) are equal, i.e.
                                                    // pos_head -> rel -> pos_dep are the same for both trees
                                                    // cout << "--> children" << endl;
                                                    // cout << (*ch1)->getSubtreeText() << " vs " << (*ch2)->getSubtreeText() << endl;
                                                    temp *= (1 + convSubTreeKernel(*ch1, *ch2));
                                                    ch2++;
                                                }
                                                ch1++;
                                                ch2 = ch2_anchor;
                                            }
                                        }
                                    }
                                    value += temp;
                                    n2++;
                                }
                                n1++;
                                n2 = n2_anchor;
                            }
                        }
                    }
                }
                gn1++;
                gn2++;
            }
        }
        // cout << "v -> " << value << endl;
        return value;
    }

    int convTreeKernel(Tree* t1, Tree* t2) {
        return convSubTreeKernel(t1->getRootWord(), t2->getRootWord());
    }
}


BOOST_PYTHON_MODULE(algorithms)
{
    using namespace boost::python;

    def("conv_subtree_kernel", algorithms::convSubTreeKernel);
    def("conv_tree_kernel", algorithms::convTreeKernel);
}