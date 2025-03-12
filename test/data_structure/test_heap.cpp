#include <iostream>

#include "src/data_structure/heap.h"

using namespace wa;

int main() {
    // 测试最小堆
    wa::Heap<int, wa::MinHeapComparator<int>> minHeap(10);
    minHeap.add(5);
    minHeap.add(3);
    minHeap.add(8);
    minHeap.add(1);
    minHeap.add(4);
    minHeap.add(2);
    while(minHeap.size()){
        std::cout<<minHeap.top()<<std::endl;
        minHeap.pop();
    }

    return 0;
}