#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include "ActionMap.h"
#include "Command.h"
#include "ConflictResolver.h"
#include "DeviceEvent.h"
#include <vector>
#include <memory>

// 输入处理器：从事件流和动作映射生成命令
class InputProcessor {
public:
    InputProcessor(const ActionMap& map) : actionMap(map) {}

    // 处理单个输入事件
    void processInput(const DeviceEvent& event) {
        // 首先检查是否应该处理这个输入事件
        if (!conflictResolver.shouldProcessInput(event)) {
            return;
        }
        
        // 生成命令
        auto commands = generateCommandsForEvent(event);
        
        // 执行所有生成的命令
        for (const auto& cmd : commands) {
            cmd->execute();
        }
    }

    // 为单个事件生成命令
    std::vector<std::shared_ptr<ICommand>> generateCommandsForEvent(const DeviceEvent& event) {
        std::vector<std::shared_ptr<ICommand>> commands;
        auto actions = actionMap.getActions(event);
        
        for (const auto& action : actions) {
            auto command = std::make_shared<GameActionCommand>(action, event);
            commands.push_back(command);
        }
        
        return commands;
    }

    // 添加冲突解决策略
    void addConflictStrategy(std::shared_ptr<IConflictResolutionStrategy> strategy) {
        conflictResolver.addStrategy(strategy);
    }

    // 获取冲突解决器
    ConflictResolver& getConflictResolver() { return conflictResolver; }

private:
    const ActionMap& actionMap;
    ConflictResolver conflictResolver;
};

#endif // INPUT_PROCESSOR_H