#pragma once

#include <regex>
#include "../global_def.h"
#include "../smart_ptr.h"
#include "../data_structure/multi_tree.h"
#include "../util//helper.h"
#include "../util/caster.h"
#include "./configer.h"
#include "../util/file_reader.h"
namespace wa{
    namespace config {
        // 字符串可能是被""包裹的,字符串的前后可能有空格
        String ParseString(const String& str,Int& start,Int end,Char stopAt) {
            // 解析字符串,返回解析后的,直到遇到stopAt字符
            while (start<end&&Helper::IsSpaceChar(str[start])){
                start++;
            }
            
            String result;
            while(start<end){
                Char currentChar=str[start];
                if(currentChar=='\\'){ 
                    if(start+1>=end){
                        throw INVALID_ARGUMENT_EXCEPTION("Bad string " + str);
                    }
                    switch (str[start + 1]) {
                        case '\"':
                            result += '\"';
                            start += 2;
                            continue;
                        case '\\':
                            result += '\\';
                            start += 2;
                            continue;
                        case 'n':
                            result += '\n';
                            start += 2;
                            continue;
                        case 't':
                            result += '\t';
                            start += 2;
                            continue;
                        default:
                            throw INVALID_ARGUMENT_EXCEPTION("Invalid escape sequence: \\" + ToString(str[start + 1]));
                    }
                }else if(currentChar==stopAt){
                    start++;
                    break;
                }else{
                    result += currentChar;
                    start++;
                }
            }
            // 跳过后续空格
            while (start < end && Helper::IsSpaceChar(str[start])) {
                start++;
            }
            return result;
        }

        SP<ConfigValues> ParseIniFile(const String &filename) {
            Vector<String> path;
            path.push_back("");
            ConfigValues *values = new ConfigValues();
            FileReader fileReader(filename);

            while (fileReader.hasMore()) {
                UP<String> line = fileReader.nextLine();
                
                Int i = 0;
                // 跳过前后的空白
                Int len = line->length();
                while (len > 0 && Helper::IsSpaceChar(line->at(len-1))) {
                    len--;
                }
                while (i < len && Helper::IsSpaceChar(line->at(i))) i++;

                if (i < len && (line->at(i) == '#' || line->at(i) == ';'))
                    continue;

                // 检查是否是节
                if (i < len && line->at(i) == '[') {
                    path.pop_back();
                    // 遍历到']'
                    i++;
                    Int j = i;
                    while (j < len && line->at(j) != ']') j++;

                    //j 此时应该正好停留到 ] 字符上
                    if (j >= len) {
                        throw INVALID_ARGUMENT_EXCEPTION("No ] found in" + ToString(*line.get()));
                    }

                    path.push_back(line->substr(i, j - i));
                } else {
                    // 处理key
                    String  key= ParseString(*line,i,len,'=');
                    String  value= ParseString(*line,i,len,0);
                    path.push_back(key);
                    // 设置值
                    values->setValue(path, value);
                    path.pop_back();
                }
            }
            return SP<ConfigValues>(values);
        }
    };
};