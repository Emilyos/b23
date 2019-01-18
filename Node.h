#ifndef B23_NODE_H
#define B23_NODE_H


#include "Key.h"
#include "Value.h"
#include "LCRS_BalancedTree.h"

/**represents an internal AND lead nodes in B+ DS*/
class Node {
private:
    friend class LCRS_BalancedTree;

    Node *leftChild;
    Node *rightSbiling;
    Node *parent;
    /**max key in this node subtreee, available only for internal Nodes*/
    Key *key;
    /** Used for leaf nodes ONLY, holds pointer to the actual data represented by key  */
    Value *value;
    /** used to link leaf nodes with each other as Linked List*/
    Node *rightNode, *leftNode;

    Node();

    ~Node();

    bool isLeaf();

    void updateMaxKey();

    void updateChildren(Node *l, Node *m, Node *r);

    void insertLeftTo(Node *node);

    void insertRightTo(Node *node);
};


#endif //B23_NODE_H
