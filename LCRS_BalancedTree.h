#ifndef B23_LCRS_BALANCEDTREE_H
#define B23_LCRS_BALANCEDTREE_H

#include "Key.h"
#include "Value.h"
#include "Node.h"

class Node;

class LCRS_BalancedTree {
private:
    Node *root;
    unsigned size;

    Node *insertAndSplit(Node *parent, Node *vertex);

    Node *find(Node *root, const Key *key) const;

    bool key_equals(const Key *key1, const Key *key2) const;

    Node *borrowOrMerge(Node *);

    void clear(Node *root);

public:
    LCRS_BalancedTree(const Key *min, const Key *max);

    ~LCRS_BalancedTree();

    void Insert(const Key *nkey, const Value *nval);

    void Delete(const Key *dkey);

    Value *Search(const Key *key) const;

    unsigned Rank(const Key *key) const;

    const Key *Select(unsigned index) const;

    void Print_Values(const Key *key1, const Key *key2) const;
};


#endif //B23_LCRS_BALANCEDTREE_H
