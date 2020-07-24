#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>

template<typename DataType>
class BTreePlain;

template<typename DataType>
class BTreePlus;

template<typename DataType>
class BTreeNode {
public:

protected:
    std::vector<DataType> _keys;
    std::vector<std::shared_ptr<BTreeNode>> _children;
    bool _is_leaf;
    std::weak_ptr<BTreeNode> _parent;

};

template<typename DataType>
class BTreePlainNode : public BTreeNode<DataType> {
    friend BTreePlain<DataType>;
};

class BTreePlusNode : public BTreeNode<std::size_t> {
private:
    std::weak_ptr<BTreePlusNode> _next_leaf;
    std::weak_ptr<BTreePlusNode> _prev_leaf;

    template<typename DataType>
    friend
    class BTreePlus;
};

template<
        typename DataType,
        typename NodeType,
        typename = typename std::enable_if_t<std::is_base_of<BTreeNode<DataType>, NodeType>::value>
>
class BTree {
protected:
    std::size_t _min_degree;
    std::shared_ptr<NodeType> _root;
};

template<typename DataType>
class BTreePlain : public BTree<DataType, BTreePlainNode<DataType>> {
    using Node = BTreePlainNode<DataType>;
};

template<typename DataType>
class BTreePlus : public BTree<DataType, BTreePlusNode> {
    using Node = BTreePlusNode;
};

#include "BTree.hxx"
