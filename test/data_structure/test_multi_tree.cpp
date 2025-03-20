#include "src/data_structure/multi_tree.h"
#include "src/global_def.h"
#include "src/logger/logger.h"
using namespace wa;
using namespace std;
class Base{
public:
    void print(){
        cout<<"123";
    }
};
class Child:Base{};
SP<Base> sp(new Base);
int main() {
    sp->print();
    return 0;
}