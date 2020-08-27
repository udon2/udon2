#ifndef UD_NODE_H
#define UD_NODE_H

#include <algorithm>
#include <queue>
#include <string>
#include <vector>
#include <map>

class Node;
struct NodeComparator;

typedef std::map<std::string, std::string> FeatMap;
typedef std::string (Node::* getterptr)();


class NodeList {
    std::vector<Node*> nodes;

public:
    NodeList();
    NodeList(const NodeList& list);

    void push_sorted(Node* node);

    template <class Compare>
    void push_sorted(Node* node, Compare cmp) {
        nodes.insert(
            upper_bound(nodes.begin(), nodes.end(), node, cmp),
            node
        );
    }

    void push_back(Node* node) {
        nodes.push_back(node);
    }

    std::vector<Node*>::iterator findById(Node* n);

    int size() {
        return nodes.size();
    }

    Node* operator[](int i) const {
        // TODO: add negative indices
        return nodes[i];
    }

    std::vector<Node*>::iterator begin() {
        return nodes.begin();
    }

    std::vector<Node*>::iterator end() {
        return nodes.end();
    }

    std::vector<Node*>::iterator erase(std::vector<Node*>::iterator it) {
        return nodes.erase(it);
    }

    std::vector<Node*> getNodes() const {
        return nodes;
    }

    std::string toString();
};


typedef std::map<std::string, NodeList> GroupedNodes;

// represents token
class Node {
    std::string text;
    std::string pos;
    std::string rel;
    float id;
    std::string lemma;
    bool mIgnore = false;

    Node* parent;
    NodeList children;
    FeatMap ufeats; //m

    NodeList selectBy(std::string prop, std::string value, bool negate);
    NodeList getBy(std::string prop, std::string value);
    getterptr getterByProp(std::string prop);
    bool propExists(std::string prop, std::string value);
    bool childHasProp(std::string prop, std::string value);
    FeatMap parseMorphFeatures(std::string morph);
    NodeList selectHaving(std::string value, bool negate);
    void accumulateByRelChain(std::string value, NodeList* res, int depth);

    std::string _subtreeToString(int);
public:
    Node();
    Node(float id, std::string text, std::string lemma, std::string pos, std::string morph, 
        std::string rel, Node* parent);
    Node(Node* n);
    void init(float id, std::string text, std::string lemma, std::string pos, std::string morph,
        std::string rel, Node* parent);

    std::string getText();
    std::string getSubtreeText();
    float getId();
    std::string getLemma();
    std::string getPos();
    FeatMap getUfeats();
    std::string getRel();
    std::string getMorph();
    Node* getParent();
    NodeList getChildren();

    bool hasUfeat(std::string, std::string);

    NodeList getByRelChain(std::string value);

    NodeList getSubtreeNodes();

    bool isRoot();

    bool isIgnored();
    void ignore();
    void ignoreSubtree();
    void reset();
    void resetSubtree();

    Node* copy();
    void makeRoot();

    int subtreeSize();

    NodeList linear();
    NodeList linearSorted();
    
    template <class Compare>
    NodeList linearBy(Compare cmp);

    bool hasChildren();
    void copyChildren(Node* node);
    void addChild(Node* node);

    bool hasAllMorph(std::string value);

    NodeList selectByPos(std::string value);
    NodeList selectByLemma(std::string value);
    NodeList selectByRel(std::string value);
    NodeList selectByMorph(std::string value);
    NodeList selectByText(std::string value);
    NodeList selectByRelChain(std::string value);

    GroupedNodes groupBy(std::string prop);

    NodeList selectHavingMorph(std::string value);
    NodeList selectMissingMorph(std::string value);

    bool isIdentical(Node* node, std::string excludeProps);
    NodeList selectIdentical(Node* node);
    NodeList selectIdenticalExcept(Node* node, std::string excludeProps);
    NodeList select(std::string query);

    void prune(std::string rel);
    void removeChild(Node* node);

    NodeList selectExceptPos(std::string value);
    NodeList selectExceptLemma(std::string value);
    NodeList selectExceptRel(std::string value);
    NodeList selectExceptText(std::string value);
    
    NodeList getByPos(std::string value);
    NodeList getByLemma(std::string value);
    NodeList getByRel(std::string value);

    Node* textualIntersect(std::string text);

    bool posExists(std::string value);
    bool lemmaExists(std::string value);
    bool relExists(std::string value);
    bool textExists(std::string value);

    bool childHasPos(std::string value);
    bool childHasLemma(std::string value);
    bool childHasRel(std::string value);
    bool childHasText(std::string value);

    std::string toString();
    std::string subtreeToString();

    Node* toPCT();
    Node* toGRCT();
    Node* toLCT();
};


struct compare_node_by_id : public std::unary_function<Node*, bool> {
    Node* baseline;
    
    explicit compare_node_by_id (Node* baseline) : baseline(baseline) {}
    
    bool operator() (Node* arg) {
        return baseline->getId() == arg->getId();
    }
};

struct compare_node_by_subtree_size {
    bool operator()(Node* left, Node* right) {
        return left->subtreeSize() < right->subtreeSize();
    }
};

struct compare_node_by_string {
    bool operator()(Node* left, Node* right) {
        return left->toString() < right->toString();
    }
};

struct compare_node_by_text {
    bool operator()(Node* left, Node* right) {
        return left->getText() < right->getText();
    }
};

// Don't move - should be here to avoid errors at linking time
template <class Compare>
NodeList Node::linearBy(Compare cmp) {
    std::queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    NodeList linear;
    linear.push_back(this);

    while (!nodes.empty()) {        
        linear.push_sorted(nodes.front(), cmp);

        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }

    return linear;
}

#endif