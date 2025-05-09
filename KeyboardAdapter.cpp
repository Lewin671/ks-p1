#include "KeyboardAdapter.h"
#include <chrono>

// 示例实现：无真输入，仅模拟空事件
std::vector<DeviceEvent> KeyboardAdapter::pollEvents() {
    std::vector<DeviceEvent> events;
    if (!enabled) return events;
    // TODO: 在这里接入操作系统键盘事件API，如 GLFW 或 WinAPI
    return events;
}