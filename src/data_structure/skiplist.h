// 跳表
#pragma once
#include "../global_def.h"
#include "../exception/exception.h"
#include <sstream>
#include <iostream>

namespace wa{
    template <class K,class V>
    class SkipList{
    private:
        F32 promoteP_;   // 提升的概率
        Int maxHeight_;  // 跳表允许的最大高度

        struct DataNode{
            K key;
            V val;
            DataNode* backward;
        };

        struct IndexNode{
            K key;
            IndexNode* backward;
            IndexNode* downward;
        };

        std::vector<IndexNode> indexHeaders_;
        DataNode dataHeader_;
        Int randomLevel(){
            Int level = 1;
            // 当 level < MAX_LEVEL，且随机数小于设定的晋升概率时，level + 1
            // 这里的promoteP_已经是0~100的数字了
            while (rand()%100 < promoteP_ && level < maxHeight_){
                level += 1;
            }
            return level;
        }
    public:
        SkipList(F32 promote,Int maxHeight):promoteP_(promote*100), dataHeader_({K{},V{}, nullptr}){
            if(maxHeight<=1){
                throw CODE_LOCATION_EXCEPTION("param error at maxHeight"+std::to_string(maxHeight));
            }else{
                maxHeight_=maxHeight;
            }

            // 初始化indexHeaders_
            // 第1层对应indexHeaders_的第0个
            IndexNode* lastHeader=reinterpret_cast<IndexNode*>(&dataHeader_);
            indexHeaders_.reserve(maxHeight_-1);
            for(Int i=0;i<maxHeight_-1;i++){
                indexHeaders_.push_back({K{}, nullptr,lastHeader});
                lastHeader=& indexHeaders_[i];
            }
        }

        V get(K key,V defaultValue) const{
            const IndexNode* ptr=&indexHeaders_[maxHeight_-2];
            // 从最高层开始，逐层向下查找
            // 从maxHeight往下找到第2层结束
            for (Int i = maxHeight_; i >= 2; i--) {
                // 在当前层，找到最后一个小于等于 key 的节点
                while (ptr->backward && ptr->backward->key <= key) {
                    ptr = ptr->backward;
                }
                ptr = ptr->downward;  // 移动到下一层
            }

            const DataNode* dataNode=reinterpret_cast<const DataNode*>(ptr);
            if(dataNode->key == key){  // 特判
                return dataNode->val;
            }

            while (dataNode->backward && dataNode->backward->key < key) {
                dataNode=dataNode->backward;
            }

            if(dataNode->backward && dataNode->backward->key == key){
                return dataNode->backward->val;
            }

            return defaultValue;
        }

        Bool add(K key,V value) {
            Int level=randomLevel();

            std::vector<IndexNode*> location;
            IndexNode* indexPtr= nullptr;
            if(level>=2){
                indexPtr=&indexHeaders_[maxHeight_ - 2];
                location.resize(level-1);
                for (Int i = maxHeight_; i >= 2; i--) {
                    // 在当前层，找到最后一个小于等于 key 的节点
                    while (indexPtr->backward && indexPtr->backward->key <= key) {
                        indexPtr = indexPtr->backward;
                    }
                    if(i<=level){
                        location[i-2]=indexPtr;     // 记录更新位置
                    }
                    indexPtr = indexPtr->downward;  // 移动到下一层
                }
            }


            // 完成dataHeader_链表的插入操作
            DataNode* dataPtr= indexPtr== nullptr? &dataHeader_ : reinterpret_cast<DataNode*>(indexPtr);
            if(dataPtr->key==key){  // 特判
                return FALSE;
            }
            while (dataPtr->backward && dataPtr->backward->key < key) {
                dataPtr = dataPtr->backward;
            }

            if(dataPtr->backward && dataPtr->backward->key==key){
                return FALSE;
            }

            DataNode *newDataNode = new DataNode({key, value, dataPtr->backward});
            dataPtr->backward = newDataNode;

            // 逐层插入
            indexPtr=reinterpret_cast<IndexNode*>(newDataNode);
            for(Int i= 2;i<=level;i++){
                IndexNode* newIndexNode=new IndexNode({key, location[i-2]->backward, indexPtr});
                location[i-2]->backward=newIndexNode;
                indexPtr=newIndexNode;
            }

            return TRUE;
        }

        Bool set(K key,V value){
            IndexNode* indexNode=&indexHeaders_[maxHeight_-2];
            for(Int i=maxHeight_;i>=2;i--){
                while(indexNode->backward&&indexNode->backward->key<=key){
                    indexNode=indexNode->backward;
                }

                indexNode=indexNode->downward;
            }


            DataNode* dataNode=reinterpret_cast<DataNode*>(indexNode);
            if(dataNode->key==key){
                dataNode->val=value;
                return TRUE;
            }

            while(dataNode->backward&&dataNode->backward->key<key){
                dataNode=dataNode->backward;
            }

            if(dataNode->backward&&dataNode->backward->key==key){
                dataNode->val=value;
                return TRUE;
            }

            return add(key, value);
        }


        Bool del(K key){
            return FALSE;
        }

        Bool contain(K key) const{
            return FALSE;

        }

        String dump() const {
            std::ostringstream oss;

            oss << "{\n";
            oss << "  \"SkipList\": {\n";

            // 打印每一层的内容
            for (int i = maxHeight_; i >= 2; --i) {
                const IndexNode* current = &indexHeaders_[i-2];
                oss << "    \"Level " << i << "\": [";

                bool first = true;
                while (current->backward) {
                    current = current->backward;
                    if (!first) oss << ", ";
                    oss << current->key;
                    first = false;
                }
                oss << "],\n";
            }

            // 打印数据层
            DataNode* current = dataHeader_.backward;
            oss << "    \"Level 1\": [";
            bool first = true;
            while (current) {
                if (!first) oss << ", ";
                oss << current->key;
                current = current->backward;
                first = false;
            }
            oss << "]\n";

            oss << "  }\n";
            oss << "}";

            return oss.str();
        }

    };
};