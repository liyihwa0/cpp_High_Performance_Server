#pragma once
#include "../global_def.h"
#include "../exception/exception.h"
#include "../smart_ptr.h"

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
            virtual void merge(SP<TreeNode> other)const{
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }
            virtual void addChild(K key, SP<TreeNode> child)const{
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }
            virtual void setValue(V value){
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }
            virtual V value() const {
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }
            virtual ~TreeNode() = default;
        };

        class LeafNode:TreeNode{
            V value_;
        public:
            explicit LeafNode(V value): TreeNode(TRUE),value_(std::move(value)){}
            V value(){
                return value_;
            }
            void setValue(V value){
                this->value_=std::move(value);
            }
        };
        class NonLeafNode:TreeNode{
        public:
            HashMap<K,SP<TreeNode>> children_;
            void addChild(K key, SP<TreeNode> child) {       // 修正后的 addChild 函数
                children_.emplace(key, std::move(child));
            }
            void merge(SP<TreeNode> other){
                for (auto otherChild : other->children_) {
                    K key = otherChild.first;
                    SP<TreeNode> child = std::move(otherChild.second);
                    auto thisChild=children_.find(key);
                    if (thisChild != children_.end()) { // 如果当前节点已经有这个键,则进行合并
                        //判断otherChild的类型
                        if(thisChild->second->isLeaf()!=child->isLeaf()){   //类型不同,合并出错
                            throw CODE_LOCATION_EXCEPTION("type dismatch");
                        }
                        if(thisChild->second->isLeaf()){ //是叶子节点,则需要进行值覆盖
                            thisChild->second->setValue(child->value());
                        }else{  //是非叶节点,需要进行合并
                            thisChild->second->merge(child);
                        }
                    } else {
                        // 如果没有冲突，直接添加
                        children_.emplace(key, child);
                    }
                }
            }
        };
    private:
        SP<TreeNode> getChild_(const Vector<K>& path){
            if (path.empty()) {
                return SP<TreeNode>(); // 如果路径为空，直接返回
            }

            // 不断向下找到对应节点
            SP<TreeNode> current=this->root_;
            for(Size i=0;i<path.size()-1;i++) {
                K &k = path[i];
                SP<TreeNode> foundChild = current->children_[path[i]];

                if (foundChild != current->children_.end()) {
                    if (foundChild->isLeaf()) {
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current = std::move(foundChild);
                } else {
                    throw CODE_LOCATION_EXCEPTION("error path");
                }
            }

            // 再向下搜索一层
            SP<TreeNode> foundChild = current->children_[path.back()];
            if(foundChild==current->children_.end()){
                return MultiTree<K,V>();
            }
            current=std::move(foundChild);
            // 创建新的 MultiTree 对象，并将找到的节点作为其根节点
            MultiTree<K, V> childTree;
            childTree.root_ = current;
            return childTree;
        }

        SP<TreeNode> root_;
        void merge(MultiTree<K,V>& otherTree){
            root_->merge(otherTree.root_);
        }

        // 递归添加value
        // 此操作会递归的创建树
        void addValue(const Vector<K>& path,V value) {
            if (path.empty()) {
                return; // 如果路径为空，直接返回
            }

            // 不断向下找到对应节点
            SP<TreeNode> current=this->root_;
            for(Size i=0;i<path.size()-1;i++){
                K& k=path[i];
                SP<TreeNode> foundChild=current->children_[path[i]];

                if(foundChild != current->children_.end()){
                    if(foundChild->isLeaf()){
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current=std::move(foundChild);
                }else{
                    // 创建子节点
                    SP<TreeNode> newChild(new NonLeafNode());

                    current->addChild(k, newChild);
                    current=std::move(newChild);
                }
            }

            // 我们需要再向下一层,创建出valueNode


            current->setValue(value);
        }

        Bool hasPath(const Vector<K>& path){ // 通过变长参数定义
            SP<TreeNode> current = root_;
            for(Size i=0;i<path.size()-1;i++){
                K k=path[i];
                SP<TreeNode> foundChild=current->children_[k];
                if(foundChild!=current->children_.end()){
                    if(foundChild->isLeaf()){
                        return FALSE;
                    }
                    current=std::move(foundChild);
                }else{
                    return FALSE;
                }
            }
            // 我们还需要再向下一层todo
            return current->children_[path[path.size()-1]];
        }

        MultiTree<K,V> getChild(const Vector<K>& path){
            if (path.empty()) {
                return MultiTree<K,V>(); // 如果路径为空，直接返回
            }

            // 不断向下找到对应节点
            SP<TreeNode> current=this->root_;
            for(Size i=0;i<path.size()-1;i++) {
                K &k = path[i];
                SP<TreeNode> foundChild = current->children_[path[i]];

                if (foundChild != current->children_.end()) {
                    if (foundChild->isLeaf()) {
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current = std::move(foundChild);
                } else {
                    throw CODE_LOCATION_EXCEPTION("error path");
                }
            }

            // 再向下搜索一层
            SP<TreeNode> foundChild = current->children_[path.back()];
            if(foundChild==current->children_.end()){
                return MultiTree<K,V>();
            }
            current=std::move(foundChild);
            // 创建新的 MultiTree 对象，并将找到的节点作为其根节点
            MultiTree<K, V> childTree;
            childTree.root_ = current;
            return childTree;
        }

        V getValue(const Vector<K>& path,V defaultValue) const{
            MultiTree<K,V> child= getChild(path);

        }
    };
}