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

