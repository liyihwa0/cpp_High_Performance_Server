#pragma once

#include "../global_def.h"
#include "../exception/exception.h"
#include "../smart_ptr.h"

namespace wa {
    // 多叉树类
    // 目前被用于存放日志
    /*
     * 1. 允许添加MultiTree作为子节点
     * 2. 允许merge两个MultiTree
     * 3. 有hasPath(变长参数)方法来查询路径是否存在
     * 4. 叶子节点只存放数据
     * 5. 非叶子节点只存放其他节点的索引
     */
    template<typename K, typename V>
    class MultiTree {
        struct TreeNode;
        struct LeafNode {
            V value_;
            LeafNode(V value):value_(std::move(value)){}
            ~LeafNode()= default;
        };
        struct NonLeafNode {
            HashMap<K, SP<TreeNode>> children_;
            ~NonLeafNode() = default;
        };

        struct TreeNode {
            Bool isLeaf_;
            SP<LeafNode> leaf_;
            SP<NonLeafNode> nonLeaf_;   //todo:这里是一个不优雅的解决方案
            explicit TreeNode(V value) : isLeaf_(TRUE),leaf_(new LeafNode(std::move(value))) {}
            explicit TreeNode() : isLeaf_(FALSE),nonLeaf_(new NonLeafNode()){}
            explicit TreeNode(SP<NonLeafNode> nonLeafNode):nonLeaf_(std::move(nonLeafNode)){}
            ~TreeNode()=default;
        };

        void merge(SP<TreeNode> a, SP<TreeNode> b) {
            if(!a.get()&&!b.get()){
                return;
            }
            if(a.get()==b.get()){
                return;
            }

            if (a->isLeaf_ && b->isLeaf_) {
                a->leaf_=b->leaf_;  //增加引用计数
            } else if (!a->isLeaf_ && !b->isLeaf_) {
                // 如果两个都是非叶子节点，合并它们的子节点
                SP<NonLeafNode> aNonLeafNode = a->nonLeaf_;
                SP<NonLeafNode> bNonLeafNode = b->nonLeaf_;

                // 遍历 b 的子节点
                for (const auto& child : bNonLeafNode->children_) {
                    K key = child.first;
                    SP<TreeNode> bSubtree = std::move(child.second);

                    // 检查 a 的子节点中是否已经存在该子节点
                    auto it = aNonLeafNode->children_.find(key);
                    if (it != aNonLeafNode->children_.end()) {
                        // 如果存在，递归合并
                        merge(it->second, std::move(bSubtree));
                    } else {
                        // 如果不存在，直接添加
                        aNonLeafNode->children_.emplace(key,std::move(bSubtree));// = std::move(bSubtree); // 直接添加 b 的子节点
                    }
                }
            } else {
                throw CODE_LOCATION_EXCEPTION("node type mismatch");
            }
        }
        // 辅助方法：遍历所有叶子节点
        void forEachLeafHelper(SP<TreeNode> node, const std::function<void(const V&)>& func) const {
            if (!node) return;

            if (node->isLeaf_) {
                func(node->leaf_->value_);
            } else {
                SP<NonLeafNode> nonLeafNode = node->nonLeaf_;
                for (const auto& child : nonLeafNode->children_) {
                    forEachLeafHelper(child.second, func);
                }
            }
        }

        SP<TreeNode> root_;
    public:
        // 叶子节点构造函数
        explicit MultiTree(V value) : root_(new TreeNode(value)) {};

        explicit MultiTree(SP<TreeNode> data) : root_(std::move(data)) {};

        explicit MultiTree() : root_(new TreeNode()) {};

        Bool isLeaf() const {
            return root_->isLeaf_;
        }

        // 返回第depth层的结果
        MultiTree searchPath(const Vector<K> &path, Int depth) const {
            if (depth > path.size()) {
                throw CODE_LOCATION_EXCEPTION(
                        "depth > path.size()" + std::to_string(depth) + " > " + std::to_string(path.size()));
            }
            if (depth == 0) {
                return MultiTree(*this);
            }


            SP<TreeNode> currentNode = root_;
            Int currentDepth = 0;
            while (currentDepth < depth) {
                if (currentNode->isLeaf_) {
                    throw CODE_LOCATION_EXCEPTION("leaf node can not search");
                }
                SP<NonLeafNode> nonLeafNode = currentNode->nonLeaf_;
                auto it = nonLeafNode->children_.find(path[currentDepth]);

                if (it != nonLeafNode->children_.end()) {
                    currentNode = it->second;                   // 更新当前树为找到的子树
                    currentDepth++;                             // 增加当前深度
                } else {
                    //抛出异常
                    throw CODE_LOCATION_EXCEPTION("path not found");
                }
            }

            return MultiTree(std::move(currentNode));
        }

        MultiTree searchPathCreate(const Vector<K> &path, Int depth) {
            if (depth > path.size()) {
                throw CODE_LOCATION_EXCEPTION(
                        "depth > path.size()" + std::to_string(depth) + " > " + std::to_string(path.size()));
            }

            if (depth == 0) {
                return MultiTree(root_);
            }

            SP<TreeNode> currentNode = root_;
            Int currentDepth = 0;
            while (currentDepth < depth) {
                if (currentNode->isLeaf_) {
                    throw CODE_LOCATION_EXCEPTION("leaf node can not search");
                }
                SP<NonLeafNode> nonLeafNode = currentNode->nonLeaf_;
                auto it = nonLeafNode->children_.find(path[currentDepth]);

                if (it != nonLeafNode->children_.end()) {
                    currentNode = it->second; // 更新当前树为找到的子树
                } else { // 创建出非叶节点并移动
                    SP<TreeNode> newChildTree(new TreeNode());
                    nonLeafNode->children_.emplace(path[currentDepth],newChildTree);// 将新子树添加到当前节点的子节点中
                    currentNode = std::move(newChildTree); // 更新当前节点为新创建的子树
                }
                currentDepth++;
            }

            return MultiTree(std::move(currentNode));
        }

        void merge(const Vector<K> &path, MultiTree &other) {
            MultiTree parent = searchPathCreate(path, path.size() - 1);
            if(parent.isLeaf()){
                throw CODE_LOCATION_EXCEPTION("leaf node can not merge");
            }
            SP<NonLeafNode> parentNonLeafNode = parent.root_->nonLeaf_;
            K lastKey = path.back();

            auto it = parentNonLeafNode->children_.find(lastKey);
            if (it != parentNonLeafNode->children_.end()) {
                merge(it->second,other.root_);
            } else {
                parentNonLeafNode->children_[lastKey] = other.root_;
            }
        }

        void setValue(const Vector<K> &path, V value) {
            MultiTree parent = searchPathCreate(path, path.size() - 1);
            if (parent.isLeaf()) {
                throw CODE_LOCATION_EXCEPTION("Cannot add value to a leaf node");
            }

            SP<NonLeafNode> nonLeafNode = parent.root_->nonLeaf_;
            K lastKey = path.back();
            // 检查 lastKey 是否已经存在
            auto it = nonLeafNode->children_.find(lastKey);
            if (it != nonLeafNode->children_.end()) {
                // 如果存在，更新现有叶子节点的值
                if (it->second->isLeaf_) {
                    it->second->leaf_->value_ = value; // 更新值
                } else {
                    throw CODE_LOCATION_EXCEPTION("Cannot set value to a non-leaf node");
                }
            } else {
                // 如果不存在，创建新的叶子节点并添加
                nonLeafNode->children_.emplace(lastKey,new TreeNode(value));
            }
        }

        Bool hasNonLeaf(const Vector<K> &path) const {
            try {
                MultiTree childTree = searchPath(path, path.size());
                return !childTree.isLeaf(); // 如果不是叶子节点，则是非叶子节点
            } catch (const CodeLocationException&) {
                return FALSE; // 路径不存在
            }
        }

        Bool hasLeaf(const Vector<K> &path) const {
            try {
                MultiTree childTree = searchPath(path, path.size());
                return childTree.isLeaf();
            } catch (const CodeLocationException&) {
                return FALSE; // 路径不存在
            }
        }

        Bool hasNode(const Vector<K> &path) const {
            try {
                MultiTree childTree = searchPath(path, path.size());
                return TRUE; // 如果能找到路径，说明存在节点
            } catch (const CodeLocationException&) {
                return FALSE; // 路径不存在
            }
        }

        // 新增方法: getValue
        V getValue(const Vector<K> &path, const V &defaultValue) const {
            try {
                MultiTree childTree = searchPath(path, path.size());
                if (childTree.isLeaf()) {
                    return childTree.root_->leaf_->value_;
                } else {
                    return defaultValue; // 不是叶子节点
                }
            } catch (const CodeLocationException&) {
                return defaultValue; // 路径不存在
            }
        }

        // 遍历所有叶子节点
        void forEachLeaf(const std::function<void(const V&)>& func) const {
            forEachLeafHelper(root_, func);
        }

        // 遍历所有节点
        void forEach(const std::function<void(const K&, const V&)>& func) const {
            forEachHelper(root_, func);
        }
    };
}