#ifndef UD_NODE_H
#define UD_NODE_H

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

    void push_back(Node* node) {
        nodes.push_back(node);
    }

    int size() {
        return nodes.size();
    }

    Node* operator[](int i) const {
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
};


typedef std::map<std::string, NodeList> GroupedNodes;

// represents token
class Node {
    std::string text;
    std::string pos;
    std::string rel;
    int id;
    std::string lemma;
    bool mIgnore = false;

    Node* parent;
    NodeList children;
    FeatMap ufeats; //m

    NodeList selectBy(std::string prop, std::string value, bool negate);
    NodeList immediateBy(std::string prop, std::string value);
    getterptr getterByProp(std::string prop);
    bool propExists(std::string prop, std::string value);
    bool childHasProp(std::string prop, std::string value);
    FeatMap parseMorphFeatures(std::string morph);
    NodeList selectHaving(std::string value, bool negate);

public:
    Node();
    Node(int id, std::string text, std::string lemma, std::string pos, std::string morph, 
        std::string rel, Node* parent);
    Node(Node* n);
    void init(int id, std::string text, std::string lemma, std::string pos, std::string morph,
        std::string rel, Node* parent);

    std::string getText();
    std::string getSubtreeText();
    int getId();
    std::string getLemma();
    std::string getPos();
    FeatMap getUfeats();
    std::string getRel();
    std::string getMorph();
    Node* getParent();
    NodeList getChildren();

    Node* getByRelChain(std::string value);

    NodeList getSubtreeNodes();

    bool isIgnored();
    void ignore();
    void reset();
    void resetSubtree();

    Node* copy();
    void makeRoot();

    int subtreeSize();

    NodeList linear();
    NodeList linearSorted();

    bool hasChildren();
    void copyChildren(Node* node);
    void addChild(Node* node);

    bool hasAllMorph(std::string value);

    NodeList selectByPos(std::string value);
    NodeList selectByLemma(std::string value);
    NodeList selectByRel(std::string value);
    NodeList selectByMorph(std::string value);
    NodeList selectByText(std::string value);
    // NodeList selectByRelChain(std::string value);

    GroupedNodes groupBy(std::string prop);

    NodeList selectHavingMorph(std::string value);
    NodeList selectMissingMorph(std::string value);

    bool isIdentical(Node* node, std::string excludeProps);
    NodeList selectIdentical(Node* node);
    NodeList identicalExcept(Node* node, std::string excludeProps);
    NodeList select(std::string query);

    void prune(std::string rel);
    void removeChild(Node* node);

    NodeList selectExceptPos(std::string value);
    NodeList selectExceptLemma(std::string value);
    NodeList selectExceptRel(std::string value);
    
    NodeList immediateByPos(std::string value);
    NodeList immediateByLemma(std::string value);
    NodeList immediateByRel(std::string value);

    Node* textualIntersect(std::string text);

    bool posExists(std::string value);
    bool lemmaExists(std::string value);
    bool relExists(std::string value);

    bool childHasPos(std::string value);
    bool childHasLemma(std::string value);
    bool childHasRel(std::string value);

    std::string toString();
};


struct NodeComparator {
    bool operator()(Node* left, Node* right) {
        return left->toString() < right->toString();
    }
};

#endif