#include "ConflictResolver.h"

bool ConflictResolver::shouldProcessInput(const DeviceEvent& event) const {
    // 应用所有策略，如果任何一个策略返回false，则事件不应该被处理
    for (const auto& strategy : strategies) {
        if (strategy && !strategy->shouldProcessInput(event)) {
            return false;
        }
    }
    return true;
} 