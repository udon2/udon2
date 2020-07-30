#include <queue>
#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>
#include "Util.h"
#include "Node.h"

using namespace std;

/*
 * NodeList methods
 */

NodeList::NodeList() {}

NodeList::NodeList(const NodeList& list) {
    this->nodes = list.getNodes();
}

void NodeList::push_sorted(Node* node) {
    // inserts in a lexicographical order
    nodes.insert(
        upper_bound(nodes.begin(), nodes.end(), node, NodeComparator()),
        node
    );
}


/*
 * Node methods
 */
Node::Node()
    : text("root"), pos("ROOT"), rel(""), id(-1), lemma(""), parent(NULL) {
    this->mIgnore = false;
}

Node::Node(int id, string text, string lemma, string pos, string morph, string rel, Node* parent)
    : text(text), pos(pos), rel(rel), id(id), lemma(lemma), parent(parent)
{
    this->ufeats = parseMorphFeatures(morph);
    this->mIgnore = false;
}

Node::Node(Node* n) {
    this->id = n->getId();
    this->text = n->getText();
    this->lemma = n->getLemma();
    this->pos = n->getPos();
    this->rel = n->getRel();
    this->parent = n->getParent();
    this->ufeats = n->getUfeats();
    this->copyChildren(n);
    this->mIgnore = n->isIgnored();
}

void Node::init(int id, string text, string lemma, string pos, string morph, string rel, Node* parent)
{
    this->id = id;
    this->text = text;
    this->lemma = lemma;
    this->pos = pos;
    this->rel = rel;
    this->parent = parent;
    this->ufeats = parseMorphFeatures(morph);
    this->mIgnore = false;
}

string Node::getText() {
    return text;
}

int Node::getId() {
    return id;
}

string Node::getLemma() {
    return lemma;
}

string Node::getPos() {
    return pos;
}

FeatMap Node::parseMorphFeatures(string morph) {
    FeatMap ufeats = FeatMap();
    vector<string> feats = Util::stringSplit(morph, '|');
    for (string f : feats) {
        vector<string> kv = Util::stringSplit(f, '=');
        if (kv.size() == 2) {
            ufeats[kv[0]] = kv[1];
        }
    }
    return ufeats;
}

FeatMap Node::getUfeats() {
    return ufeats;
}

string Node::getMorph() {
    // Create a map iterator and point to beginning of map
    map<string, string>::iterator it = ufeats.begin();

    vector<string> feats;
 
    // Iterate over the map using Iterator till end.
    while (it != ufeats.end()) {
        feats.push_back(it->first + "=" + it->second);
 
        // Increment the Iterator to point to next entry
        it++;
    }

    sort(feats.begin(), feats.end()); 

    return Util::stringJoin(feats, '|');
}

string Node::getRel() {
    return rel;
}

Node* Node::getParent() {
    return parent;
}

bool Node::hasChildren() {
    return children.size() > 0;
}

NodeList Node::getChildren() {
    // children are sorted by means of NodeComparator
    return children;
}

NodeList Node::getSubtreeNodes() {
    NodeList all;
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
        all.push_back(children[i]);
    }

    while (!nodes.empty()) {        
        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
            all.push_back(ch[i]);
        } 
        nodes.pop();
    }

    return all;
}

Node* Node::getByRelChain(string value) {
    // just follow a rel chain from the current node and return whatever node is in the end if exists
    // otherwise return NULL
    // can input chains like .conj.obl
    if (value.empty()) return NULL;

    vector<string> chain = Util::stringSplit(value, '.');

    vector<Node*>::iterator node = children.begin();
    while (node != children.end()) {
        if ((*node)->getRel() == chain[0]) {
            if (chain.size() == 1) {
                // TODO: do not erase directly, but mark it instead
                //       should be able to reset it later
                return *node;
            } else {
                vector<string> subChain(chain.begin() + 1, chain.end());
                return (*node)->getByRelChain(Util::stringJoin(subChain, '.'));
            }
        } else {
            ++node;
        }
    }
    return NULL;
}

bool Node::isIgnored() {
    return mIgnore;
}

void Node::ignore() {
    this->mIgnore = true;
}

void Node::reset() {
    // TODO: a more generic reset from pruning?
    this->mIgnore = false;
}

void Node::ignoreSubtree() {
    this->ignore();
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    while (!nodes.empty()) {
        // add children of the head node to the stack
        nodes.front()->ignore();

        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }
}

void Node::resetSubtree() {
    this->reset();
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    while (!nodes.empty()) {
        // add children of the head node to the stack
        nodes.front()->reset();

        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }
}

int Node::subtreeSize() {
    int sz = 0;
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
        sz++;
    }

    while (!nodes.empty()) {
        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
            sz++;
        } 
        nodes.pop();
    }
    return sz;
}

void Node::copyChildren(Node* node) {
    children = NodeList();
    for (Node* n : node->getChildren()) {
        children.push_sorted(new Node(n));
    }
}

void Node::addChild(Node* node) {
    children.push_sorted(node);
}

string Node::getSubtreeText() {
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    map<int, string> words;
    map<int, bool> isPunct;
    int wordsNum;
    if (!isIgnored()) {
        words[id] = text;
        wordsNum = 1;
    } else {
        wordsNum = 0;
    }
        

    while (!nodes.empty()) {
        if (!nodes.front()->isIgnored() && words.find(nodes.front()->getId()) == words.end()) {
            words[nodes.front()->getId()] = nodes.front()->getText();
            isPunct[nodes.front()->getId()] = nodes.front()->getPos() == "PUNCT";
            wordsNum++;
        }
        
        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }

    ostringstream os;
    int j = 0;
    for (std::map<int, string>::iterator it = words.begin(); it != words.end(); it++) {
        if (j > 0 && j < wordsNum && !isPunct[it->first]) os << " ";
        j++;
        os << it->second;
    }
    return os.str();
}

// turn a subtree into a linear structure
NodeList Node::linear() {
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    map<int, Node*> words;
    words[id] = this;

    while (!nodes.empty()) {
        if (words.find(nodes.front()->getId()) == words.end()) {
            words[nodes.front()->getId()] = nodes.front();
        }
        
        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }

    NodeList linear;
    for (std::map<int, Node*>::iterator it = words.begin(); it != words.end(); it++) {
        linear.push_back(it->second);
    }
    return linear;
}


NodeList Node::linearSorted() {
    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    NodeList linear;
    linear.push_back(this);

    while (!nodes.empty()) {        
        linear.push_sorted(nodes.front());

        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }

    return linear;
}

getterptr Node::getterByProp(string prop) {
    getterptr getterFn = NULL;
    if (prop == "pos")
        getterFn = &Node::getPos;
    else if (prop == "lemma")
        getterFn = &Node::getLemma;
    else if (prop == "rel")
        getterFn = &Node::getRel;
    else if (prop == "morph")
        getterFn = &Node::getMorph;
    else if (prop == "text")
        getterFn = &Node::getText;
    return getterFn;
}


/*
 * Select by a node by a property from the subtree induced by the node
 * NOTE: including the node itself
 */
NodeList Node::selectBy(string prop, string value, bool negate) {
    getterptr getterFn = getterByProp(prop);
    if (getterFn == NULL) return NodeList();

    queue<Node*> nodes;
    NodeList result;

    string nodeValue = (this->*getterFn)();
    if ((!negate && nodeValue == value) || (negate && nodeValue != value)) {
        result.push_back(this);
    }

    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    while (!nodes.empty()) {
        // check the POS-tag and if matches add to the result
        if (!nodes.front()->isIgnored()) {
            string nodeValue = (nodes.front()->*getterFn)();

            if ((!negate && nodeValue == value) || (negate && nodeValue != value)) {
                result.push_back(nodes.front());
            }
            
            // add children of the head node to the stack
            NodeList ch = nodes.front()->getChildren();
            for (int i = 0, len = ch.size(); i < len; i++) {
                nodes.push(ch[i]);
            } 
        }
        nodes.pop();
    }
    return result;
}

NodeList Node::selectByPos(string value) {
    return selectBy("pos", value, false);
}

NodeList Node::selectByLemma(string value) {
    return selectBy("lemma", value, false);
}

NodeList Node::selectByRel(string value) {
    return selectBy("rel", value, false);
}

NodeList Node::selectByMorph(string value) {
    vector<string> v = Util::stringSplit(value, '|');
    sort(v.begin(), v.end());
    return selectBy("morph", Util::stringJoin(v, '|'), false);
}

NodeList Node::selectByText(string value) {
    return selectBy("text", value, false);
}

NodeList Node::selectExceptPos(string value) {
    return selectBy("pos", value, true);
}

NodeList Node::selectExceptLemma(string value) {
    return selectBy("lemma", value, true);
}

NodeList Node::selectExceptRel(string value) {
    return selectBy("rel", value, true);
}

NodeList Node::selectExceptText(string value) {
    return selectBy("text", value, true);
}

void Node::accumulateByRelChain(string value, NodeList* res, int depth) {
    if (value.empty()) return;

    vector<string> chain = Util::stringSplit(value, '.');

    NodeList list = depth > 0 ? children : selectByRel(chain[0]);

    vector<Node*>::iterator node = list.begin();
    while (node != list.end()) {
        if ((*node)->getRel() == chain[0]) {
            if (chain.size() == 1) {
                // TODO: do not erase directly, but mark it instead
                //       should be able to reset it later
                res->push_back(*node);
            } else {
                vector<string> subChain(chain.begin() + 1, chain.end());
                (*node)->accumulateByRelChain(Util::stringJoin(subChain, '.'), res, depth + 1);
            }
        }
        ++node;
    }
    return;
}

NodeList Node::selectByRelChain(string value) {
    // return all possible nodes satisfying the relchain
    NodeList res;
    accumulateByRelChain(value, &res, 0);
    return res;
}


GroupedNodes Node::groupBy(string prop) {
    getterptr getterFn = getterByProp(prop);
    if (getterFn == NULL) return GroupedNodes();

    GroupedNodes gn = GroupedNodes();
    // (nodes.front()->*getterFn)()
    NodeList linearOrder = linear();

    for (Node* n : linearOrder) {
        string val = (n->*getterFn)();
        if (gn.find(val) == gn.end()) {
            gn[val] = NodeList();
        }
        gn[val].push_sorted(n);
    }
    return gn;
}


bool Node::hasAllMorph(string value) {
    FeatMap feats = parseMorphFeatures(value);
    for (auto it = feats.begin(); it != feats.end(); it++) {
        if (ufeats.find(it->first) == ufeats.end() || ufeats[it->first] != it->second) {
            return false;
        }
    }
    return true;
}

NodeList Node::selectHaving(string value, bool negate) {
    queue<Node*> nodes;
    NodeList result;

    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    FeatMap feats = parseMorphFeatures(value);

    while (!nodes.empty()) {
        // check the POS-tag and if matches add to the result
        FeatMap nodeFeats = nodes.front()->getUfeats();

        bool match = true;
        for (auto it = feats.begin(); it != feats.end(); it++) {
            if (nodeFeats.find(it->first) == nodeFeats.end() || nodeFeats[it->first] != it->second) {
                match = false;
                break;
            }
        }
            
        if ((match && !negate) || (!match && negate)) {
            result.push_back(nodes.front());
        }
        
        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }
    return result;
}

NodeList Node::selectHavingMorph(string value) {
    return selectHaving(value, false);
}

NodeList Node::selectMissingMorph(string value) {
    return selectHaving(value, true);
}

bool Node::isIdentical(Node* node, string excludeProps) {
    bool posIncluded = true;
    bool relIncluded = true;
    bool textIncluded = true;
    bool lemmaIncluded = true;

    if (!excludeProps.empty()) {
        vector<string> banned = Util::stringSplit(excludeProps, ',');
    
        for (vector<string>::size_type i = 0; i < banned.size(); i++) {
            if (banned[i] == "pos") {
                posIncluded = false;
            } else if (banned[i] == "rel") {
                relIncluded = false;
            } else if (banned[i] == "lemma") {
                lemmaIncluded = false;
            } else if (banned[i] == "text") {
                textIncluded = false;
            }
        }    
    }
    return (!posIncluded || pos == node->getPos()) && 
           (!relIncluded || rel == node->getRel()) && 
           (!lemmaIncluded || lemma == node->getLemma()) &&
           (!textIncluded || text == node->getText());
}

/*
 * Query should be a csv list of properties to exclude
 * if all properties should be included, pass an empty string
 */
NodeList Node::selectIdenticalExcept(Node* node, string excludeProps) {
    queue<Node*> nodes;
    NodeList result;

    bool posIncluded = true;
    bool relIncluded = true;
    bool textIncluded = true;
    bool lemmaIncluded = true;

    if (!excludeProps.empty()) {
        vector<string> banned = Util::stringSplit(excludeProps, ',');
    
        for (vector<string>::size_type i = 0; i < banned.size(); i++) {
            if (banned[i] == "pos") {
                posIncluded = false;
            } else if (banned[i] == "rel") {
                relIncluded = false;
            } else if (banned[i] == "lemma") {
                lemmaIncluded = false;
            } else if (banned[i] == "text") {
                textIncluded = false;
            }
        }    
    }

    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    while (!nodes.empty()) {
        // check the POS-tag and if matches add to the result
        Node* front = nodes.front();

        if ((!posIncluded || front->getPos() == node->getPos()) && 
            (!relIncluded || front->getRel() == node->getRel()) && 
            (!lemmaIncluded || front->getLemma() == node->getLemma()) &&
            (!textIncluded || front->getText() == node->getText())) {
            result.push_back(front);
        }
        
        // add children of the head node to the stack
        NodeList ch = front->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }
    return result;
}

NodeList Node::selectIdentical(Node* node) {
    return selectIdenticalExcept(node, "");
}

/*
 * Query language is a pretty simple boolean language. Possible properties: pos, rel, lemma.
 * Example queries:
 * > pos = ADJ
 * > pos = AUX && rel = cop
 * > (pos = ADJ || pos = NOUN) && rel = cop
 */
NodeList Node::select(string query) {
    // TODO: implement this later
    return NodeList();
}

/*
 *  Select an immediate child of the node with the property `prop` equal to `value`
 */
NodeList Node::immediateBy(string prop, string value) {
    getterptr getterFn = getterByProp(prop);
    if (getterFn == NULL) return NodeList();

    NodeList result;

    for (int i = 0, len = children.size(); i < len; i++) {
        if ((children[i]->*getterFn)() == value) {
            result.push_back(children[i]);
        }
    }
    return result;
}

NodeList Node::immediateByPos(string value) {
    return immediateBy("pos", value);
}

NodeList Node::immediateByLemma(string value) {
    return immediateBy("lemma", value);
}

NodeList Node::immediateByRel(string value) {
    return immediateBy("rel", value);
}


void Node::prune(string rel) {
    // can input chains like .conj.obl
    if (rel.empty()) return;

    vector<string> chain = Util::stringSplit(rel, '.');

    vector<Node*>::iterator node = children.begin();
    while (node != children.end()) {
        if ((*node)->getRel() == chain[0]) {
            if (chain.size() == 1) {
                // TODO: do not erase directly, but mark it instead
                //       should be able to reset it later?
                node = children.erase(node);
            } else {
                vector<string> subChain(chain.begin() + 1, chain.end());
                (*node)->prune(Util::stringJoin(subChain, '.'));
                ++node;
            }
        } else {
            ++node;
        }
    }
}

void Node::removeChild(Node* node) {
    vector<Node*>::iterator it = find(children.begin(), children.end(), node);
    if (it != children.end())
        children.erase(it);
}


/*
 * Check if the subtree induced by the node has a node with the prop
 * NOTE: excluding the node itself
 */
bool Node::propExists(string prop, string value) {
    getterptr getterFn = getterByProp(prop);
    if (getterFn == NULL) return false;

    queue<Node*> nodes;
    for (int i = 0, len = children.size(); i < len; i++) {
        nodes.push(children[i]);
    }

    while (!nodes.empty()) {
        // check the POS-tag and if matches add to the result
        if ((nodes.front()->*getterFn)() == value) {
            return true;
        }
        
        // add children of the head node to the stack
        NodeList ch = nodes.front()->getChildren();
        for (int i = 0, len = ch.size(); i < len; i++) {
            nodes.push(ch[i]);
        } 
        nodes.pop();
    }
    return false;
}

bool Node::posExists(string value) {
    return propExists("pos", value);
}

bool Node::relExists(string value) {
    return propExists("rel", value);
}

bool Node::lemmaExists(string value) {
    return propExists("lemma", value);
}

bool Node::textExists(string value) {
    return propExists("text", value);
}

/*
 * Check if the subtree induced by the node has a node with the prop
 * NOTE: excluding the node itself
 */
bool Node::childHasProp(string prop, string value) {
    getterptr getterFn = getterByProp(prop);
    if (getterFn == NULL) return false;

    for (int i = 0, len = children.size(); i < len; i++) {
        if ((children[i]->*getterFn)() == value) {
            return true;
        }
    }
    return false;
}

bool Node::childHasPos(string value) {
    return childHasProp("pos", value);
}

bool Node::childHasRel(string value) {
    return childHasProp("rel", value);
}

bool Node::childHasLemma(string value) {
    return childHasProp("lemma", value);
}

bool Node::childHasText(string value) {
    return childHasProp("text", value);
}

Node* Node::copy() {
    return new Node(this);
}

void Node::makeRoot() {
    Node* newRoot = new Node();
    this->rel = "root";
    this->parent = newRoot;
}

string Node::toString() {
    return pos + "|" + rel + "|" + text;
}

Node* Node::textualIntersect(string text) {
    // TODO: maybe return a NodeList instead?
    vector<string> words = Util::stringSplit(text, ' ');
    
    Node* root = NULL;

    for (string word : words) {
        NodeList nodes = selectByText(word);
        for (auto n : nodes) {
            while (n->getParent() != NULL) {
                // NOTE: this finds only contiguous pieces of text
                if (n->getSubtreeText().find(text) != string::npos) {
                    root = n;
                    break;
                } else {
                    n = n->getParent();
                }
            }

            if (root != NULL) break;
        }
        if (root != NULL) break;
    }
    return root;
}