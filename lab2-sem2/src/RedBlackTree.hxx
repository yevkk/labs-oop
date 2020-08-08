#pragma once

#include "RedBlackTree.hpp"

template<typename T>
RedBlackTree<T>::Node::Node(const value_type &key) :
        key{key},
        color{Color::RED},
        left{nullptr},
        right{nullptr},
        parent{} {}

template<typename T>
void RedBlackTree<T>::Node::switchColor() {
    if (color == Color::BLACK) {
        color = Color::RED;
    } else {
        color = Color::BLACK;
    };
}

template<typename T>
RedBlackTree<T>::RedBlackTree() :
        _root{nullptr},
        _null_node{std::make_shared<Node>(value_type{})} {
    _null_node->color = Node::Color::BLACK;
}

template<typename T>
void RedBlackTree<T>::_insertFixup(std::shared_ptr<Node> node) {

}

template<typename T>
void RedBlackTree<T>::_deleteFixup(std::shared_ptr<Node> node) {

}

template<typename T>
void RedBlackTree<T>::_transplant(std::shared_ptr<Node> dst,
                                  std::shared_ptr<Node> src) {
    if (dst->parent.lock() == _null_node) {
        _root = src;
    } else if (dst == dst->parent.lock()->left) {
        dst->parent.lock()->left = src;
    } else {
        dst->parent.lock()->right = src;
    }

    if (src != _null_node) {
        src->parent = dst->parent;
    }
}

template<typename T>
auto RedBlackTree<T>::_subtreeMinNode(std::shared_ptr<Node> subtree_root) -> std::shared_ptr<Node> {
    auto ptr = _root;

    while (ptr->left != _null_node) {
        ptr = ptr->left;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_subtreeMaxNode(std::shared_ptr<Node> subtree_root) -> std::shared_ptr<Node> {
    auto ptr = _root;

    while (ptr->right != _null_node) {
        ptr = ptr->right;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_nodeSuccessor(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {
    if (node->right != _null_node) {
        return _subtreeMinNode(node->right);
    }

    auto ptr = node->parent.lock();
    while((ptr != _null_node) && (node == ptr->right)) {
        node = ptr;
        ptr = ptr->parent.lock();
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_nodePredecessor(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {
    if (node->left != _null_node) {
        return _subtreeMaxNode(node->left);
    }

    auto ptr = node->parent.lock();
    while((ptr != _null_node) && (node == ptr->left)) {
        node = ptr;
        ptr = ptr->parent.lock();
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_searchImpl(std::shared_ptr<Node> node, const value_type &key) -> std::shared_ptr<Node> {
    if ((node == _null_node) || (key = node->key)) {
        return node;
    }

    if (key < node->key) {
        return _searchImpl(node->left, key);
    } else {
        return _searchImpl(node->right, key);
    }
}

template<typename T>
void RedBlackTree<T>::_leftRotate(std::shared_ptr<Node> node_x) {
    auto node_y = node_x->right;
    node_x->right = node_y->left;

    if (node_y->left != _null_node) {
        node_y->left->parent = node_x;
    }

    node_y->parent = node_x->parent;
    if (node_x->parent.lock() == _null_node) {
        _root = node_y;
    } else if (node_x == node_x->parent.lock()->left) {
        node_x->parent.lock()->left = node_y;
    } else {
        node_x->parent.lock()->right = node_y;
    }

    node_y->left = node_x;
    node_x->parent = node_y;
}

template<typename T>
void RedBlackTree<T>::_rightRotate(std::shared_ptr<Node> node_x) {
    auto node_y = node_x->left;
    node_x->left = node_y->right;

    if (node_y->right != _null_node) {
        node_y->right->parent = node_x;
    }

    node_y->parent = node_x->parent;
    if (node_x->parent.lock() == _null_node) {
        _root = node_y;
    } else if (node_x == node_x->parent.lock()->left) {
        node_x->parent.lock()->left = node_y;
    } else {
        node_x->parent.lock()->right = node_y;
    }

    node_y->right = node_x;
    node_x->parent = node_y;
}
