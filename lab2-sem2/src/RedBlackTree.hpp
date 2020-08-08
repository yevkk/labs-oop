#pragma once

#include <memory>

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

private:

    void _insertFixup(std::shared_ptr<Node> node);

    void _deleteFixup(std::shared_ptr<Node> node);

    void _transplant(std::shared_ptr<Node> dst, std::shared_ptr<Node> src);

    std::shared_ptr<Node> _subtreeMinNode(std::shared_ptr<Node> subtree_root);

    std::shared_ptr<Node> _subtreeMaxNode(std::shared_ptr<Node> subtree_root);

    std::shared_ptr<Node> _nodeSuccessor(std::shared_ptr<Node> node);

    std::shared_ptr<Node> _nodePredecessor(std::shared_ptr<Node> node);

    std::shared_ptr<Node> _searchImpl(std::shared_ptr<Node> node, const value_type &key);

    void _leftRotate(std::shared_ptr<Node> node);

    void _rightRotate(std::shared_ptr<Node> node);
};

#include "RedBlackTree.hxx"