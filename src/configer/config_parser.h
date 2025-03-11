#pragma once
#include "src/global_def.h"


namespace wa{

    enum class ConfigFileType{
        AUTO,
        YAML,
        JSON,
        INI
    };

    template<class T>
    class ConfigVar{
    private:
        String name_;
        String originalText_;
        T value_;

    public:
        Bool fromString(const String& val){

        }

        String toString()const{

        }

        T value(){

        }
    };




    // Config类,存放真正的配置内容
    class Config{
    private:
        // 用于读取配置文件的类
        // 是一个基类
        class ConfigParser{

        };

        // 根据文件后缀推断文件类型
        ConfigFileType getFileTypeFromExtension(const String& filename) {
            // 找到文件名的最后一个"."的位置
            size_t dotPos = filename.find_last_of(".");
            if (dotPos == std::string::npos) {
                throw std::invalid_argument("Invalid filename: no extension found.");
            }

            // 获取文件扩展名
            String extension = filename.substr(dotPos + 1);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // 转小写

            // 根据扩展名返回对应的 ConfigFileType
            if (extension == "yaml" || extension == "yml") {
                return ConfigFileType::YAML;
            } else if (extension == "json") {
                return ConfigFileType::JSON;
            } else if (extension == "ini") {
                return ConfigFileType::INI;
            } else {
                return ConfigFileType::AUTO; // 如果无法识别，返回 AUTO
            }
        }

        void loadYaml(std::istream & inStream){
            String line;
            if(std::getline(inStream,line)){

            }
        }

        void loadJson(std::istream & inStream);

        void loadIni(std::istream & inStream){

        }


    public:
        void load(const String& filename,ConfigFileType type=ConfigFileType::AUTO){  // 载入文件
            // AUTO代表根据文件名自行推断
            if(type==ConfigFileType::AUTO){
                type= getFileTypeFromExtension(filename);
            }
            std::ifstream inputFile;
            inputFile.open(filename);
            switch (type) {
                case ConfigFileType::YAML:
                    loadYaml(inputFile);
                    break;
                case ConfigFileType::JSON:
                    break;
                case ConfigFileType::INI:
                    break;
            }

        }

    };


}
