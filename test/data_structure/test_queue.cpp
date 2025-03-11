#include "src/global_def.h"
#include "src/data_structure/queue.h"
#include "iostream"
using namespace std;
using namespace wa;
int main() {
    CircularQueue<int> cq(3);

    cq.enqueue(1);
    cq.enqueue(2);
    cq.enqueue(3);

    std::cout << "Front: " << cq.front() << std::endl;

    cq.dequeue();
    cq.enqueue(4);

    while (!cq.empty()) {
        std::cout << "Front: " << cq.front() << std::endl;
        cq.dequeue();
    }

    return 0;
}