#ifndef KEYBOARD_ADAPTER_H
#define KEYBOARD_ADAPTER_H

#include "IDeviceAdapter.h"

// 键盘适配器（示例，实际接底层API）
class KeyboardAdapter : public IDeviceAdapter {
public:
    std::vector<DeviceEvent> pollEvents() override;
};

#endif // KEYBOARD_ADAPTER_H