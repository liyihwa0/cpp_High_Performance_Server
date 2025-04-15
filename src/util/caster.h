#pragma once

#include "src/global_def.h"
#include "../exception/exception.h"
namespace wa{
    template<typename T>
    T CastAsType(const String& value) {
        // 默认实现，抛出异常
        throw INVALID_ARGUMENT_EXCEPTION("Cannot cast string to " + String(typeid(T).name()));
    }
    
    template<typename T>
    String ToString(const T& value){
        throw INVALID_ARGUMENT_EXCEPTION("Cannot cast value to string");
    }
    
    // 特化模板，用于基本类型
    template<>
    Char CastAsType<Char>(const String& value) {
        if (value.length() != 1) {
            throw INVALID_ARGUMENT_EXCEPTION("Cannot cast string to Char: string length is not 1");
        }
        return value[0];
    }

    template<>
    I8 CastAsType<I8>(const String& value) {
        return static_cast<I8>(std::stoi(value));
    }

    template<>
    U8 CastAsType<U8>(const String& value) {
        return static_cast<U8>(std::stoul(value));
    }

    template<>
    I16 CastAsType<I16>(const String& value) {
        return std::stoi(value);
    }

    template<>
    U16 CastAsType<U16>(const String& value) {
        return static_cast<U16>(std::stoul(value));
    }

    template<>
    I32 CastAsType<I32>(const String& value) {
        return std::stoi(value);
    }

    template<>
    U32 CastAsType<U32>(const String& value) {
        return static_cast<U32>(std::stoul(value));
    }

    template<>
    I64 CastAsType<I64>(const String& value) {
        return std::stoll(value);
    }

    template<>
    U64 CastAsType<U64>(const String& value) {
        return std::stoull(value);
    }

    template<>
    F32 CastAsType<F32>(const String& value) {
        return std::stof(value);
    }

    template<>
    F64 CastAsType<F64>(const String& value) {
        return std::stod(value);
    }

    template<>
    bool CastAsType<bool>(const String& value) {
        if (value == "true" || value == "1") {
            return true;
        } else if (value == "false" || value == "0") {
            return false;
        } else {
            throw INVALID_ARGUMENT_EXCEPTION("Cannot cast string to bool: " + value);
        }
    }

    template<>
    String CastAsType<String>(const String& value) {
        return value;
    }

    // 特化 ToString 函数模板以支持基本类型
    template<>
    String ToString<int>(const int& value) {
        return std::to_string(value);
    }

    template<>
    String ToString<double>(const double& value) {
        return std::to_string(value);
    }

    template<>
    String ToString<float>(const float& value) {
        return std::to_string(value);
    }

    template<>
    String ToString<char>(const char& value) {
        return String(1, value); // 将 char 转换为 String
    }

    template<>
    String ToString<bool>(const bool& value) {
        return value ? "true" : "false"; // 将 bool 转换为 "true" 或 "false"
    }


}
