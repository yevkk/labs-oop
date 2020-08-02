#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <cassert>

template<typename DataType>
BTreeNode<DataType>::BTreeNode() :
        _is_leaf{false},
        _parent{} {}

template<typename DataType>
BTreePlainNode<DataType>::BTreePlainNode() :
        BTreeNode<DataType>{} {}

template<typename DataType>
BTreePlusNode<DataType>::BTreePlusNode() :
        BTreeNode<DataType>{},
        _next_leaf{},
        _prev_leaf{} {}

template<typename DataType>
std::size_t BTreeNode<DataType>::size() const {
    return _keys.size();
}

template<typename DataType>
std::size_t BTreeNode<DataType>::childrenNumber() const {
    return _children.size();
}

template<typename DataType>
DataType BTreeNode<DataType>::getKeyValue(std::size_t index) const {
    assert(index < size() && "wrong parameter value");
    return _keys[index];
}

template<typename DataType>
std::shared_ptr<BTreeNode<DataType>> BTreeNode<DataType>::getChild(std::size_t index) const {
    assert(index < childrenNumber() && "wrong parameter value");
    return _children[index];
}

template<typename DataType>
bool BTreeNode<DataType>::isLeaf() const {
    return _is_leaf;
}

template<typename DataType>
std::shared_ptr<BTreeNode<DataType>> BTreeNode<DataType>::parent() const {
    return _parent.lock();
}

template<typename DataType>
std::shared_ptr<BTreePlusNode<DataType>> BTreePlusNode<DataType>::nextLeaf() const {
    assert(this->_is_leaf && "called for non-leaf node");
    return _next_leaf.lock();
}

template<typename DataType>
std::shared_ptr<BTreePlusNode<DataType>> BTreePlusNode<DataType>::prevLeaf() const {
    assert(this->_is_leaf && "called for non-leaf node");
    return _prev_leaf.lock();
}

template<typename DataType>
auto BTreeNodeFactory::NewPlain() {
    return std::make_shared<BTreePlainNode<DataType>>();
}

template<typename DataType>
auto BTreeNodeFactory::NewPlus() {
    return std::make_shared<BTreePlusNode<DataType>>();
}

template<typename DataType, typename NodeType, typename Condition>
BTree<DataType, NodeType, Condition>::BTree(std::size_t min_degree) :
        _min_degree{min_degree < MIN_DEGREE ? MIN_DEGREE : min_degree},
        _min_node_fill{_min_degree - 1},
        _max_node_fill{_min_node_fill * 2 - 1},
        _root{std::make_shared<NodeType>()} {
    _root->leaf = true;
}

template<typename DataType, typename NodeType, typename Condition>
std::size_t BTree<DataType, NodeType, Condition>::minDegree() {
    return _min_degree;
}

template<typename DataType, typename NodeType, typename Condition>
std::shared_ptr<NodeType> BTree<DataType, NodeType, Condition>::root() const {
    return _root;
}

template<typename DataType>
BTreePlain<DataType>::BTreePlain(std::size_t min_degree) :
        BTree<DataType, NodeType>{min_degree} {}

template<typename  DataType>
BTreePlus<DataType>::BTreePlus(std::size_t min_degree) :
        BTree<DataType, NodeType>{min_degree} {}

