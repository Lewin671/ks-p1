#ifndef GAMEPAD_ADAPTER_H
#define GAMEPAD_ADAPTER_H

#include "IDeviceAdapter.h"

// 手柄适配器（示例）
class GamepadAdapter : public IDeviceAdapter {
public:
    std::vector<DeviceEvent> pollEvents() override;
};

#endif // GAMEPAD_ADAPTER_H