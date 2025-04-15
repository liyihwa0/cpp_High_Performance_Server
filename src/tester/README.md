# 测试相关知识
https://www.zhihu.com/question/53143398

# wa-tester usage
> wa-tester是一个测试框架,包括多个模块:
> 1. unit-tester: 用来进行单元测试, 内部包含 data-inputer和verdict
> 2. data-inputer: 数据输入器+转化器,用于将`测例输入`的数据转化`被测单元`依赖的类型,并传入给`被测单元`
> 3. batch-tester: 批量测试器,用于同时运行多个`单元测试`


## 单元测试
在进行单元测试时,不需要使用额外的命令,而是通过引入`UnitTester`模块即可
```cpp
// UnitTester会打开配置文件configFile,并解析其中unitName项
UnitTester uni(configFile,unitName);

int expectRes=0;
Int testFunc(String input1,Int input2){
    return expectRes++;
}
int main(){
    try{
        while(uni.inputHasMore()){
            String input1=uni.getNext<String>();
            Int input2=uni.getNext<Int>();
            uni.expectValues(testFunc(input1,input2));    // 在不符合时会抛出输出有误的异常
        }
    }catch(Exception& e){  // 处理输入格式有误/输入长度有误/输出有误/输出长度有误
        uni.handleException(e);
    }
    
    uni.cleanUp();  // 在这里output的剩余行,并处理输出长度有误的情况
return 0;
}
```


## 批量测试
它可以读取`ini`文件作为配置,扫描特定目录下的`可执行文件`,`测例输入`,`期望输出`,并对比结果

示例配置如下:
```ini
[common]
baseDir=/xxx/xxx/base
skip=test1;

[test1]
binFile=/xxx/xxx/bin
testFile=/xxx/test/1.test;/xxx/test/2.test

[otherTest]
```
需要满足:
1. baseDir下有 `/test/`目录用来存放test文件 `.test`
2. baseDir下有`/expect/`目录用来存放预期结果文件 `.expect`
3. `test`与`expect`下的文件名前缀必须一一对应

使用方式:
```shell
wa_tester -c configFilePath -s test1;test2 
wa_tester --config-file=configFilePath --skip-test=test1;test2
```


# wa-tester自身的模块
