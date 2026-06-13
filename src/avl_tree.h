#pragma once

#include <string>
#include <vector>

#include "model.h"

class AvlTree {
public:
    AvlTree() = default;
    ~AvlTree() {
        clear();
    }

    AvlTree(const AvlTree&)            = delete;
    AvlTree& operator=(const AvlTree&) = delete;

    bool insert(const Flight& f);
    bool remove(const std::string& flightNumber);
    const Flight* find(const std::string& flightNumber) const;
    Flight* findMutable(const std::string& flightNumber);
    void clear();

    bool empty() const {
        return root_ == nullptr;
    }
    int size() const {
        return count_;
    }

    std::vector<Flight> inorder() const;
    std::vector<Flight> postorder() const;

private:
    struct Node {
        Flight data;
        Node* left  = nullptr;
        Node* right = nullptr;
        int height  = 1;
    };

    static int height(Node* n) {
        return n ? n->height : 0;
    }
    static int balanceFactor(Node* n);
    static void updateHeight(Node* n);
    static Node* rotateRight(Node* n);
    static Node* rotateLeft(Node* n);
    static Node* balance(Node* n);
    static Node* minNode(Node* n);

    Node* insert(Node* n, const Flight& f, bool& inserted);
    Node* remove(Node* n, const std::string& key, bool& removed);
    void destroy(Node* n);

    static void inorder(Node* n, std::vector<Flight>& out);
    static void postorder(Node* n, std::vector<Flight>& out);

    Node* root_ = nullptr;
    int count_  = 0;
};
