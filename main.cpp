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
  InputProcessor inputProcessor(ActionMap::instance());     // 输入处理器

  std::cout << "=== 输入控制模块测试 ===" << std::endl;

  // 2. 打印所有 binding
  std::cout << "\n--- 打印所有 binding ---" << std::endl;
  auto allBindings = ActionMap::instance().getAllBindings();
  for (const auto &[deviceCode, actions] : allBindings) {
    std::cout << "设备: " << deviceTypeToString(deviceCode.first)
              << ", 输入代码: " << deviceCode.second << " -> 动作: ";
    for (const auto &action : actions) {
      std::cout << action << " ";
    }
    std::cout << std::endl;
  }

  // 3. 添加冲突解决策略
  std::cout << "\n--- 添加冲突解决策略 ---" << std::endl;
  
  // 添加触屏滑动时禁用手柄方向键策略
  std::cout << "添加策略: TouchSlideGamepadFilterStrategy" << std::endl;
  inputProcessor.addConflictStrategy(std::make_shared<TouchSlideGamepadFilterStrategy>());

  // 立即测试 TouchSlideGamepadFilterStrategy
  std::cout << "\n--- 测试 TouchSlideGamepadFilterStrategy ---" << std::endl;
  
  // 测试场景1：触屏滑动时的手柄方向键输入
  std::cout << "测试场景1：触屏滑动时的手柄方向键输入" << std::endl;
  DeviceEvent touch_slide_test{DeviceType::Touch, 1};    // 触屏滑动
  DeviceEvent gamepad_up_test{DeviceType::Gamepad, 1001}; // 手柄上方向键 (MoveForward)
  DeviceEvent gamepad_down_test{DeviceType::Gamepad, 1002}; // 手柄下方向键 (MoveBackward)
  
  std::cout << "1.1 先触发触屏滑动（应该执行 Jump 动作）:" << std::endl;
  inputProcessor.processInput(touch_slide_test);
  
  std::cout << "1.2 触屏滑动时，尝试手柄上方向键（应该被忽略，不执行 MoveForward）:" << std::endl;
  inputProcessor.processInput(gamepad_up_test);
  
  std::cout << "1.3 触屏滑动时，尝试手柄下方向键（应该被忽略，不执行 MoveBackward）:" << std::endl;
  inputProcessor.processInput(gamepad_down_test);
  
  // 测试场景2：触屏抬起后的手柄方向键输入
  std::cout << "\n测试场景2：触屏抬起后的手柄方向键输入" << std::endl;
  DeviceEvent touch_release_test{DeviceType::Touch, 0};  // 触屏抬起
  
  std::cout << "2.1 触发触屏抬起（重置滑动状态）:" << std::endl;
  inputProcessor.processInput(touch_release_test);
  
  std::cout << "2.2 触屏抬起后，尝试手柄上方向键（应该执行 MoveForward）:" << std::endl;
  inputProcessor.processInput(gamepad_up_test);
  
  // 测试场景3：触屏滑动时的其他手柄输入
  std::cout << "\n测试场景3：触屏滑动时的其他手柄输入" << std::endl;
  DeviceEvent touch_slide_test2{DeviceType::Touch, 1};   // 触屏滑动
  DeviceEvent gamepad_button_test{DeviceType::Gamepad, 0}; // 手柄按钮（非方向键，Jump）
  
  std::cout << "3.1 再次触发触屏滑动（应该执行 Jump）:" << std::endl;
  inputProcessor.processInput(touch_slide_test2);
  
  std::cout << "3.2 触屏滑动时，尝试手柄按钮（应该执行 Jump，因为不是方向键）:" << std::endl;
  inputProcessor.processInput(gamepad_button_test);

  // 4. 演示运行时动态切换设备类型
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

  std::cout << "\n=== 测试完成 ===" << std::endl;

  return 0;
}