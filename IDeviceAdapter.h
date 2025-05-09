#ifndef IDEVICE_ADAPTER_H
#define IDEVICE_ADAPTER_H

#include "DeviceEvent.h"
#include <vector>

// 设备适配器接口：将原生事件转换为 DeviceEvent
class IDeviceAdapter {
public:
    virtual ~IDeviceAdapter() = default;
    // 拉取本帧所有事件
    virtual std::vector<DeviceEvent> pollEvents() = 0;
    // 启用／禁用该适配器
    virtual void enable(bool on) { enabled = on; }
    virtual bool isEnabled() const { return enabled; }

protected:
    bool enabled = true;
};

#endif // IDEVICE_ADAPTER_H