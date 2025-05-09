#include "GamepadAdapter.h"

// 示例实现：无真输入，仅模拟空事件
std::vector<DeviceEvent> GamepadAdapter::pollEvents() {
    std::vector<DeviceEvent> events;
    if (!enabled) return events;
    // TODO: 接入手柄SDK（如 XInput、SDL2）获取按钮/摇杆事件
    return events;
}