#include "TouchAdapter.h"

// 示例实现：无真输入，仅模拟空事件
std::vector<DeviceEvent> TouchAdapter::pollEvents() {
    std::vector<DeviceEvent> events;
    if (!enabled) return events;
    // TODO: 接入触摸框架（如 Android NDK 或 iOS Touch API）
    return events;
}