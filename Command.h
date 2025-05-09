#ifndef COMMAND_H
#define COMMAND_H

#include "ActionMap.h" // 用于GameAction
#include "DeviceEvent.h"
#include <iostream> // 用于std::cout示例命令
#include <memory>   // 用于std::shared_ptr
#include <string>
#include <vector>

// 前向声明，如果GameContext定义在别处
// class GameContext;

// 命令接口 (Command Pattern)
class ICommand {
public:
  virtual ~ICommand() = default;
  virtual void
      execute(/* GameContext& context */) = 0; // 游戏上下文可以按需传入
  virtual GameAction getAction() const = 0;
  virtual DeviceType getSourceDevice() const = 0;
  virtual int getSourceCode() const = 0;
  virtual float getSourceValue() const = 0;
};

// 具体命令示例：执行一个游戏动作
class GameActionCommand : public ICommand {
public:
  GameActionCommand(const GameAction &action, const DeviceEvent &triggerEvent)
      : action(action), triggerEvent(triggerEvent) {}

  void execute(/* GameContext& context */) override {
    // 实际游戏逻辑会在这里被调用
    // 例如：context.getPlayer()->performAction(action.name);
    if (triggerEvent.device == DeviceType::Touch &&
        (triggerEvent.type == EventType::TouchDown ||
         triggerEvent.type == EventType::TouchUp)) {
      std::cout << "执行动作: " << action.name
                << " (由设备触发: " << deviceTypeToString(triggerEvent.device)
                << ", 事件类型:" << eventTypeToString(triggerEvent.type) << ")"
                << std::endl;
    } else {
      std::cout << "执行动作: " << action.name
                << " (由设备触发: " << deviceTypeToString(triggerEvent.device)
                << ")" << std::endl;
    }
  }

  GameAction getAction() const override { return action; }

  DeviceType getSourceDevice() const override { return triggerEvent.device; }

  int getSourceCode() const override { return triggerEvent.code; }

  float getSourceValue() const override { return triggerEvent.value; }

  // 新增：获取原始事件
  const DeviceEvent &getEvent() const { return triggerEvent; }

private:
  GameAction action;
  DeviceEvent triggerEvent; // 记录触发此命令的原始事件
};

#endif // COMMAND_H