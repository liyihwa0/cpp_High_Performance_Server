#pragma once

#include <regex>
#include "../global_def.h"
#include "../smart_ptr.h"
#include "../data_structure/multi_tree.h"
#include "../util/helper.h"
#include "../util/caster.h"
#include "src/locker/locker.h"

namespace wa{
    namespace config{
        enum ConfigFileType{
                AUTO,
                INI,
                JSON
            };
        
            // ConfigVar定义了配置文件的变量规范
            // DefinitionItem:
            // String defaultValue;
            // Bool couldEmpty;
            // String name;
            // Char shortOpt;
            /*
             * couldEmpty为false时,必须有对应配置值
             * couldEmpty为true时,允许配置值为空,并且在配置值为空时采用defaultValue
             */
            struct ConfigDefinitionItem{
                String name_;
                Char shortOpt_;
                Bool couldEmpty_;
                String defaultValue_;
            };
        
            class Config;
        
            class ConfigDefinitions{
                friend Config;
                MultiTree<String,SP<ConfigDefinitionItem>> definitions_;
                SP<ConfigDefinitionItem> shortOptDefinitions_[52];
            public:
        
                ConfigDefinitions(): definitions_(){}
        
                void setValue(const Vector<String>& path, SP<ConfigDefinitionItem> value){
                    if(value->shortOpt_!='\0'){
                        if(value->shortOpt_>='a'&&value->shortOpt_<='z'){
                            shortOptDefinitions_[value->shortOpt_-'a']=value;
                        }else if(value->shortOpt_>='A'&&value->shortOpt_<='Z'){
                            shortOptDefinitions_[value->shortOpt_-'A'+26]=value;
                        }else{
                            throw CODE_LOCATION_EXCEPTION("invalid argument: "+value->shortOpt_);
                        }
                    }
                    definitions_.setValue(path, std::move(value));
                }
        
                void merge(const Vector<String>& path,ConfigDefinitions& subDefinition){
                    for(Int i=0;i<52;i++){
                        if(subDefinition.shortOptDefinitions_[i].get()){
                            shortOptDefinitions_[i]=subDefinition.shortOptDefinitions_[i];
                        }
                    }
                    definitions_.merge(path,subDefinition.definitions_);
                }
            };
        
            class ConfigValues{
                friend Config;
                MultiTree<String,String> values_;
               
                void merge(const Vector<String>& path,ConfigValues& value){
                    values_.merge(path,value.values_);
                }
            public:
                ConfigValues(): values_(){} 
                void setValue(const Vector<String>& path,const String& value){
                     values_.setValue(path, value);
                }
                String getString(const Vector<String>& path,const String& defaultValue)const{
                    return values_.getValue(path,defaultValue);
                }

                Bool hasValue(const Vector<String>& path) const{
                    return values_.hasLeaf(path);
                }
        
                template<class T>
                T getValue(const Vector<String>& path,const String& defaultValue)const{
                    String value=values_.getValue(path,defaultValue);
                    return CastAsType<T>(value);
                }
            };
        
            class Config{
                ConfigValues values_;
                ConfigDefinitions definitions_;
                SP<ConfigValues> parsedValues_;
                ReadWriteLock lock_;
            public:
                Config()= default;

                void mergeValues(const Vector<String>& path,ConfigValues& values){
                    ScopedWriteLock l(lock_);
                    values_.merge(path,values);
                }

                void mergeDefinitions(const Vector<String>& path,ConfigDefinitions& definitions){
                     ScopedWriteLock l(lock_);
                    definitions_.merge(path,definitions);
                }
        
                void parseCommand(){
                    auto l=AsScopedLock(lock_);
                    //todo
                }
        
                // 实现ini读取
                void parseFile(String filename,ConfigFileType fileType){
                    ScopedWriteLock l(lock_);
                    //todo
                }
        
                SP<ConfigValues> parsedValues() {
                    lock_.lockRead();
                    if (parsedValues_.get()) {
                        SP<ConfigValues> res=parsedValues_;
                        lock_.unlock();
                        return res;
                    }

                    ScopedWriteLock l(lock_);
                    // 创建一个新的 ConfigValues 对象
                    parsedValues_.reset(new ConfigValues());
                    // 遍历definitions_,找到不存在于values_中的,而存在于definitions_的defaultValue中的变量,将其添加到parsedValues设置为defaultValue
                    // 遍历 definitions_ 中的所有路径
                    definitions_.definitions_.forEach([this](const Vector<String>& path, const MultiTree<String,SP<ConfigDefinitionItem>>& items) {
                        SP<ConfigDefinitionItem> item=items.getValue();
        
                        if (!values_.hasValue(path)){
                            if (!item->couldEmpty_){
                                throw INVALID_ARGUMENT_EXCEPTION(Helper::Join(path,".")+" can not be null");
                            }
                            values_.setValue(path,item->defaultValue_);
                        }
                    });
        
                    //合并 parsedValues_与values_
                    parsedValues_->merge({},values_);
        
                    return parsedValues_;
                }
            };
    }
    
}
