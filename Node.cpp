#include "Node.h"
#include <cstddef>

Node::Node() : subLeft(NULL), subMiddle(NULL), subRight(NULL),
               parent(NULL), key(NULL), value(NULL),
               rightNode(NULL), leftNode(NULL) {}

bool Node::isLeaf() {
    return subLeft == NULL && subRight == NULL && subMiddle == NULL; // only leads have key;
}

void Node::updateMaxKey() {
    key = subLeft->key;
    if (subMiddle) key = subMiddle->key;
    if (subRight) key = subRight->key;
}

void Node::updateChildren(Node *l, Node *m, Node *r) {
    subLeft = l;
    subMiddle = m;
    subRight = r;
    l->parent = this;
    if (m) m->parent = this;
    if (r) r->parent = this;
    updateMaxKey();
}

void Node::insertLeftTo(Node *node) {
    Node *old_left = node->leftNode;
    node->leftNode = this;
    this->rightNode = node;
    this->leftNode = old_left;
    if (old_left != NULL) old_left->rightNode = this;
}

void Node::insertRightTo(Node *node) {
    Node *old_right = node->rightNode;
    node->rightNode = this;
    this->leftNode = node;
    this->rightNode = old_right;
    if (old_right != NULL) {
        old_right->leftNode = this;
    }

}

Node::~Node() {
    if (isLeaf()) {
        Node *old_left = leftNode;
        Node *old_right = rightNode;
        if (old_left != NULL) old_left->rightNode = old_right;
        if (old_right != NULL) old_right->leftNode = old_left;
        if (value)delete value;
        if (key) delete key;
    }
}

#include "Node.h"
