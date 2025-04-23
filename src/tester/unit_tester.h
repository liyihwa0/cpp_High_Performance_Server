#include "../global_def.h"
#include "../util/caster.h"
#include "../util/file_reader.h"
#include "../util/helper.h"
namespace wa{
    namespace tester{
        /*
         * DataInputer会将输入的多行字符串以空白字符进行分割,无论是空格,tab,还是空行,将由空白字符分割的看作一部分
         * 如果需要使用多行字符串,请用 ""包裹
         * DataInputer通过getNext每次获取一个期望类型的变量
         */
        class InputDataOutOfRange : public std::exception {
        private:
            std::string message_;
        public:
            InputDataOutOfRange(const std::string& message = "Input data is out of range")
                    : message_(message) {}

            virtual const char* what() const noexcept override {
                return message_.c_str();
            }
        };

        class ExpectedDataOutOfRange : public std::exception {
        private:
            std::string message_;
        public:
            ExpectedDataOutOfRange(const std::string& message = "Expected data is out of range")
                    : message_(message) {}

            virtual const char* what() const noexcept override {
                return message_.c_str();
            }
        };

        class InputDataTypeMismatch : public std::exception {
        private:
            std::string message_;
        public:
            InputDataTypeMismatch(const std::string& message = "Input data type mismatch")
                    : message_(message) {}

            virtual const char* what() const noexcept override {
                return message_.c_str();
            }
        };

        class ExpectedDataMismatch : public std::exception {
        private:
            std::string message_;
        public:
            ExpectedDataMismatch(const String& expect,const String& got)
                    : message_("Expected Data Mismatch, expect "+expect+",got "+got) {}

            virtual const char* what() const noexcept override {
                return message_.c_str();
            }
        };
        
        class DataInputer {
        private:
            String tempValue_;
            std::ifstream  inputStream_; // 输入流，可以是 std::cin 或 std::ifstream

            void nextItem() {
                tempValue_.clear(); // 清空之前的值
                Char ch;
                
                while (inputStream_.get(ch)) {
                    if (ch == '\"') {
                        // 进入读取多行字符串状态,直到遇到 "
                        while(inputStream_.get(ch)){
                            if(ch=='\\'){
                                if(!inputStream_.get(ch)){
                                    throw InputDataOutOfRange();
                                }
                                tempValue_.push_back(ch);
                            }else if(ch=='\"'){
                                break;
                            }else{
                                tempValue_.push_back(ch);
                            }
                        }
                    }else if (Helper::IsSpaceChar(ch)) {
                        return;
                    }else{
                        tempValue_.push_back(ch);
                    }
                }
            }

        public:
            DataInputer(const String& file) : inputStream_(ifstream(file)) {
                if(!inputStream_.is_open()){
                    throw FILE_NOT_FOUND_EXCEPTION(file);
                }
                nextItem();
            }
            bool hasMore() const {
                return !tempValue_.empty();
            }

            template<class T>
            T getNext() {
                if (!hasMore()) {
                    throw InputDataOutOfRange();
                }
                T data = CastAsType<T>(tempValue_); // 假设有合适的转换
                nextItem();
                return data;
            }
        };
    
        class UnitTester{
        private:
            DataInputer dataInputer_;
            std::ifstream expectedDatas_;
            
        public:
            UnitTester(const String& inputFile, const String& outputFile)
                    : dataInputer_(inputFile), expectedDatas_(outputFile) {
                if(!expectedDatas_.is_open()){
                    throw FILE_NOT_FOUND_EXCEPTION(outputFile);
                }
            }
            template<class T>
            T getNext() {
                return dataInputer_.getNext<T>();
            }
            Bool inputHasMore() const {
                return dataInputer_.hasMore();
            }
            
            template<typename... Args>
            void expectValues(Args... args) {
                (([this](const auto& arg) {
                    String expectedStr;
                    // 吞掉前缀空白字符
                    Char c;
                    while(expectedDatas_.get(c)){
                       if(!Helper::IsSpaceChar(c)){
                           break;
                       }
                    }

                    expectedStr.push_back(c);
                    String argStr = ToString(arg);
                    expectedStr.resize(argStr.length());
                    expectedDatas_.read(&expectedStr[1], argStr.length()-1);
                    if (expectedDatas_.gcount() < argStr.length()-1) {
                        throw ExpectedDataOutOfRange();
                    }
                    if(argStr!=expectedStr){
                        throw ExpectedDataMismatch(expectedStr,argStr);
                    }
                }(args)), ...); // 展开参数包并调用 lambda
            }
            
            void cleanUp(){
                if (expectedDatas_.good()) {
                    String remainingData;
                    Char c;
                    while (expectedDatas_.get(c)) {
                        if(!Helper::IsSpaceChar(c)){
                            std::cerr << "Expected Data has remaining data" <<std::endl;
                            exit(-1);
                        }
                    }
                }
            }
        };
    };
};