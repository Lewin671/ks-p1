#include "KeyboardAdapter.h"
#include <chrono>
#include <random>

// 示例实现：无真输入，仅模拟空事件
std::vector<DeviceEvent> KeyboardAdapter::pollEvents() {
    std::vector<DeviceEvent> events;
    if (!enabled) return events;
    
    static uint64_t lastEventTime = 0;
    static bool isMoving = false;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(500, 2000); // 随机间隔500-2000ms
    
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    
    // 模拟玩家操作模式：跳跃、移动、攻击
    if (currentTime - lastEventTime >= dis(gen)) {
        // 随机选择一个动作
        int action = dis(gen) % 4; // 0:Jump, 1:Attack, 2:MoveForward, 3:MoveBackward
        
        DeviceEvent event;
        event.device = DeviceType::Keyboard;
        event.timestamp = currentTime;
        
        switch (action) {
            case 0: // Jump
                event.code = 32;  // 空格键
                event.value = 1.0f;
                break;
            case 1: // Attack
                event.code = 74;  // J键
                event.value = 1.0f;
                break;
            case 2: // MoveForward
                event.code = 87;  // W键
                event.value = isMoving ? 0.0f : 1.0f;
                isMoving = !isMoving;
                break;
            case 3: // MoveBackward
                event.code = 83;  // S键
                event.value = isMoving ? 0.0f : 1.0f;
                isMoving = !isMoving;
                break;
        }
        
        events.push_back(event);
        lastEventTime = currentTime;
    }
    
    return events;
}