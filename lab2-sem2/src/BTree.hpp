#pragma once

#include <memory>
#include <vector>

/**
 * included for quick debug output
 * TODO: remove
 */
#include <iostream>

constexpr std::size_t MIN_DEGREE = 2;

/**
 * @brief a class for storing b-tree data structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class BTree {
public:
    using value_type = T;

private:
    /**
     * @brief tree node structure
     */
    struct Node {
        Node();

        /**
         * @return number of keys stored in node
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @return number of node's children
         */
        [[nodiscard]] std::size_t childrenNumber() const;

        std::vector<value_type> keys;
        std::vector<std::shared_ptr<Node>> children;
        bool is_leaf;
        std::weak_ptr<Node> parent;
    };

public:
    explicit BTree(std::size_t min_degree = MIN_DEGREE);

    /**
     * @return min degree parameter of tree
     */
    [[nodiscard]] std::size_t minDegree();

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
    }

private:
    //temporary solution for quick testing
    //TODO:: remove;
    template<typename OStream>
    void _print(OStream &output, std::shared_ptr<Node> node, int level = 0) {
        output << '|';
        for (std::size_t i = 0; i < level; i++) {
            output << '\t' << '|';
        }

        if (!node) {
            output << "*\n";
        } else {
            output << "Data: (" << node->size() << ")\n";

            for (auto &item : node->keys) {
                output << '|';
                for (std::size_t i = 0; i < level; i++) {
                    output << '\t' << '|';
                }
                output << item << "\n";
            }

            if (!node->is_leaf) {
                output << '|';
                for (std::size_t i = 0; i < level; i++) {
                    output << '\t' << '|';
                }
                output << "Children:\n";
                for (auto &item : node->children) {
                    _print(output, item, level + 1);
                }
            }
        }

        output << '|';
        for (std::size_t i = 0; i < level; i++) {
            output << '\t' << '|';
        }
        output << "=============\n";
    }

    /**
     * @param subtree_root root of subtree being processed
     * @return min value stored in subtree
     */
    value_type _subtreeMinValue(std::shared_ptr<Node> subtree_root);

    /**
     * @param subtree_root root of subtree being processed
     * @return max value stored in subtree
     */
    value_type _subtreeMaxValue(std::shared_ptr<Node> subtree_root);

    /**
     * @brief splits node's child to separate nodes to avoid overflow
     * @param node the node with full child
     * @param child the full node
     * @param child_index index of full child
     */
    void _splitChild(std::shared_ptr<Node> node,
                     std::shared_ptr<Node> child,
                     std::size_t child_index);

    /**
     * @brief merges two node's children separated by single key
     * @param node a parent node of mergeable children
     * @param separator_index index of key - separator of mergeable children
     */
    void _mergeChildren(std::shared_ptr<Node> node,
                        std::size_t separator_index);

    /**
     * @brief inserts new key to the tree with non full root
     * @param node a candidate node for key insertion
     * @param key new key to be inserted
     */
    void _insertNonFull(std::shared_ptr<Node> node,
                        const value_type &key);

    /**
     * @brief implementation of new key insertion
     * @param key new key to be inserted
     */
    void _insertImpl(const value_type &key);

    /**
     * @brief implementation of key removal
     * @param node a candidate node for key removal
     * @param key key to be removed
     */
    void _removeImpl(std::shared_ptr<Node> node,
                     const value_type &key);

    /**
     * @brief implementation of key search
     * @param node a candidate node for key search
     * @param key searched key
     * @return true if key is in node, else - false
     */
    bool _searchImpl(std::shared_ptr<Node> node,
                     const value_type &key);

    std::size_t _min_degree;
    std::size_t _min_node_fill;
    std::size_t _max_node_fill;
    std::shared_ptr<Node> _root;
};

#include "BTree.hxx"
