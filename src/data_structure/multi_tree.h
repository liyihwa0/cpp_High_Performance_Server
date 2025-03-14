#pragma once
#include "../global_def.h"
#include "../exception/exception.h"
#include "../smart_ptr.h"
#include <sstream>
#include <iostream>

namespace wa{
    // 多叉树类
    // 目前被用于存放日志
    /*
     * 1. 允许添加MultiTree作为子节点
     * 2. 允许merge两个MultiTree
     * 3. 有hasPath(变长参数)方法来查询路径是否存在
     * 4. 叶子节点只存放数据
     * 5. 非叶子节点只存放其他节点的索引
     */
    template<typename K,typename V>
    class MultiTree{
        class TreeNode{
            Bool isLeaf_;
        public:
            explicit TreeNode(Bool isLeaf):isLeaf_(isLeaf){}
            Bool isLeaf() const {
                return isLeaf_;
            }
        };
        class LeafNode:TreeNode{
            V value_;
        public:
            explicit LeafNode(V value): TreeNode(TRUE),value_(std::move(value)){}
            V value(){
                return value_;
            }

        };
        class NonLeafNode:TreeNode{
            Vector<SP<TreeNode>> children_;

        };



    };
}