#include "avl_tree.h"

#include <algorithm>

int AvlTree::balanceFactor(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

void AvlTree::updateHeight(Node* n) {
    n->height = 1 + std::max(height(n->left), height(n->right));
}

AvlTree::Node* AvlTree::rotateRight(Node* n) {
    Node* l  = n->left;
    n->left  = l->right;
    l->right = n;
    updateHeight(n);
    updateHeight(l);
    return l;
}

AvlTree::Node* AvlTree::rotateLeft(Node* n) {
    Node* r  = n->right;
    n->right = r->left;
    r->left  = n;
    updateHeight(n);
    updateHeight(r);
    return r;
}

AvlTree::Node* AvlTree::balance(Node* n) {
    updateHeight(n);
    int bf = balanceFactor(n);

    if (bf > 1) {
        if (balanceFactor(n->left) < 0)
            n->left = rotateLeft(n->left);
        return rotateRight(n);
    }
    if (bf < -1) {
        if (balanceFactor(n->right) > 0)
            n->right = rotateRight(n->right);
        return rotateLeft(n);
    }
    return n;
}

AvlTree::Node* AvlTree::minNode(Node* n) {
    while (n->left)
        n = n->left;
    return n;
}

bool AvlTree::insert(const Flight& f) {
    bool inserted = false;
    root_         = insert(root_, f, inserted);
    if (inserted)
        ++count_;
    return inserted;
}

AvlTree::Node* AvlTree::insert(Node* n, const Flight& f, bool& inserted) {
    if (!n) {
        inserted = true;
        return new Node{f};
    }
    if (f.flightNumber < n->data.flightNumber)
        n->left = insert(n->left, f, inserted);
    else if (f.flightNumber > n->data.flightNumber)
        n->right = insert(n->right, f, inserted);
    else
        return n;

    return balance(n);
}

bool AvlTree::remove(const std::string& flightNumber) {
    bool removed = false;
    root_        = remove(root_, flightNumber, removed);
    if (removed)
        --count_;
    return removed;
}

AvlTree::Node* AvlTree::remove(Node* n, const std::string& key, bool& removed) {
    if (!n)
        return nullptr;

    if (key < n->data.flightNumber) {
        n->left = remove(n->left, key, removed);
    } else if (key > n->data.flightNumber) {
        n->right = remove(n->right, key, removed);
    } else {
        removed = true;
        if (!n->left || !n->right) {
            Node* child = n->left ? n->left : n->right;
            delete n;
            return child;
        }
        Node* succ = minNode(n->right);
        n->data    = succ->data;
        bool dummy = false;
        n->right   = remove(n->right, succ->data.flightNumber, dummy);
    }
    return balance(n);
}

const Flight* AvlTree::find(const std::string& flightNumber) const {
    Node* n = root_;
    while (n) {
        if (flightNumber < n->data.flightNumber)
            n = n->left;
        else if (flightNumber > n->data.flightNumber)
            n = n->right;
        else
            return &n->data;
    }
    return nullptr;
}

Flight* AvlTree::findMutable(const std::string& flightNumber) {
    Node* n = root_;
    while (n) {
        if (flightNumber < n->data.flightNumber)
            n = n->left;
        else if (flightNumber > n->data.flightNumber)
            n = n->right;
        else
            return &n->data;
    }
    return nullptr;
}

void AvlTree::destroy(Node* n) {
    if (!n)
        return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

void AvlTree::clear() {
    destroy(root_);
    root_  = nullptr;
    count_ = 0;
}

void AvlTree::inorder(Node* n, std::vector<Flight>& out) {
    if (!n)
        return;
    inorder(n->left, out);
    out.push_back(n->data);
    inorder(n->right, out);
}

std::vector<Flight> AvlTree::inorder() const {
    std::vector<Flight> result;
    result.reserve(count_);
    inorder(root_, result);
    return result;
}

void AvlTree::postorder(Node* n, std::vector<Flight>& out) {
    if (!n)
        return;
    postorder(n->left, out);
    postorder(n->right, out);
    out.push_back(n->data);
}

std::vector<Flight> AvlTree::postorder() const {
    std::vector<Flight> result;
    result.reserve(count_);
    postorder(root_, result);
    return result;
}
