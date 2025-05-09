#include "ActionMap.h"
#include "ConflictResolver.h"
#include "DeviceEvent.h"
#include "DeviceManager.h"
#include "InputProcessor.h"
#include "KeyboardAdapter.h"
#include "GamepadAdapter.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

// 辅助函数：获取事件对应的操作名称
std::string getEventActions(const DeviceEvent& event) {
    auto actions = ActionMap::instance().getActions(event);
    if (actions.empty()) {
        return "未知操作";
    }
    
    std::string result;
    for (size_t i = 0; i < actions.size(); ++i) {
        if (i > 0) result += ", ";
        result += actions[i].name;
    }
    return result;
}

// 辅助函数：检查事件是否被冲突解决策略过滤
bool isEventFiltered(const DeviceEvent& event, const ConflictResolver& resolver) {
    return !resolver.shouldProcessInput(event);
}

void handleEvents(const std::vector<DeviceEvent>& events,InputProcessor& inputProcessor){
      // 处理每个事件
    for (const auto& event : events) {
      bool isFiltered = isEventFiltered(event, inputProcessor.getConflictResolver());
      std::cout << "收到事件: " << deviceTypeToString(event.device) 
                << " -> " << getEventActions(event)
                << (isFiltered ? " [被冲突策略过滤]" : "") << std::endl;
      
      if (!isFiltered) {
        inputProcessor.processInput(event);
      }
    }
}

std::vector<DeviceEvent> getMockedEvents(){
    std::vector<DeviceEvent> events;
    uint64_t baseTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // 手柄触摸按下
    DeviceEvent touchDown;
    touchDown.device = DeviceType::Touch;
    touchDown.type = EventType::TouchDown;
    touchDown.code = 2001;
    touchDown.value = 1.0f;
    touchDown.timestamp = baseTime + 100;
    events.push_back(touchDown);

    // 手柄按下上方向键（应该被过滤）
    DeviceEvent gamepadUp;
    gamepadUp.device = DeviceType::Keyboard;
    gamepadUp.type = EventType::Directional;
    gamepadUp.code = 32;
    gamepadUp.value = 1.0f;
    gamepadUp.timestamp = baseTime + 200;
    events.push_back(gamepadUp);

    // 手柄触摸抬起
    DeviceEvent touchUp;
    touchUp.device = DeviceType::Touch;
    touchUp.type = EventType::TouchUp;
    touchUp.code = 2002;

    touchUp.value = 0.0f;
    touchUp.timestamp = baseTime + 300;
    events.push_back(touchUp);

    // 手柄再次按下上方向键（应该被处理）

    events.push_back(gamepadUp);

    return events;
}

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
  inputProcessor.getConflictResolver().addStrategy(
    std::make_shared<TouchVsDirectionalStrategy>()
  );
  
  auto mockedEvents = getMockedEvents();
  handleEvents(mockedEvents, inputProcessor);

  // 4. 注册设备适配器
  std::cout << "\n--- 注册设备适配器 ---" << std::endl;
  deviceManager.registerAdapter(std::make_shared<KeyboardAdapter>());
  deviceManager.registerAdapter(std::make_shared<GamepadAdapter>());

  // 5. 主循环：处理设备事件
  std::cout << "\n--- 开始处理设备事件 ---" << std::endl;
  std::cout << "按 Ctrl+C 退出" << std::endl;

  // 模拟设备切换
  std::vector<DeviceType> devices = {
    DeviceType::Keyboard,
    DeviceType::Touch
  };
  size_t currentDeviceIndex = 0;
  uint64_t lastSwitchTime = 0;

  while (true) {
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
    ).count();
    
    // 每10秒切换一次设备
    if (currentTime - lastSwitchTime >= 10000) {
      // 禁用当前设备
      deviceManager.enableDevice(devices[currentDeviceIndex], false);
      
      // 切换到下一个设备
      currentDeviceIndex = (currentDeviceIndex + 1) % devices.size();
      deviceManager.enableDevice(devices[currentDeviceIndex], true);
      
      std::cout << "\n=== 切换到设备: " 
                << deviceTypeToString(devices[currentDeviceIndex]) 
                << " ===" << std::endl;
      
      lastSwitchTime = currentTime;
    }

    // 获取所有设备的事件
    auto events = deviceManager.pollEvents();
    handleEvents(events,inputProcessor);

  
    // 控制帧率
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  return 0;
}