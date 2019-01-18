#include "LCRS_BalancedTree.h"
#include <cstddef>

LCRS_BalancedTree::LCRS_BalancedTree(const Key *min, const Key *max) : size(0) {
    root = new Node(); // internal
    Node *left = new Node(); // the -infinity leaf
    Node *middle = new Node(); // the +infinity leaf
    left->key = min->clone();
    middle->key = max->clone();
    left->parent = middle->parent = root;
    root->key = middle->key;
    root->subLeft = left;
    root->subMiddle = middle;

    left->leftNode = NULL;
    left->rightNode = middle;
    middle->leftNode = left;
    middle->rightNode = NULL;
}

LCRS_BalancedTree::~LCRS_BalancedTree() {
    clear(root);
}

void LCRS_BalancedTree::Insert(const Key *nkey, const Value *nval) {
    Node *vertex = new Node();
    vertex->key = nkey->clone();
    vertex->value = nval->clone();
    Node *current = root;
    while (!current->isLeaf()) {
        if (*vertex->key < *current->subLeft->key)
            current = current->subLeft;
        else if (*vertex->key < *current->subMiddle->key)
            current = current->subMiddle;
        else current = current->subRight;
    }
    Node *parent = current->parent;
    if (*nkey < *parent->subLeft->key) {
        vertex->insertLeftTo(parent->subLeft);
    } else if (*nkey < *parent->subMiddle->key) {
        vertex->insertLeftTo(parent->subMiddle);
    } else if (parent->subRight == NULL || *nkey < *parent->subRight->key) {
        vertex->insertRightTo(parent->subMiddle);
    } else {
        vertex->insertRightTo(parent->subRight);
    }


    Node *insertResult = insertAndSplit(parent, vertex);
    while (root != parent) {
        parent = parent->parent;
        if (insertResult != NULL)
            insertResult = insertAndSplit(parent, insertResult);
        else
            parent->updateMaxKey();
    }
    if (insertResult != NULL) {
        Node *internal = new Node();
        internal->updateChildren(parent, insertResult, NULL);
        root = internal;
    }
    size++;
}

void LCRS_BalancedTree::Delete(const Key *dkey) {
    Node *vertex = find(root, dkey);
    if (vertex == NULL) {
        return;
    }
    size--;
    Node *parent = vertex->parent;
    if (vertex == parent->subLeft) {
        parent->updateChildren(parent->subMiddle, parent->subRight, NULL);
    } else if (vertex == parent->subMiddle) {
        parent->updateChildren(parent->subLeft, parent->subRight, NULL);
    } else {
        parent->updateChildren(parent->subLeft, parent->subMiddle, NULL);
    }
    delete vertex;
    while (parent != NULL) {
        if (parent->subMiddle == NULL) {
            if (parent != root) {
                parent = borrowOrMerge(parent);
            } else {
                root = parent->subLeft;
                root->parent = NULL;
                delete parent;
                return;
            }
        } else {
            parent->updateMaxKey();
            parent = parent->parent;
        }
    }
}

Value *LCRS_BalancedTree::Search(const Key *key) const {
    Node *vertex = find(root, key);
    if (vertex == NULL) return NULL;
    return vertex->value;
}

unsigned LCRS_BalancedTree::Rank(const Key *key) const {
    return 0;
}

const Key *LCRS_BalancedTree::Select(unsigned index) const {
    return NULL;
}

void LCRS_BalancedTree::Print_Values(const Key *key1, const Key *key2) const {

}

Node *LCRS_BalancedTree::insertAndSplit(Node *parent, Node *vertex) {
    if (parent == NULL || vertex == NULL) return NULL;
    Node *l = parent->subLeft, *m = parent->subMiddle, *r = parent->subRight;
    if (r == NULL) {
        if (*vertex->key < *l->key)
            parent->updateChildren(vertex, l, m);
        else if (*vertex->key < *m->key)
            parent->updateChildren(l, vertex, m);
        else
            parent->updateChildren(l, m, vertex);
        return NULL;
    }
    Node *newVertex = new Node();
    if (*vertex->key < *l->key) {
        parent->updateChildren(vertex, l, NULL);
        newVertex->updateChildren(m, r, NULL);
    } else if (*vertex->key < *m->key) {
        parent->updateChildren(l, vertex, NULL);
        newVertex->updateChildren(m, r, NULL);
    } else if (*vertex->key < *r->key) {
        parent->updateChildren(l, m, NULL);
        newVertex->updateChildren(vertex, r, NULL);
    } else {
        parent->updateChildren(l, m, NULL);
        newVertex->updateChildren(r, vertex, NULL);
    }
    return newVertex;
}

Node *LCRS_BalancedTree::find(Node *root, const Key *key) const {
    if (root->isLeaf()) {
        return key_equals(root->key, key) ? root : NULL;
    }
    if (*key < *root->subLeft->key || key_equals(key, root->subLeft->key)) {
        return find(root->subLeft, key);
    }
    if ((*key < *root->subMiddle->key || key_equals(key, root->subMiddle->key))) {
        return find(root->subMiddle, key);
    }
    return find(root->subRight, key);
}

bool LCRS_BalancedTree::key_equals(const Key *key1, const Key *key2) const {
    return !(*key1 < *key2) && !(*key2 < *key1);
}

Node *LCRS_BalancedTree::borrowOrMerge(Node *vertex) {
    Node *parent = vertex->parent;
    Node *toBorrow = NULL;
    if (vertex == parent->subLeft) {
        toBorrow = parent->subMiddle;
        if (toBorrow->subRight != NULL) {
            vertex->updateChildren(vertex->subLeft, toBorrow->subLeft, NULL);
            toBorrow->updateChildren(toBorrow->subMiddle, toBorrow->subRight, NULL);
        } else {
            toBorrow->updateChildren(vertex->subLeft, toBorrow->subLeft, toBorrow->subMiddle);
            delete vertex;
            parent->updateChildren(toBorrow, parent->subRight, NULL);
        }
        return parent;
    }
    if (vertex == parent->subMiddle) {
        toBorrow = parent->subLeft;
        if (toBorrow->subRight != NULL) {
            toBorrow->updateChildren(toBorrow->subLeft, toBorrow->subMiddle, NULL);
            vertex->updateChildren(toBorrow->subRight, vertex->subLeft, NULL);
        } else {
            toBorrow->updateChildren(toBorrow->subLeft, toBorrow->subMiddle, vertex->subLeft);
            delete vertex;
            parent->updateChildren(toBorrow, parent->subRight, NULL);
        }
        return parent;
    }
    toBorrow = parent->subMiddle;
    if (toBorrow->subRight != NULL) {
        toBorrow->updateChildren(toBorrow->subLeft, toBorrow->subMiddle, NULL);
        vertex->updateChildren(toBorrow->subRight, vertex->subLeft, NULL);
    } else {
        toBorrow->updateChildren(toBorrow->subLeft, toBorrow->subMiddle, vertex->subLeft);
        delete vertex;
        parent->updateChildren(parent->subLeft, toBorrow, NULL);

    }
    return parent;
}

void LCRS_BalancedTree::clear(Node *root) {
    if (root == NULL) return;
    if (root->isLeaf()) {
        delete root; // should i delete key? should i delete value?
        return;
    }
    clear(root->subLeft);
    root->subLeft = NULL;
    clear(root->subMiddle);
    root->subMiddle = NULL;
    clear(root->subRight);
    root->subRight = NULL;
    clear(root);
}


