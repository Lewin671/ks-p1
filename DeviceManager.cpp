#include "DeviceManager.h"
#include "KeyboardAdapter.h" // 用于 dynamic_cast
#include "GamepadAdapter.h"  // 用于 dynamic_cast
#include "TouchAdapter.h"    // 用于 dynamic_cast
#include <vector>           // 用于 std::vector
#include <algorithm>        // 用于 std::remove，虽然已在.h中包含，但明确包含是个好习惯
#include <mutex>            // 用于 std::lock_guard，虽然已在.h中包含

DeviceManager& DeviceManager::instance() {
    static DeviceManager mgr;
    return mgr;
}

void DeviceManager::registerAdapter(std::shared_ptr<IDeviceAdapter> adapter) {
    std::lock_guard<std::mutex> lk(mtx);
    if (adapter) {
        adapters.push_back(adapter);
    }
}

void DeviceManager::unregisterAdapter(std::shared_ptr<IDeviceAdapter> adapter) {
    std::lock_guard<std::mutex> lk(mtx);
    if (adapter) {
        adapters.erase(
            std::remove(adapters.begin(), adapters.end(), adapter),
            adapters.end());
    }
}

std::vector<DeviceEvent> DeviceManager::pollEvents() {
    std::lock_guard<std::mutex> lk(mtx);
    std::vector<DeviceEvent> allEvents;
    for (const auto& adapter : adapters) {
        if (adapter && adapter->isEnabled()) {
            std::vector<DeviceEvent> deviceEvents = adapter->pollEvents();
            // 将 deviceEvents 中的所有元素追加到 allEvents 的末尾
            allEvents.insert(allEvents.end(), deviceEvents.begin(), deviceEvents.end());
        }
    }
    return allEvents;
}

void DeviceManager::enableDevice(DeviceType type, bool on) {
    std::lock_guard<std::mutex> lk(mtx);
    for (auto& adapter : adapters) {
        if (!adapter) continue;
        bool typeMatch = false;
        // 使用 dynamic_cast 检查适配器的实际类型
        if (type == DeviceType::Keyboard) {
            if (dynamic_cast<KeyboardAdapter*>(adapter.get())) {
                typeMatch = true;
            }
        } else if (type == DeviceType::Gamepad) {
            if (dynamic_cast<GamepadAdapter*>(adapter.get())) {
                typeMatch = true;
            }
        } else if (type == DeviceType::Touch) {
            if (dynamic_cast<TouchAdapter*>(adapter.get())) {
                typeMatch = true;
            }
        }
        if (typeMatch) {
            adapter->enable(on);
        }
    }
}

// 新增：设置当前活跃设备
void DeviceManager::setActiveDevice(DeviceType type) {
    std::lock_guard<std::mutex> lk(mtx);
    activeDevice = type;
}

// 新增：获取当前活跃设备
DeviceType DeviceManager::getActiveDevice() const {
    return activeDevice;
}