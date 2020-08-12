#pragma once

#include "RedBlackTree.hpp"

template<typename T>
RedBlackTree<T>::Node::Node(const value_type &key) :
        key{key},
        color{Color::RED},
        left{nullptr},
        right{nullptr},
        parent{nullptr} {}

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
        _null_node{new Node(value_type())} {
    _null_node->parent = _null_node->left = _null_node->right = _null_node;
    _null_node->color = Node::Color::BLACK;
    _root = _null_node;
}

template<typename T>
void RedBlackTree<T>::insert(const value_type &key) {
    auto node_y = _null_node;
    auto node_x = _root;

    while (node_x != _null_node) {
        node_y = node_x;

        if (key < node_x->key) {
            node_x = node_x->left;
        } else {
            node_x = node_x->right;
        }
    }

    auto new_node = new Node(key);
    new_node->parent = node_y;
    new_node->left = new_node->right = _null_node;
    if (node_y == _null_node) {
        _root = new_node;
    } else if (key < node_y->key) {
        node_y->left = new_node;
    } else {
        node_y->right = new_node;
    }

    _insertFixup(new_node);
}

template<typename T>
void RedBlackTree<T>::remove(const value_type &key) {
    auto node = _searchImpl(_root, key);
    if (node != _null_node) {
        _removeImpl(node);
    }
}

template<typename T>
void RedBlackTree<T>::_removeImpl(Node *node) {
    auto *y = node;
    decltype(node) x;
    auto y_original_color = y->color;

    if (node->left == _null_node) {
        x = node->right;
        _transplant(node, node->right);
    } else if (node->right == _null_node) {
        x = node->left;
        _transplant(node, node->left);
    } else {
        y = _subtreeMinNode(node->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == node) {
            x->parent = y;
        } else {
            _transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        _transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }
    delete node;

    if (y_original_color == Node::Color::BLACK) {
        _deleteFixup(x);
    }
}

template<typename T>
void RedBlackTree<T>::_insertFixup(Node *node) {
    while (node->parent->color == Node::Color::RED) {
        if (node->parent == node->parent->parent->left) {
            auto node_y = node->parent->parent->right;
            if (node_y->color == Node::Color::RED) {
                node->parent->color = Node::Color::BLACK;
                node_y->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    _leftRotate(node);
                }
                node->parent->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                _rightRotate(node->parent->parent);

            }

        } else {
            auto node_y = node->parent->parent->left;
            if (node_y->color == Node::Color::RED) {
                node->parent->color = Node::Color::BLACK;
                node_y->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    _rightRotate(node);
                }
                node->parent->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                _leftRotate(node->parent->parent);

            }
        }
    }

    _root->color = Node::Color::BLACK;
}

template<typename T>
void RedBlackTree<T>::_deleteFixup(Node *node) {
    while (node != _root && node->color == Node::Color::BLACK) {
        if (node == node->parent->left) {
            auto node_w = node->parent->right;
            if (node_w->color == Node::Color::RED) {
                node_w->color = Node::Color::BLACK;
                node->parent->color = Node::Color::RED;
                _leftRotate(node->parent);
                node_w = node->parent->right;
            }
            if (node_w->left->color == Node::Color::BLACK && node_w->right->color == Node::Color::BLACK) {
                node_w->color = Node::Color::RED;
                node = node->parent;
            } else {
                if (node_w->right->color == Node::Color::BLACK) {
                    node_w->left->color = Node::Color::BLACK;
                    node_w->color = Node::Color::RED;
                    _rightRotate(node_w);
                    node_w = node->parent->right;
                }
                node_w->color = node->parent->color;
                node->parent->color = Node::Color::BLACK;
                node_w->right->color = Node::Color::BLACK;
                _leftRotate(node->parent);
                node = _root;
            }
        } else {
            auto node_w = node->parent->left;
            if (node_w->color == Node::Color::RED) {
                node_w->color = Node::Color::BLACK;
                node->parent->color = Node::Color::RED;
                _rightRotate(node->parent);
                node_w = node->parent->left;
            }
            if (node_w->right->color == Node::Color::BLACK && node_w->left->color == Node::Color::BLACK) {
                node_w->color = Node::Color::RED;
                node = node->parent;
            } else {
                if (node_w->left->color == Node::Color::BLACK) {
                    node_w->right->color = Node::Color::BLACK;
                    node_w->color = Node::Color::RED;
                    _leftRotate(node_w);
                    node_w = node->parent->left;
                }
                node_w->color = node->parent->color;
                node->parent->color = Node::Color::BLACK;
                node_w->left->color = Node::Color::BLACK;
                _rightRotate(node->parent);
                node = _root;
            }
        }
    }
    node->color = Node::Color::BLACK;
}

template<typename T>
void RedBlackTree<T>::_transplant(Node *dst, Node *src) {
    if (dst->parent == _null_node) {
        _root = src;
    } else if (dst == dst->parent->left) {
        dst->parent->left = src;
    } else {
        dst->parent->right = src;
    }

    if (src != _null_node) {
        src->parent = dst->parent;
    }
}

template<typename T>
auto RedBlackTree<T>::_subtreeMinNode(Node *subtree_root) -> Node * {
    auto ptr = subtree_root;

    while (ptr->left != _null_node) {
        ptr = ptr->left;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_subtreeMaxNode(Node *subtree_root) -> Node * {
    auto ptr = subtree_root;

    while (ptr->right != _null_node) {
        ptr = ptr->right;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_nodeSuccessor(Node *node) -> Node * {
    if (node->right != _null_node) {
        return _subtreeMinNode(node->right);
    }

    auto ptr = node->parent;
    while ((ptr != _null_node) && (node == ptr->right)) {
        node = ptr;
        ptr = ptr->parent;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_nodePredecessor(Node *node) -> Node * {
    if (node->left != _null_node) {
        return _subtreeMaxNode(node->left);
    }

    auto ptr = node->parent;
    while ((ptr != _null_node) && (node == ptr->left)) {
        node = ptr;
        ptr = ptr->parent;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_searchImpl(Node *node, const value_type &key) -> Node * {
    if ((node == _null_node) || (key == node->key)) {
        return node;
    }

    if (key < node->key) {
        return _searchImpl(node->left, key);
    } else {
        return _searchImpl(node->right, key);
    }
}

template<typename T>
void RedBlackTree<T>::_leftRotate(Node *node_x) {
    auto node_y = node_x->right;
    node_x->right = node_y->left;

    if (node_y->left != _null_node) {
        node_y->left->parent = node_x;
    }

    node_y->parent = node_x->parent;
    if (node_x->parent == _null_node) {
        _root = node_y;
    } else if (node_x == node_x->parent->left) {
        node_x->parent->left = node_y;
    } else {
        node_x->parent->right = node_y;
    }

    node_y->left = node_x;
    node_x->parent = node_y;
}

template<typename T>
void RedBlackTree<T>::_rightRotate(Node *node_x) {
    auto node_y = node_x->left;
    node_x->left = node_y->right;

    if (node_y->right != _null_node) {
        node_y->right->parent = node_x;
    }

    node_y->parent = node_x->parent;
    if (node_x->parent == _null_node) {
        _root = node_y;
    } else if (node_x == node_x->parent->left) {
        node_x->parent->left = node_y;
    } else {
        node_x->parent->right = node_y;
    }

    node_y->right = node_x;
    node_x->parent = node_y;
}
