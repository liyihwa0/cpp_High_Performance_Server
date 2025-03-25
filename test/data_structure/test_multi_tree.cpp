#include <iostream>
#include "src/data_structure/multi_tree.h"

using namespace wa;
using namespace std;

int main() {
    // 创建一个 MultiTree
    MultiTree<String, int> tree;

    // 测试 1: 添加值到树中
    tree.setValue({"root", "child1", "leaf1"}, 10);
    tree.setValue({"root", "child1", "leaf2"}, 20);
    tree.setValue({"root", "child2", "leaf3"}, 30);

    // 测试 2: 检查路径是否存在
    cout << "Path 'root/child1/leaf1' exists: " << tree.hasPath({"root", "child1", "leaf1"}) << endl; // 应该输出 1 (true)
    cout << "Path 'root/child1/leaf3' exists: " << tree.hasPath({"root", "child1", "leaf3"}) << endl; // 应该输出 0 (false)

    // 测试 3: 获取值
    cout << "Value at 'root/child1/leaf1': " << tree.getValue({"root", "child1", "leaf1"}, -1) << endl; // 应该输出 10
    cout << "Value at 'root/child1/leaf3': " << tree.getValue({"root", "child1", "leaf3"}, -1) << endl; // 应该输出 -1 (默认值)

    // 测试 4: 检查节点类型
    cout << "Is 'root/child1/leaf1' a leaf node: " << tree.isLeaf({"root", "child1", "leaf1"}) << endl; // 应该输出 1 (true)
    cout << "Is 'root/child1' a non-leaf node: " << tree.isNonLeaf({"root", "child1"}) << endl; // 应该输出 1 (true)

    // 测试 5: 合并两棵树
    MultiTree<String, int> otherTree;
    otherTree.setValue({"root", "child1", "leaf1"}, 100); // 覆盖原有值
    otherTree.setValue({"root", "child3", "leaf4"}, 40);  // 添加新值
    Vector<String> path;
    tree.merge(path ,otherTree);

    cout << "Value at 'root/child1/leaf1' after merge: " << tree.getValue({"root", "child1", "leaf1"}, -1) << endl; // 应该输出 100
    cout << "Value at 'root/child3/leaf4' after merge: " << tree.getValue({"root", "child3", "leaf4"}, -1) << endl; // 应该输出 40

    // 测试 6: 获取子节点
    auto childTree = tree.getChild({"root", "child1"});
    cout << "Value at 'leaf1' in child tree: " << childTree.getValue({"leaf1"}, -1) << endl; // 应该输出 100

    return 0;
}