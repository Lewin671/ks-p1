#ifndef CONFLICT_RESOLVER_H
#define CONFLICT_RESOLVER_H

#include "Command.h"
#include "DeviceEvent.h"
#include <iostream> // For example strategy
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// 冲突解决策略接口 (Strategy Pattern)
class IConflictResolutionStrategy {
public:
  virtual ~IConflictResolutionStrategy() = default;
  // 判断是否应该处理输入事件
  virtual bool shouldProcessInput(const DeviceEvent &event) const = 0;
  virtual std::string getName() const = 0;
};

// 示例策略：最后输入优先
class LastInputWinsStrategy : public IConflictResolutionStrategy {
public:
  bool shouldProcessInput(const DeviceEvent &event) const override {
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

  bool shouldProcessInput(const DeviceEvent &event) const override {
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

// 触摸与方向键冲突解决策略
class TouchVsDirectionalStrategy : public IConflictResolutionStrategy {
public:
  TouchVsDirectionalStrategy() : isTouching(false) {}

  bool shouldProcessInput(const DeviceEvent &event) const override {
    // 如果是触摸事件，更新触摸状态
    if (event.device == DeviceType::Touch &&
        (event.type == EventType::TouchDown ||
         event.type == EventType::TouchUp)) {
      isTouching = (event.type == EventType::TouchDown);
      return true; // 允许处理触摸事件
    }
    if (isTouching) {
      return event.device == DeviceType::Touch;
    }
    return true;
  }

  std::string getName() const override { return "TouchVsDirectional"; }

private:
  mutable bool isTouching; // 标记是否正在触摸
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
  bool shouldProcessInput(const DeviceEvent &event) const;

private:
  std::vector<std::shared_ptr<IConflictResolutionStrategy>> strategies;
};

#endif // CONFLICT_RESOLVER_H