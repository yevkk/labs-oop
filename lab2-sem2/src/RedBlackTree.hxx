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

