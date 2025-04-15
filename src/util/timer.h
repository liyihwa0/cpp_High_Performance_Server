#pragma once

#include "src/global_def.h"
#include <iomanip>

namespace wa{
    class Timer{
    private:
        Int ms_;
    public:
        // 定义时间单位的枚举
        enum Unit {
            MS = 1,
            S  = 1000*MS,
            MIN  = 60 * S,
            HOUR  = 60 * MIN,
            DAY  = 24 *HOUR,
        };
        Timer()=default;
        explicit Timer(Int ms):ms_(ms){}

        Timer(Timer const &other)noexcept{
            ms_=other.ms_;
        }

        static Timer Now() noexcept{
            using namespace std::chrono;
            // 获取当前时间点
            auto now = system_clock::now();
            // 将当前时间点转换为自 UNIX 纪元以来的毫秒数
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
            return Timer(ms);  // 返回以毫秒为单位的 Timer 对象
        }


        // 运算符重载：加法运算符
        Timer operator+(const Timer& other) const noexcept {
            return Timer{ms_ + other.ms_};
        }

        // 运算符重载：减法运算符
        Timer operator-(const Timer& other) const  noexcept{
            return Timer{ms_ - other.ms_};
        }

        Int getMS()const noexcept{
            return ms_;
        }

        // 运算符重载：== 比较运算符
        bool operator==(const Timer& other) const noexcept{
            return ms_ == other.ms_;
        }

        // 运算符重载：!= 比较运算符
        bool operator!=(const Timer& other) const noexcept{
            return ms_ != other.ms_;
        }

        // 运算符重载：< 比较运算符
        bool operator<(const Timer& other) const noexcept{
            return ms_ < other.ms_;
        }

        // 运算符重载：<= 比较运算符
        bool operator<=(const Timer& other) const noexcept{
            return ms_ <= other.ms_;
        }

        // 运算符重载：> 比较运算符
        bool operator>(const Timer& other) const noexcept{
            return ms_ > other.ms_;
        }

        // 运算符重载：>= 比较运算符
        bool operator>=(const Timer& other) const noexcept{
            return ms_ >= other.ms_;
        }

        String format(String const & fmt="%Y-%m-%d %H:%M:%S")const{
            using namespace std::chrono;

            // 将毫秒数转为时间点
            auto tp = system_clock::from_time_t(0) + milliseconds(ms_);

            // 将时间点转为时间
            auto tt = system_clock::to_time_t(tp);
            std::ostringstream oss;
            oss<<std::put_time(std::localtime(&tt), fmt.c_str());
            return oss.str();
        }
    };
};
