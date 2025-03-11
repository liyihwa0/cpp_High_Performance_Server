/*
 * 该程序是一个多线程测试工具,思路来自:
 * https://www.bilibili.com/video/BV1Qy411q7Xq
 */


#pragma once
#include "./global_def.h"
#include "./thread/thread.h"
#include "functional"
#include "vector"
#include "unordered_map"
namespace wa{
    template<class T>
    class ThreadTester{
        // doTest方法会执行testCount次callback
        // 并将结果记录以字符串形式返回
    public:
       static String doTest(Int testCount=15000){
            String result;
            std::unordered_map<Int,Int> mp;
            for(Int i=0; i < testCount; i++){
                Int data=0;
                T t;
                Thread t1([&t, &data]{
                    t.task1(data);
                });
                Thread t2([&t , &data]{
                    t.task2(data);
                });
                t2.join();
                t1.join();
                mp[data]++;
            }

            // 格式化为字符串
            for(auto &it:mp){
                result+= std::to_string(it.first)+" : "+std::to_string(it.second)+"\n";
            }
            return result;
        }
    };

}
