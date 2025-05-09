#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include "DeviceEvent.h"
#include "ActionMap.h"
#include "Command.h"
#include <vector>
#include <queue>
#include <memory> // For std::shared_ptr

// 输入处理器：从事件流和动作映射生成命令
class InputProcessor {
public:
    InputProcessor(const ActionMap& map);

    // 从队列中获取已生成的命令
    std::vector<std::shared_ptr<ICommand>> getCommands();

    // (可选) 直接根据单个事件生成命令，不使用内部队列
    std::vector<std::shared_ptr<ICommand>> generateCommandsForEvent(const DeviceEvent& event);

    // 新增：便捷处理单个事件
    void processInput(const DeviceEvent& event);

private:
    const ActionMap& actionMap; // 引用动作映射表
    std::queue<std::shared_ptr<ICommand>> commandQueue; // 存储生成的命令
};

#endif // INPUT_PROCESSOR_H