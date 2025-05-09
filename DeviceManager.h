#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "IDeviceAdapter.h"
#include "DeviceEvent.h" // 为 DeviceType 添加
#include <memory>
#include <vector>
#include <mutex>
#include <algorithm> // 为 std::remove 添加

class DeviceManager {
public:
    // 单例获取
    static DeviceManager& instance();

    // 注册／移除适配器
    void registerAdapter(std::shared_ptr<IDeviceAdapter> adapter);
    void unregisterAdapter(std::shared_ptr<IDeviceAdapter> adapter);

    // 拉取所有适配器事件
    std::vector<DeviceEvent> pollEvents();

    // 启用／禁用某种类型设备（循环所有适配器判断类型）
    void enableDevice(DeviceType type, bool on);

    // 新增：设置和获取当前活跃设备
    void setActiveDevice(DeviceType type);
    DeviceType getActiveDevice() const;

private:
    DeviceManager() = default;
    std::vector<std::shared_ptr<IDeviceAdapter>> adapters;
    std::mutex mtx;
    DeviceType activeDevice = DeviceType::Keyboard; // 默认活跃设备
};

#endif // DEVICE_MANAGER_H