#include "global_def.h"
#include <iostream>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include<exception>



// 有限状态机类
template<typename S,typename E> // State和Event
class FiniteStateMachine {
public:
    // 构造函数，初始化为某个状态
    FiniteStateMachine(S initialState) : currentState_(initialState) {}

    // 添加状态转换规则：当前状态 + 事件 -> 新状态
    void addTransition(S fromState, E event, S toState) {
        transitions_[{fromState, event}] = toState;
    }

    // 处理事件并进行状态转换
    void (handleEventE event) {
        auto key = std::make_pair(currentState_, event);
        if (transitions_.find(key) != transitions_.end()) {
            currentState_ = transitions_[key];
        } else {
            throw std::runtime_error("Invalid transition!");
        }
    }

    // 获取当前状态
    S getcurrentState() const {
        return currentState_;
    }

private:
    S currentState_;  // 当前状态

    // 状态转换表，键是{状态, 事件}，值是目标状态
    std::map<std::pair<S, E>, S> transitions_;

    // 帮助函数，将状态转换为字符串
    std::string stateToString(TrafficLightState state) const {
        switch (state) {
            case TrafficLightState::RED: return "RED";
            case TrafficLightState::GREEN: return "GREEN";
            case TrafficLightState::YELLOW: return "YELLOW";
            default: return "UNKNOWN";
        }
    }
};