#include "ActionMap.h"
#include "ConflictResolver.h"
#include "DeviceEvent.h"
#include "DeviceManager.h"
#include "InputProcessor.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
  // 1. 初始化核心组件
  ActionMap::instance().initialize("bindings.json");        // 动作映射
  DeviceManager &deviceManager = DeviceManager::instance(); // 设备管理器
  ConflictResolver conflictResolver;                        // 冲突解决器
  InputProcessor inputProcessor(ActionMap::instance());     // 输入处理器

  std::cout << "=== 输入控制模块测试 ===" << std::endl;

  // 2. 打印所有 binding
  std::cout << "\n--- 打印所有 binding ---" << std::endl;
  auto allBindings = ActionMap::instance().getAllBindings();
  for (const auto &[deviceCode, actions] : allBindings) {
    std::cout << "设备: " << DeviceEvent::deviceTypeToString(deviceCode.first)
              << ", 输入代码: " << deviceCode.second << " -> 动作: ";
    for (const auto &action : actions) {
      std::cout << action << " ";
    }
    std::cout << std::endl;
  }

  // 3. 演示运行时动态切换设备类型
  std::cout << "\n--- 测试：动态设备切换 ---" << std::endl;
  deviceManager.setActiveDevice(DeviceType::Keyboard);
  DeviceEvent kb_jump_event{DeviceType::Keyboard, 32}; // 空格键
  inputProcessor.processInput(kb_jump_event);

  deviceManager.setActiveDevice(DeviceType::Gamepad);
  DeviceEvent gp_jump_event{DeviceType::Gamepad, 0}; // 手柄按钮 0 (Jump)
  inputProcessor.processInput(gp_jump_event);

  deviceManager.setActiveDevice(DeviceType::Touch);
  DeviceEvent touch_swipe_event{DeviceType::Touch, 1}; // 触屏滑动 (Jump)
  inputProcessor.processInput(touch_swipe_event);

  // 4. 演示处理多端输入事件冲突
  std::cout << "\n--- 测试：输入冲突解决 ---" << std::endl;
  // 场景：触屏滑动时，手柄方向键输入应该被忽略
  // 首先，确保触屏是活跃的，或者冲突解决逻辑能处理非活跃设备的优先输入
  deviceManager.setActiveDevice(DeviceType::Touch); // 假设触屏优先
  std::cout << "模拟触屏滑动（主导）和手柄方向键（次要）同时输入:" << std::endl;

  DeviceEvent touch_slide_event{DeviceType::Touch, 1}; // 触屏滑动
  DeviceEvent gamepad_dpad_event{DeviceType::Gamepad,
                                 1001}; // 手柄方向键上 (假设)

  // 模拟输入处理器接收到两个事件，并通过冲突解决器处理
  // 这里的 InputProcessor::processInput 应该内部调用 ConflictResolver
  // 为了简化演示，我们直接调用 ConflictResolver (实际应用中 InputProcessor
  // 会封装这个逻辑)

  std::vector<DeviceEvent> concurrentEvents;
  concurrentEvents.push_back(touch_slide_event);  // 触屏事件先到
  concurrentEvents.push_back(gamepad_dpad_event); // 手柄事件后到

  std::cout << "解决触屏事件 (1) 和手柄事件 (1001) 的冲突:" << std::endl;
  // 先将事件转为命令
  std::vector<std::shared_ptr<ICommand>> cmds1;
  for (const auto &ev : concurrentEvents) {
    auto cmds = inputProcessor.generateCommandsForEvent(ev);
    cmds1.insert(cmds1.end(), cmds.begin(), cmds.end());
  }
  auto resolvedCmds = conflictResolver.resolve(cmds1);
  if (!resolvedCmds.empty()) {
    auto &resolvedEvent =
        static_cast<GameActionCommand *>(resolvedCmds[0].get())->getEvent();
    std::cout << "冲突解决器决定: 设备 "
              << DeviceEvent::deviceTypeToString(resolvedEvent.device) << ", 代码 "
              << resolvedEvent.code << std::endl;
    inputProcessor.processInput(resolvedEvent);
  }

  // 另一个冲突场景：键盘和手柄同时输入移动指令，但只有一个能生效
  // 假设当前活跃设备是键盘
  deviceManager.setActiveDevice(DeviceType::Keyboard);
  std::cout << "\n模拟键盘 'W' (活跃) 和手柄摇杆上 (非活跃) 用于向前移动:"
            << std::endl;
  DeviceEvent kb_move_event{DeviceType::Keyboard, 87};  // 'W' 键
  DeviceEvent gp_move_event{DeviceType::Gamepad, 1001}; // 手柄左摇杆上

  concurrentEvents.clear();
  concurrentEvents.push_back(kb_move_event);
  concurrentEvents.push_back(gp_move_event);

  std::vector<std::shared_ptr<ICommand>> cmds2;
  for (const auto &ev : concurrentEvents) {
    auto cmds = inputProcessor.generateCommandsForEvent(ev);
    cmds2.insert(cmds2.end(), cmds.begin(), cmds.end());
  }
  auto resolvedCmds2 = conflictResolver.resolve(cmds2);
  if (!resolvedCmds2.empty()) {
    auto &resolvedEvent2 =
        static_cast<GameActionCommand *>(resolvedCmds2[0].get())->getEvent();
    std::cout << "冲突解决器决定: 设备 "
              << DeviceEvent::deviceTypeToString(resolvedEvent2.device) << ", 代码 "
              << resolvedEvent2.code << std::endl;
    inputProcessor.processInput(resolvedEvent2);
  }

  // 5.
  // 新增设备对既有模块的修改（主要体现在配置文件和适配器模式，main.cpp中较难直接展示）
  // 这个主要通过设计模式（如策略模式、适配器模式）和配置文件来实现。
  // ActionMap 从 JSON 加载配置，新增设备类型时，主要修改 JSON 和可能增加新的
  // DeviceAdapter。 DeviceManager, InputProcessor, ConflictResolver
  // 的核心逻辑应尽量保持不变。
  std::cout << "\n--- 关于扩展性（添加新设备）的说明 ---" << std::endl;
  std::cout << "添加新设备（例如方向盘）主要涉及:" << std::endl;
  std::cout << "1. 在 'bindings.json' 中更新新设备类型及其映射。" << std::endl;
  std::cout << "2. 在 DeviceEvent.h 中添加新的 DeviceType 枚举值。"
            << std::endl;
  std::cout << "3. 如果设备特定逻辑复杂，可能需要实现新的 'IDeviceAdapter'。"
            << std::endl;
  std::cout << "4. 如果设计通用，核心类如 InputProcessor 或 ConflictResolver "
               "的修改应尽量少。"
            << std::endl;

  std::cout << "\n=== 测试完成 ===" << std::endl;

  return 0;
}