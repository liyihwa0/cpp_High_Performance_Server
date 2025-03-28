#include <iostream>
#include "src/data_structure/multi_tree.h"

using namespace wa;
using namespace std;


int main() {
    // 创建一个 MultiTree 实例
    MultiTree<string, int> tree; // 根节点的值为 0

    // 添加一些值
    tree.setValue({"A", "C"}, 3);
    cout << "Value at path {A, C}: " << tree.getValue({"A", "C"},-1) << endl; // 应该输出 3

    tree.setValue({"A", "D"}, 4);
    cout << "Value at path {A, D}: " << tree.getValue({"A", "D"},-1) << endl; // 应该输出 4

    tree.setValue({"B", "E"}, 5);
    cout << "Value at path {B, E}: " << tree.getValue({"B", "E"},-1) << endl; // 应该输出 5

    // 测试 get 方法

    // 测试 hasLeaf 方法
    cout << "Has leaf at path {A}: " << (tree.hasLeaf({"A"}) ? "true" : "false") << endl; // 应该输出 false
    cout << "Has leaf at path {B}: " << (tree.hasLeaf({"B"}) ? "true" : "false") << endl; // 应该输出 false
    cout << "Has leaf at path {A, C}: " << (tree.hasLeaf({"A", "C"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has leaf at path {A, D}: " << (tree.hasLeaf({"A", "D"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has leaf at path {B, E}: " << (tree.hasLeaf({"B", "E"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has leaf at path {A, X}: " << (tree.hasLeaf({"A", "X"}) ? "true" : "false") << endl; // 应该输出 false

    // 测试 hasNode 方法
    cout << "Has node at path {A}: " << (tree.hasNode({"A"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has node at path {B}: " << (tree.hasNode({"B"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has node at path {A, C}: " << (tree.hasNode({"A", "C"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has node at path {A, D}: " << (tree.hasNode({"A", "D"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has node at path {B, E}: " << (tree.hasNode({"B", "E"}) ? "true" : "false") << endl; // 应该输出 true
    cout << "Has node at path {A, X}: " << (tree.hasNode({"A", "X"}) ? "true" : "false") << endl; // 应该输出 false

    // 测试 hasNonLeaf 方法
    cout << "Has non-leaf at path {A}: " << (tree.hasNonLeaf({"A"}) ? "true" : "false") << endl; // 应该输出 false
    cout << "Has non-leaf at path {B}: " << (tree.hasNonLeaf({"B"}) ? "true" : "false") << endl; // 应该输出 false
    cout << "Has non-leaf at path {A, C}: " << (tree.hasNonLeaf({"A", "C"}) ? "true" : "false") <<endl; // 应该输出 false

    // 应该输出 false
    cout << "Has non-leaf at path {A, D}: " << (tree.hasNonLeaf({"A", "D"}) ? "true" : "false") << endl; // 应该输出 false
    cout << "Has non-leaf at path {B, E}: " << (tree.hasNonLeaf({"B", "E"}) ? "true" : "false") << endl; // 应该输出 false
    cout << "Has non-leaf at path {A, X}: " << (tree.hasNonLeaf({"A", "X"}) ? "true" : "false") << endl; // 应该输出 false

    // 测试 getValue 方法
    cout << "Get value at path {A, C} with default 99: " << tree.getValue({"A", "C"}, 99) << endl; // 应该输出 3
    cout << "Get value at path {A, X} with default 99: " << tree.getValue({"A", "X"}, 99) << endl; // 应该输出 99
    cout << "Get value at path {B} with default 99: " << tree.getValue({"B"}, 99) << endl; // 应该输出 2

    // 测试合并功能
    MultiTree<string, int> otherTree;
    otherTree.setValue({"B", "F"}, 6);
    otherTree.setValue({"A", "C", "G"}, 7);
    otherTree.setValue({"B","F"},1);

    tree.merge({"B"}, otherTree); // 合并 otherTree 到 tree 的 B 节点
    cout << "Value at path {B, B, F} after merge: " << tree.getValue({"B","B", "F"},-1) << endl; // 应该输出 1
    cout << "Value at path {B, A, C, G} after merge: " << tree.getValue({"B","A", "C", "G"},-1) << endl; // 应该输出 7

    // 测试 forEach 方法
    tree.forEach([](const Vector<string>& path,const MultiTree<string, int>& node) {
        if (node.isLeaf()) {
            cout << "Path: ";
            for (const auto& key : path) {
                cout << key << " ";
            }
            cout << "-> Value: " << node.getValue({}, -1) << endl;
        }
    });

    return 0;
}