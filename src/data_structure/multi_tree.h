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
            virtual HashMap<K,SP<TreeNode>>& getChildren(){
                throw CODE_LOCATION_EXCEPTION("should not reach");
            }
            virtual ~TreeNode() = default;
        };

        class LeafNode: public TreeNode{
            V value_;
        public:
            explicit LeafNode(V value): TreeNode(TRUE),value_(std::move(value)){}
            V value(){
                return value_;
            }
            void setValue(V value){
                this->value_=std::move(value);
            }
            ~LeafNode()= default;
        };
        class NonLeafNode: public TreeNode{
        public:

            HashMap<K,SP<TreeNode>> children_;
            explicit NonLeafNode(): TreeNode(FALSE),children_(){};
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
            HashMap<K,SP<TreeNode>>& getChildren(){
                return children_;
            }
            ~NonLeafNode()= default;
        };
    private:
        SP<TreeNode> root_;
        SP<TreeNode> searchPath(const Vector<K>& path,Int depth){
            if (path.empty() || depth==0) {
                return root_; // 如果路径为空
            }
            if(depth<path.size()){
                throw CODE_LOCATION_EXCEPTION("path.size() must >= depth");
            }
            // 不断向下找到对应节点
            SP<TreeNode> current=this->root_;
            for(Int i=0;i<depth-1;i++) {
                K &k = path[i];
                auto children=current->getChildren();
                SP<TreeNode> foundChild = children[k];

                if (foundChild != children.end()) {
                    if (foundChild->isLeaf()) {
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current = std::move(foundChild);
                } else {
                    throw CODE_LOCATION_EXCEPTION("error path");
                }
            }

            // 再向下搜索一层
            SP<TreeNode> foundChild = current->getChildren()[path[depth-1]];
            if(foundChild==current->getChildren().end()){
                return SP<TreeNode>();
            }
            return foundChild;
        }

        SP<TreeNode> searchPathCreate(const Vector<K>& path,Int depth){
            if (path.empty() || depth==0) {
                return root_; // 如果路径为空
            }
            if(depth<path.size()){
                throw CODE_LOCATION_EXCEPTION("path.size() must >= depth");
            }

            // 不断向下找到对应节点
            SP<TreeNode> current=this->root_;
            for(Int i=0;i<depth-1;i++) {
                K &k = path[i];
                auto children=current->getChildren();
                SP<TreeNode> foundChild = children[k];

                if (foundChild != children.end()) {
                    if (foundChild->isLeaf()) {
                        throw CODE_LOCATION_EXCEPTION("error path");
                    }
                    current = std::move(foundChild);
                } else {
                    // 创建子节点
                    SP<TreeNode> newChild(new NonLeafNode());

                    current->addChild(k, newChild);
                    current=std::move(newChild);
                }
            }

            // 再向下搜索一层
            SP<TreeNode> foundChild = current->getChildren()[path[depth-1]];
            if(foundChild==current->getChildren().end()){
                // 创建子节点
                SP<TreeNode> newChild(new NonLeafNode());

                current->addChild(path.back(), newChild);
                current=std::move(newChild);
                return current;
            }else{
                return foundChild;
            }
        }

    public:

        MultiTree():root_((TreeNode*) new NonLeafNode()){}
        explicit MultiTree(SP<TreeNode> root):root_(std::move(root)){}
        ~MultiTree()= default;
        void merge(MultiTree<K,V>& otherTree){
            root_->merge(otherTree.root_);
        }

        // 递归添加value
        // 此操作会递归的创建树
        void addValue(const Vector<K>& path,V value) {
            auto tree= searchPathCreate(path,path.size()-1);
            if(!tree->isLeaf()){
                SP<TreeNode> valueNode((TreeNode*) new LeafNode(value));
                tree->addChild(path.back(),valueNode);
            }else{
                throw CODE_LOCATION_EXCEPTION("Leaf node can not addChild");
            }
        }

        Bool hasPath(const Vector<K>& path)const{ // 通过变长参数定义
            try {
                return searchPath(path,path.size()).get()!=NULLPTR;
            }catch(const Exception &e) {
                return FALSE;
            }
        }

        Bool isLeaf(const Vector<K>& path)const{
            try {
                SP<TreeNode> node = searchPath(path, path.size());
                return node.get() != NULLPTR && node->isLeaf();
            }catch(const Exception &e) {
                return FALSE;
            }
        }

        Bool isNonLeaf(const Vector<K>& path)const{
            try {
                SP<TreeNode> node = searchPath(path, path.size());
                return node.get() != NULLPTR && !node->isLeaf();
            }catch(const Exception &e) {
                return FALSE;
            }
        }

        MultiTree<K,V> getChild(const Vector<K>& path){
            return MultiTree<K,V> (searchPath(path,path.size()));
        }

        V getValue(const Vector<K>& path,V defaultValue) const{
            SP<TreeNode> child= searchPath(path,path.size());
            if(child.get()==NULLPTR || !child->isLeaf()){
                return defaultValue;
            }
            return child->value();
        }
    };
}