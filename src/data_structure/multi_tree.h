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
        class TreeNode {
        public:
            explicit TreeNode() {}

            virtual HashMap<K, SP<TreeNode>> &getChildren() {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }

            virtual Bool isLeaf() const {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }

            virtual void merge(SP<TreeNode> other) {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }

            virtual void addChild(const K &key, SP<TreeNode> child) {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }

            virtual void setValue(V value) {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }

            virtual V value() const {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }

            virtual void forEach(){

            }

            virtual void forEachLeaf(){

            }

            virtual void forEachNonLeaf(){

            }

            virtual ~TreeNode() = default;
        };

        class LeafNode : public TreeNode {
            V value_;
        public:
            explicit LeafNode(V value) : TreeNode(), value_(std::move(value)) {}

            Bool isLeaf() const override {
                return TRUE;
            };

            V value() const override {
                return value_;
            }

            void setValue(V value) override {
                this->value_ = std::move(value);
            }

            void merge(SP<TreeNode> other) override {
                if (!other.get() || !other->isLeaf()) {
                    throw CODE_LOCATION_EXCEPTION("Cannot merge with null or leaf node");
                }
                // 值覆盖
                this->value_ = std::move(other->value());
            }

            ~LeafNode() = default;
        };

        class NonLeafNode : public TreeNode {
        public:

            HashMap<K, SP<TreeNode>> children_;

            explicit NonLeafNode() : TreeNode(), children_() {};

            Bool isLeaf() const override {
                return FALSE;
            }

            void merge(SP<TreeNode> other) override {
                if (!other.get() || other->isLeaf()) {
                    throw CODE_LOCATION_EXCEPTION("Cannot merge with null or leaf node");
                }

                // 获取当前节点和另一个节点的子节点
                auto &thisChildren = this->getChildren();
                auto &otherChildren = other->getChildren();

                // 遍历另一个节点的子节点
                for (const auto &otherChild: otherChildren) {
                    const K &key = otherChild.first;
                    SP<TreeNode> otherNode = otherChild.second;

                    // 查找当前节点是否已经存在相同的 key
                    auto thisChild = thisChildren.find(key);
                    if (thisChild != thisChildren.end()) {
                        thisChild->second->merge(otherNode);
                    } else {
                        // 如果不存在，直接插入
                        thisChildren.emplace(key, std::move(otherNode));
                    }
                }
            }

            void addChild(const K &key, SP<TreeNode> child) override {
                children_.emplace(key, std::move(child));
            }

            HashMap<K, SP<TreeNode>> &getChildren() override {
                return children_;
            }
            void forEach(){

            }
            void forEachLeaf(){}
            void forEachNonLeaf(){}
            ~NonLeafNode() = default;
        };

    private:
        SP<TreeNode> root_;


        SP<TreeNode> searchPath(const Vector<K> &path, Int depth) const {
            if (path.empty() || depth == 0) {
                return root_; // 如果路径为空
            }
            if (depth < path.size()) {
                throw CODE_LOCATION_EXCEPTION("path.size() must >= depth");
            }
            // 不断向下找到对应节点
            SP<TreeNode> current = this->root_;
            for (Int i = 0; i < depth - 1; i++) {
                const K &k = path[i];
                auto children = current->getChildren();
                auto foundChild = children.find(k);

                if (foundChild != children.end()) {
                    if (foundChild->second->isLeaf()) {
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current = foundChild->second;
                } else {
                    throw CODE_LOCATION_EXCEPTION("error path");
                }
            }

            // 再向下搜索一层
            auto foundChild = current->getChildren().find(path[depth - 1]);
            if (foundChild == current->getChildren().end()) {
                return SP<TreeNode>();
            }
            return foundChild->second;
        }

        SP<TreeNode> searchPathCreate(const Vector<K> &path, Int depth) {
            if (path.empty() || depth == 0) {
                return root_; // 如果路径为空
            }
            if (depth > path.size()) {
                throw CODE_LOCATION_EXCEPTION("path.size() must >= depth");
            }

            // 不断向下找到对应节点
            SP<TreeNode> current = this->root_;
            for (Int i = 0; i < depth - 1; i++) {
                const K &k = path[i]; // 使用 const 引用
                auto children = current->getChildren();
                auto foundChild = children.find(k);

                if (foundChild != children.end()) {
                    if (foundChild->second->isLeaf()) {
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current = foundChild->second;
                } else {
                    // 创建子节点
                    SP<TreeNode> newChild(new NonLeafNode());
                    current->addChild(k, newChild);
                    current = std::move(newChild);
                }
            }

            // 再向下搜索一层
            auto foundChild = current->getChildren().find(path[depth - 1]);
            if (foundChild == current->getChildren().end()) {
                // 创建子节点
                SP<TreeNode> newChild(new NonLeafNode());

                current->addChild(path[depth - 1], newChild);
                current = std::move(newChild);
                return current;
            } else {
                return foundChild->second;
            }
        }

    public:

        MultiTree() : root_((TreeNode *) new NonLeafNode()) {}

        explicit MultiTree(SP<TreeNode> root) : root_(std::move(root)) {}

        ~MultiTree() = default;

        void merge(const Vector<String> &path, MultiTree<K, V> other) {
            SP<TreeNode> node = searchPathCreate(path, path.size() - 1);
            if (node->isLeaf()) {
                throw CODE_LOCATION_EXCEPTION("error path");
            }

            node->merge(other.root_);
        }

        // 递归添加value
        // 此操作会递归的创建树
        void setValue(const Vector<K> &path, V value) {
            auto tree = searchPathCreate(path, path.size() - 1);
            if (tree->isLeaf()) {
                throw CODE_LOCATION_EXCEPTION("Leaf node can not addChild");
            }

            tree->getChildren()[path.back()] = SP<TreeNode>(new LeafNode(value));
        }

        Bool hasPath(const Vector<K> &path) const { // 通过变长参数定义
            try {
                return searchPath(path, path.size()).get() != NULLPTR;
            } catch (const Exception &e) {
                return FALSE;
            }
        }

        Bool isLeaf(const Vector<K> &path) const {
            try {
                SP<TreeNode> node = searchPath(path, path.size());
                return node.get() != NULLPTR && node->isLeaf();
            } catch (const Exception &e) {
                return FALSE;
            }
        }

        Bool isNonLeaf(const Vector<K> &path) const {
            try {
                SP<TreeNode> node = searchPath(path, path.size());
                return node.get() != NULLPTR && !node->isLeaf();
            } catch (const Exception &e) {
                return FALSE;
            }
        }

        MultiTree<K, V> getChild(const Vector<K> &path) {
            return MultiTree<K, V>(searchPath(path, path.size()));
        }

        V getValue(const Vector<K> &path, V defaultValue) const {
            SP<TreeNode> child = searchPath(path, path.size());
            if (child.get() == NULLPTR || !child->isLeaf()) {
                return defaultValue;
            }
            return child->value();
        }
    };
}