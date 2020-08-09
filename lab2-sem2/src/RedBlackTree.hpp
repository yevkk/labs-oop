#pragma once

#include <memory>

/**
 * included for quick debug output
 * TODO: remove
 */
#include <iostream>

/**
 * a class for storing red-black tree data structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class RedBlackTree {
public:
    using value_type = T;

private:
    /**
     * @brief tree node structure
     */
    struct Node {
        enum class Color {
            RED, BLACK
        };

        explicit Node(const value_type &key);

        void switchColor();

        Color color;
        value_type key;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;
    };

public:
    RedBlackTree();

    /**
     * @brief checks if key was added to tree
     * @param key a key to check
     * @return true if key is in tree, else - false
     */
    [[nodiscard]] bool includes(const value_type &key);

    /**
     * @brief inserts key to tree
     * @param key a key to be inserted
     */
    void insert(const value_type &key);

    /**
     * @brief removes key from tree
     * @param key a key to be removed
     */
    void remove(const value_type &key);

    //temporary solution for quick testing
    //TODO:: remove;
    void print() {
        _print(std::cout, _root);
    };

private:
    //temporary solution for quick testing
    //TODO:: remove;
    template<typename OStream>
    void _print(OStream &output, std::shared_ptr<Node> node, int level = 0) {

        output << '|';
        for (std::size_t i = 0; i < level; i++) {
            output << "\t|";
        }

        if (node == _null_node) {
            output << "*\n";
            return;
        } else {
            output << node->key << " (";

            if (node->color == Node::Color::BLACK) {
                output << "B";
            } else {
                output << "R";
            }

            output << ")\n";
        }
        _print(output, node->left, level + 1);
        _print(output, node->right, level + 1);
    }

    /**
     * @brief restores rb-tree features after key insertion
     * @param node a node of tree
     */
    void _insertFixup(std::shared_ptr<Node> node);

    /**
     * @brief restores rb-tree features after key removal
     * @param node a node of tree
     */
    void _deleteFixup(std::shared_ptr<Node> node);

    /**
     * @brief replaces dst node with src
     * @param dst destination node (with all its branches)
     * @param src source node
     */
    void _transplant(std::shared_ptr<Node> dst, std::shared_ptr<Node> src);

    /**
     * @brief finds a node with min value in subtree
     * @param subtree_root root of subtree being processed
     * @return a node with min value in subtree with given root
     */
    auto _subtreeMinNode(std::shared_ptr<Node> subtree_root) -> std::shared_ptr<Node>;

    /**
     * @brief finds a node with max value in subtree
     * @param subtree_root root of subtree being processed
     * @return a node with max value in subtree with given root
     */
    auto _subtreeMaxNode(std::shared_ptr<Node> subtree_root) -> std::shared_ptr<Node>;

    /**
     * @brief finds a node with successor value
     * @param node a node of tree
     * @return a node with value, that is a successor of value in given node
     */
    auto _nodeSuccessor(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;

    /**
     * @brief finds a node with predecessor value
     * @param node a node of tree
     * @return a node with value? that is a predecessor of value in given node
     */
    auto _nodePredecessor(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;

    /**
     * @brief an implementation of search operation
     * @param node a candidate node
     * @param key a key to be searched
     * @return a node where search is stopped (equal to _null_node if key was not found)
     */
    auto _searchImpl(std::shared_ptr<Node> node, const value_type &key) -> std::shared_ptr<Node>;

    /**
     * @brief an implementation of node removal
     * @param node a node to remove
     */
    void _removeImpl(std::shared_ptr<Node> node);

    /**
     * @brief left rotate method for tree balancing
     * @param node_x a node of tree
     */
    void _leftRotate(std::shared_ptr<Node> node_x);

    /**
     * @brief right rotate method for tree balancing
     * @param node_x a node of tree
     */
    void _rightRotate(std::shared_ptr<Node> node_x);

    std::shared_ptr<Node> _root;
    std::shared_ptr<Node> _null_node;
};


#include "RedBlackTree.hxx"