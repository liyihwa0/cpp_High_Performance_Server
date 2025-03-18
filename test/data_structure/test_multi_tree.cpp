#include "src/data_structure/multi_tree.h"

using namespace wa;
using namespace std;
// 测试类
class MultiTreeTest {
public:
    void runTests() {
        testNonLeafNode();
    }

private:
    void testNonLeafNode() {
        using Tree = MultiTree<int, std::string>;
        Tree tree1;
        Tree tree2;

        // 创建非叶子节点并添加子节点
        auto nonLeaf1 = SP<MultiTree<String,String>::NonLeafNode>();
        auto nonLeaf2 = std::make_shared<typename Tree::NonLeafNode>();

        // 添加叶子节点到 tree1
        nonLeaf1->addChild(1, std::make_shared<typename Tree::LeafNode>("Leaf 1"));
        nonLeaf1->addChild(2, std::make_shared<typename Tree::LeafNode>("Leaf 2"));

        // 添加叶子节点到 tree2
        nonLeaf2->addChild(2, std::make_shared<typename Tree::LeafNode>("Leaf 2 Updated"));
        nonLeaf2->addChild(3, std::make_shared<typename Tree::LeafNode>("Leaf 3"));

        // 合并 tree2 到 tree1
        nonLeaf1->merge(nonLeaf2);

        // 输出合并后的结果
        printTree(nonLeaf1);
    }

    void printTree(std::shared_ptr<typename MultiTree<int, std::string>::NonLeafNode> node, int depth = 0) {
        if (!node) return;

        for (const auto& child : node->getChildren()) {
            std::cout << std::string(depth * 2, ' ') << "Key: " << child.first << ", ";
            if (child.second->isLeaf()) {
                auto leaf = std::dynamic_pointer_cast<typename MultiTree<int, std::string>::LeafNode>(child.second);
                std::cout << "Value: " << leaf->value() << std::endl;
            } else {
                std::cout << "NonLeafNode" << std::endl;
                printTree(std::dynamic_pointer_cast<typename MultiTree<int, std::string>::NonLeafNode>(child.second), depth + 1);
            }
        }
    }
};

int main() {
    MultiTreeTest test;
    test.runTests();
    return 0;
}