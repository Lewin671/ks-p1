#include "GamepadAdapter.h"
#include <chrono>
#include <random>

// 示例实现：无真输入，仅模拟空事件
std::vector<DeviceEvent> GamepadAdapter::pollEvents() {
    std::vector<DeviceEvent> events;
    if (!enabled) return events;
    
    static uint64_t lastEventTime = 0;
    static bool isMoving = false;
    static bool isTouching = false;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(800, 2500); // 随机间隔800-2500ms
    
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    
    // 模拟玩家操作模式：跳跃、攻击、移动、触摸
    if (currentTime - lastEventTime >= dis(gen)) {
        // 随机选择一个动作
        int action = dis(gen) % 6; // 0:Jump, 1:Attack, 2:MoveForward, 3:MoveBackward, 4:TouchDown, 5:TouchUp
        
        DeviceEvent event;
        event.device = DeviceType::Touch;
        event.timestamp = currentTime;
        
        switch (action) {
            case 0: // Jump
                event.type = EventType::Button;
                event.code = 0;   // A按钮
                event.value = 1.0f;
                break;
            case 1: // Attack
                event.type = EventType::Button;
                event.code = 1;   // B按钮
                event.value = 1.0f;
                break;
            case 2: // MoveForward
                event.type = EventType::Directional;
                event.code = 1001; // 上方向键
                event.value = isMoving ? 0.0f : 1.0f;
                isMoving = !isMoving;
                break;
            case 3: // MoveBackward
                event.type = EventType::Directional;
                event.code = 1002; // 下方向键
                event.value = isMoving ? 0.0f : 1.0f;
                isMoving = !isMoving;
                break;
            case 4: // TouchDown
                event.type = EventType::TouchDown;
                event.code = 2001;
                event.value = 1.0f;
                isTouching = true;
                break;
            case 5: // TouchUp
                event.type = EventType::TouchUp;
                event.code = 2002;
                event.value = 0.0f;
                isTouching = false;
                break;
        }
        
        events.push_back(event);
        lastEventTime = currentTime;
    }
    
    return events;
}