#include "ConflictResolver.h"

// 示例策略：若当前有触摸滑动事件（code==1），则屏蔽所有手柄方向键（code 10~13）
class TouchSlideFilterStrategy : public IConflictResolutionStrategy {
public:
    bool shouldProcessInput(const DeviceEvent& event) override {
        if (event.device == DeviceType::Touch && event.code == 1) {
            return true;
        }
        if (event.device == DeviceType::Gamepad && event.code >= 10 && event.code <= 13) {
            return false;
        }
        return true;
    }
    
    std::string getName() const override {
        return "TouchSlideFilter";
    }
}; 