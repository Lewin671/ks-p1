#ifndef TOUCH_ADAPTER_H
#define TOUCH_ADAPTER_H

#include "IDeviceAdapter.h"

// 触摸适配器（示例）
class TouchAdapter : public IDeviceAdapter {
public:
    std::vector<DeviceEvent> pollEvents() override;
};

#endif // TOUCH_ADAPTER_H