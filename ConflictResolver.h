#ifndef CONFLICT_RESOLVER_H
#define CONFLICT_RESOLVER_H

#include "Command.h"
#include "DeviceEvent.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream> // For example strategy

// 冲突解决策略接口 (Strategy Pattern)
class IConflictResolutionStrategy {
public:
    virtual ~IConflictResolutionStrategy() = default;
    // 判断是否应该处理输入事件
    virtual bool shouldProcessInput(const DeviceEvent& event) = 0;
    virtual std::string getName() const = 0;
};

// 示例策略：最后输入优先
class LastInputWinsStrategy : public IConflictResolutionStrategy {
public:
    bool shouldProcessInput(const DeviceEvent& event) override {
        // 这里可以添加时间戳比较逻辑
        return true; // 默认允许处理
    }
    std::string getName() const override { return "LastInputWins"; }
};

// 示例策略：特定设备优先
class DevicePriorityStrategy : public IConflictResolutionStrategy {
public:
    DevicePriorityStrategy(DeviceType preferred, DeviceType lessPreferred)
        : preferredDevice(preferred), lessPreferredDevice(lessPreferred) {}

    bool shouldProcessInput(const DeviceEvent& event) override {
        // 如果事件来自优先设备，允许处理
        if (event.device == preferredDevice) {
            return true;
        }
        // 如果事件来自次优先设备，且当前没有优先设备的事件，允许处理
        if (event.device == lessPreferredDevice) {
            // 这里可以添加检查是否有优先设备事件的逻辑
            return true;
        }
        return false;
    }
    std::string getName() const override { return "DevicePriority"; }

private:
    DeviceType preferredDevice;
    DeviceType lessPreferredDevice;
};

// 新增：触屏滑动时禁用手柄方向键策略
class TouchSlideGamepadFilterStrategy : public IConflictResolutionStrategy {
public:
    bool shouldProcessInput(const DeviceEvent& event) override {
        // 处理触屏事件
        if (event.device == DeviceType::Touch) {
            switch (event.code) {
                case 1: // 触屏滑动开始
                    isTouchSliding = true;
                    return true;
                case 0: // 触屏抬起
                    isTouchSliding = false;
                    return true;
                default:
                    return true;
            }
        }
        
        // 处理手柄方向键事件
        if (event.device == DeviceType::Gamepad && 
            event.code >= 1000 && event.code <= 1003) {
            // 如果正在触屏滑动，则拒绝处理手柄方向键输入
            return !isTouchSliding;
        }
        
        // 其他事件默认允许处理
        return true;
    }
    
    std::string getName() const override { return "TouchSlideGamepadFilter"; }

    // 新增：获取当前状态的方法，用于调试
    bool isCurrentlyTouchSliding() const { return isTouchSliding; }

private:
    bool isTouchSliding = false;
};

// 冲突解析器
class ConflictResolver {
public:
    void addStrategy(std::shared_ptr<IConflictResolutionStrategy> strategy) {
        if (strategy) {
            strategies.push_back(strategy);
        }
    }
    
    // 判断是否应该处理输入事件
    bool shouldProcessInput(const DeviceEvent& event) {
        // 应用所有策略，如果任何一个策略返回false，则事件不应该被处理
        for (const auto& strategy : strategies) {
            if (strategy && !strategy->shouldProcessInput(event)) {
                return false;
            }
        }
        return true;
    }

private:
    std::vector<std::shared_ptr<IConflictResolutionStrategy>> strategies;
};

#endif // CONFLICT_RESOLVER_H