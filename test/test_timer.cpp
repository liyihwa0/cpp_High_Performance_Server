//
// Created by Liyihwa on 24-11-15.
//

#include "src/timer.h"
#include "iostream"

using namespace std;
using namespace wa;
int main(){
   auto t= Timer::Now();
    cout<<t.format();
}