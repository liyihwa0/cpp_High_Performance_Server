
#include <iostream>
#include "src/exception/exception.h"

int t(){
    throw ERRNO_EXCEPTION(1);
}
int t2(){
    throw CODE_LOCATION_EXCEPTION();
}
int main() {
    t2();
    return 0;
}
