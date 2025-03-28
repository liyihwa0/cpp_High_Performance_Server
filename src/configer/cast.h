#pragma once

#include "../global_def.h"
namespace wa{
    template<typename T>
       T CastAsType(const String& value) {
        // 默认实现，抛出异常
        throw std::invalid_argument("Cannot cast string to " + String(typeid(T).name()));
    }

    // 特化模板，用于基本类型
    template<>
    Char CastAsType<Char>(const String& value) {
        if (value.length() != 1) {
            throw std::invalid_argument("Cannot cast string to Char: string length is not 1");
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
            throw std::invalid_argument("Cannot cast string to bool: " + value);
        }
    }

    template<>
    String CastAsType<String>(const String& value) {
        return value;
    }
}
