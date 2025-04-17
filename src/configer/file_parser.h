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
    SP<ConfigValues> ParseIniFile(const String& filename){
        Vector<String> path;
        path.push_back("");
        ConfigValues* values=new ConfigValues();
        FileReader fileReader(filename);

        while(fileReader.hasMore()){
            UP<String> line=fileReader.nextLine();
            
            Int i=0;
            while(i< line->length()){
                // 跳过前后的空白
                Int len= line->length()-1;
                while(len>=0&&Helper::IsSpaceChar(line->at(len))){
                    len--;
                }
                while(i<len &&Helper::IsSpaceChar(line->at(i))) i++;
                
                // 检查是否是节
                if(i<len && line->at(i)=='['){
                    path.pop_back();
                    // 遍历到']'
                    String currentSection="";
                    i++;
                    while(i<len&&line->at(i)!=']'){
                        currentSection.push_back(line->at(i));
                        i++;
                    }
                    i++;
                    if(i<len){
                        throw INVALID_ARGUMENT_EXCEPTION("Unknow char "+ ToString(line->at(i)));
                    }
                    path.push_back(currentSection);
                }else if(i<len&&(line->at(i)=='#'||line->at(i)==';')){
                    break;
                }else{
                    // 处理键值对
                    Int delimiterPos=line->find('=',i);
                    if (delimiterPos != String::npos) {
                        String key=line->substr(i,delimiterPos-i);
                        String value=line->substr(delimiterPos,len-delimiterPos);

                        path.push_back(key);
                        // 设置值
                        values->setValue(path, value);
                        path.pop_back();
                    }
                }
            }
        }
        
        return SP<ConfigValues>(values);
    }
};