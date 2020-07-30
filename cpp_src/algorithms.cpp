#include <vector>
#include <iostream>
#include "algorithms.h"
#include <boost/python.hpp>

using namespace std;

namespace algorithms {
    int convTreeKernel(Node* root1, Node* root2) {
        int value = 0;

        // TODO: change to groupBy('rel') because we're interested in the same dep. structures
        //       although maybe applied to different pos tags???
        //       or maybe not really, since we want parallel structures?
        GroupedNodes gnodes1 = root1->groupBy("rel");
        GroupedNodes gnodes2 = root2->groupBy("rel");

        auto gn1 = gnodes1.begin();
        auto gn2 = gnodes2.begin();

        auto gn1_end = gnodes1.end();
        auto gn2_end = gnodes2.end();

        while (gn1 != gn1_end && gn2 != gn2_end) {
            if (gn1->first < gn2->first) {
                gn1++;
            } else if (gn1->first > gn2->first) {
                gn2++;
            } else if (!gn1->first.empty()) {
                // means the dependency relations are equal, time to check if the nodes on both ends have the same pos-tags
                NodeList nodes1 = gn1->second;
                NodeList nodes2 = gn2->second;

                auto n1 = nodes1.begin();
                auto n2 = nodes2.begin();

                auto n1_end = nodes1.end();
                auto n2_end = nodes2.end();

                while (n1 != n1_end && n2 != n2_end) {
                    if ((*n1)->getPos() < (*n2)->getPos()) {
                        n1++;
                    } else if ((*n1)->getPos() > (*n2)->getPos()) {
                        n2++;
                    } else {
                        // means the POS-tags of dependents are the same, check pos-tags of heads
                        Node* p1 = (*n1)->getParent();
                        Node* p2 = (*n2)->getParent();
                        if (p1->getPos() == p2->getPos()) {
                            if (!(*n1)->getChildren().size() && !(*n2)->getChildren().size()) {
                                // means p1 and p2 are pre-terminals since n1 and n2 are terminals
                                value++;
                            } else {
                                int temp = 1;

                                NodeList n1_children = (*n1)->getChildren();
                                NodeList n2_children = (*n2)->getChildren();

                                auto n1_ch = n1_children.begin();
                                auto n2_ch = n2_children.begin();

                                auto n1_ch_end = n1_children.end();
                                auto n2_ch_end = n2_children.end();

                                while (n1_ch != n1_ch_end) {
                                    while (n2_ch != n2_ch_end) {
                                        temp *= (1 + convTreeKernel(*n1_ch, *n2_ch));
                                        n2_ch++;
                                    }
                                    n1_ch++;
                                }

                                value += temp;
                            }
                        }
                        n1++;
                        n2++;
                    }
                }
                
                gn1++;
                gn2++;
            }
        }
        // cout << "v -> " << value << endl;
        return value;
    }
}


BOOST_PYTHON_MODULE(algorithms)
{
    using namespace boost::python;

    def("conv_tree_kernel", algorithms::convTreeKernel);
}