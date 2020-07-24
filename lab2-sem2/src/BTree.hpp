#pragma once

#include <memory>
#include <vector>
#include <type_traits>

template<typename DataType>
class BTreePlain;

template<typename DataType>
class BTreePlus;

/**
 * @brief A base class provides a general interface for b_tree nodes and its variations
 * @tparam DataType a type of data stored in a node
 */
template<typename DataType>
class BTreeNode {
public:
    /**
     * @return number of keys stored in node
     */
    [[nodiscard]] std::size_t size() const;

    /**
     * @return number of node's children
     */
    [[nodiscard]] std::size_t childrenNumber() const;

    /**
     * @param index
     * @return value of key stored at position index
     */
    [[nodiscard]] DataType getKeyValue(std::size_t index) const;

    /**
     * @param index
     * @return shared_ptr to child node at position index
     */
    [[nodiscard]] std::shared_ptr<BTreeNode> getChild(std::size_t index) const;

    /**
     * @return true if node is leaf, else - false
     */
    [[nodiscard]] bool isLeaf() const;

    /**
     * @return shared_ptr to parent node
     */
    [[nodiscard]] std::shared_ptr<BTreeNode> parent() const;

protected:
    std::vector<DataType> _keys;
    std::vector<std::shared_ptr<BTreeNode>> _children;
    bool _is_leaf;
    std::weak_ptr<BTreeNode> _parent;

};

/**
 * @brief A class for storing a node of plain b_tree
 * @tparam DataType a type of data stored in a node
 */
template<typename DataType>
class BTreePlainNode : public BTreeNode<DataType> {
    friend BTreePlain<DataType>;
};

/**
 * @brief A class for storing a node of plain b_tree
 * @note stores keys of std::size_t type which correspond to indexes of values stored in member _data of BTreePlus object
 */
class BTreePlusNode : public BTreeNode<std::size_t> {
public:
    /**
     * @return shared_ptr to next leaf node
     */
    [[nodiscard]] std::shared_ptr<BTreePlusNode> nextLeaf() const;

    /**
     * @return shared_ptr to previous leaf node
     */
    [[nodiscard]] std::shared_ptr<BTreePlusNode> prevLeaf() const;
private:
    std::weak_ptr<BTreePlusNode> _next_leaf;
    std::weak_ptr<BTreePlusNode> _prev_leaf;

    template<typename DataType>
    friend
    class BTreePlus;
};

/**
 * @brief A base class provides a general interface for b_tree data structure and its variations
 * @tparam DataType a type of data stored in a node
 * @tparam NodeType a type of nodes used in a tree
 * @note NodeType must be BTreeNode or derived from BTreeNode
 */
template<
        typename DataType,
        typename NodeType,
        typename = typename std::enable_if_t<std::is_base_of<BTreeNode<DataType>, NodeType>::value>
>
class BTree {
public:
    /**
     * @return min degree parameter of tree
     */
    [[nodiscard]] std::size_t minDegree();

    /**
     * @return shared_ptr to root node of tree
     */
    [[nodiscard]] std::shared_ptr<NodeType> root() const;

protected:
    std::size_t _min_degree;
    std::size_t _min_node_fill;
    std::size_t _max_node_fill;
    std::shared_ptr<NodeType> _root;
};

/**
 * @brief A class for storing a plain b_tree
 * @tparam DataType a type of data stored in a node
 * @note type of nodes is set BTreePlainNode<DataType>
 */
template<typename DataType>
class BTreePlain : public BTree<DataType, BTreePlainNode<DataType>> {
    using Node = BTreePlainNode<DataType>;
};

/**
 * @brief A class for storing a b+_tree
 * @tparam DataType a type of data stored in a node
 * @note type of nodes is set BTreePlusNode
 */
template<typename DataType>
class BTreePlus : public BTree<DataType, BTreePlusNode> {
    using Node = BTreePlusNode;
public:
    /**
     * @param index
     * @return value stored at position index in data container
     */
    DataType getValue(std::size_t index) const;

private:
    /**
     * @brief stores keys of tree to avoid duplication of objects in non-leaf nodes
     */
    std::vector<DataType> _data;
};

#include "BTree.hxx"
