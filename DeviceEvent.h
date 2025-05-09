#ifndef DEVICE_EVENT_H
#define DEVICE_EVENT_H

#include <cstdint>
#include <string> // 新增：包含 string

// 支持的设备类型
enum class DeviceType {
    Keyboard,
    Gamepad,
    Touch,
};

// 新增：将 DeviceType 转换为字符串的辅助函数
inline std::string deviceTypeToString(DeviceType type) {
    switch (type) {
        case DeviceType::Keyboard:
            return "Keyboard";
        case DeviceType::Gamepad:
            return "Gamepad";
        case DeviceType::Touch:
            return "Touch";
        default:
            return "Unknown Device";
    }
}

// 统一的底层事件对象
struct DeviceEvent {
    DeviceType device; // 事件来源设备
    int code;          // 键码或按钮编号／触摸类型
    float value;       // 数值（如压力、轴值，或触摸坐标可拆为两个事件）
    uint64_t timestamp;// 时间戳
};

#endif // DEVICE_EVENT_H