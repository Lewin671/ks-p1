#ifndef DEVICE_EVENT_H
#define DEVICE_EVENT_H

#include <cstdint>
#include <string>

// 支持的设备类型
enum class DeviceType {
    Keyboard,
    Touch
};

// 事件类型
enum class EventType {
    Button,
    Directional,
    TouchDown,
    TouchUp
};

// 将设备类型转换为字符串
inline std::string deviceTypeToString(DeviceType type) {
    switch (type) {
        case DeviceType::Keyboard:
            return "键盘";
        case DeviceType::Touch:
            return "触屏";
        default:
            return "未知";
    }
}

// 将事件类型转换为字符串
inline std::string eventTypeToString(EventType type) {
    switch (type) {
        case EventType::Button:
            return "Button";
        case EventType::Directional:
            return "Directional";
        case EventType::TouchDown:
            return "TouchDown";
        case EventType::TouchUp:
            return "TouchUp";
        default:
            return "Unknown";
    }
}

// 统一的底层事件对象
struct DeviceEvent {
    DeviceType device;  // 事件来源设备
    EventType type;     // 事件类型
    int code;           // 键码或按钮编号
    float value;        // 数值（如压力、轴值）
    uint64_t timestamp; // 时间戳
};

#endif // DEVICE_EVENT_H