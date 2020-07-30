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

BTreePlusNode::BTreePlusNode() :
        BTreeNode<std::size_t>{},
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

std::shared_ptr<BTreePlusNode> BTreePlusNode::nextLeaf() const {
    assert(_is_leaf && "called for non-leaf node");
    return _next_leaf.lock();
}

std::shared_ptr<BTreePlusNode> BTreePlusNode::prevLeaf() const {
    assert(_is_leaf && "called for non-leaf node");
    return _prev_leaf.lock();
}

template<typename DataType>
auto BTreeNodeFactory::NewPlain() {
    return std::make_shared<BTreePlainNode<DataType>>();
}

auto BTreeNodeFactory::NewPlus() {
    return std::make_shared<BTreePlusNode>();
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
DataType BTreePlus<DataType>::getValue(std::size_t index) const {
    assert(index < _data.size() && "wrong parameter value");
    return _data[index];
}
