#pragma once

#include <regex>
#include "../global_def.h"
#include "../smart_ptr.h"
#include "../data_structure/multi_tree.h"
#include "../helper.h"
namespace wa{
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
        SP<MultiTree<String,SP<ConfigDefinitionItem>>> definitions_;
        SP<ConfigDefinitionItem> shortOptDefinitions_[52];
    public:

        ConfigDefinitions(): definitions_(new MultiTree<String,SP<ConfigDefinitionItem>>()){}

        void addDefinition(const Vector<String>& path,SP<ConfigDefinitionItem> value){
            if(value->shortOpt_!='\0'){
                if(value->shortOpt_>='a'&&value->shortOpt_<='z'){
                    shortOptDefinitions_[value->shortOpt_-'a']=value;
                }else if(value->shortOpt_>='A'&&value->shortOpt_<='Z'){
                    shortOptDefinitions_[value->shortOpt_-'A'+26]=value;
                }else{
                    throw CODE_LOCATION_EXCEPTION("invalid argument: "+value->shortOpt_);
                }
            }
            definitions_->setValue(path,std::move(value));
        }

        void mergeDefinitions(const Vector<String>& path,const ConfigDefinitions& subDefinition){
            for(Int i=0;i<52;i++){
                if(subDefinition.shortOptDefinitions_[i].get()){
                    shortOptDefinitions_[i]=subDefinition.shortOptDefinitions_[i];
                }
            }
            definitions_->merge(path,*subDefinition.definitions_);
        }
    };

    class ConfigValues{
        friend Config;
        SP<MultiTree<String,String>> values_;
        void addValue(const Vector<String>& path,const String& value){
            values_->setValue(path, value);
        }
        void mergeValues(const Vector<String>& path,const ConfigValues& value){
            values_->merge(path,*value.values_);
        }
    public:
        ConfigValues(): values_(new MultiTree<String,String>()){}
        String getString(const Vector<String>& path,const String& defaultValue)const{
            return values_->getValue(path,defaultValue);
        }
        Bool hasValue(const Vector<String>& path){
            return values_->isLeaf(path);
        }

        template<class T>
        T getValue(const Vector<String>& path,const String& defaultValue)const{
            //todo
        }
    };

    class Config{
        ConfigValues values_;
        ConfigDefinitions definitions_;
        SP<ConfigValues> parsedValues_;
    public:
        Config()= default;
        void mergeConfig(const Vector<String>& path,const Config& other){
            definitions_.mergeDefinitions(path,other.definitions_);
            values_.mergeValues(path,other.values_);
        }
        void parseCommand(){

        }
        void parseFile(){

        }

        SP<ConfigValues> parsedValues() {
            if (parsedValues_.get()) {
                return parsedValues_;
            }

            // 创建一个新的 ConfigValues 对象
            parsedValues_.reset(new ConfigValues());

            // 遍历 definitions_ 中的所有路径
            auto definitions=definitions_.definitions_->getChild({});

            definitions.forEach([this](const Vector<String>& path, const SP<ConfigDefinitionItem>& item) {
                // 检查 values_ 中是否存在该路径的值
                if (values_.hasValue(path)) {
                    // 如果存在值，直接添加到 parsedValues_
                    parsedValues_->addValue(path, values_.getString(path, ""));
                } else {
                    // 如果不存在值，检查 couldEmpty_
                    if (!item->couldEmpty_) {
                        throw CODE_LOCATION_EXCEPTION("Missing required value for path: " + Helper::Join(path, "."));
                    }
                    // 如果 couldEmpty_ 为 true,使用 defaultValue_
                    parsedValues_->addValue(path, item->defaultValue_);
                }
            });

            // 遍历defaultValue,找到不存在于parsedValues中的,而存在于defaultValue中的变量
            return parsedValues_;
        }


    };
}
