#ifndef CONFLICT_RESOLVER_H
#define CONFLICT_RESOLVER_H

#include "Command.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream> // For example strategy

// 冲突解决策略接口 (Strategy Pattern)
class IConflictResolutionStrategy {
public:
    virtual ~IConflictResolutionStrategy() = default;
    // 应用策略，可能会修改命令列表（如移除、重排序）或命令本身
    virtual void resolve(std::vector<std::shared_ptr<ICommand>>& commands) = 0;
    virtual std::string getName() const = 0;
};

// 示例策略：最后输入优先 (基于时间戳，假设时间戳越大越新)
// 注意：一个更完整的实现可能需要命令有时间戳，或者依赖于它们在列表中的顺序
class LastInputWinsStrategy : public IConflictResolutionStrategy {
public:
    void resolve(std::vector<std::shared_ptr<ICommand>>& commands) override {
        if (commands.empty()) return;

        // 这是一个简化的示例。实际的“最后输入优先”可能更复杂，
        // 例如，如果多个命令是针对同一个动作的，则只保留最新的那个。
        // 或者，如果命令来自不同设备但目标相同，则保留最新设备的命令。
        // 这里我们仅打印一个消息，并不实际修改列表，因为需要更具体的冲突定义。
        std::cout << "Applying LastInputWinsStrategy (conceptual)" << std::endl;
        // 若要实现，可能需要排序或筛选，例如：
        // std::sort(commands.begin(), commands.end(), [](const auto& a, const auto& b) {
        //     // 假设 ICommand 有 getTimestamp() 方法
        //     return a->getTimestamp() > b->getTimestamp(); 
        // });
        // 然后可能只取第一个，或者根据动作类型进行更细致的筛选。
    }
    std::string getName() const override { return "LastInputWins"; }
};

// 示例策略：特定设备优先 (例如，触摸优先于手柄)
class DevicePriorityStrategy : public IConflictResolutionStrategy {
public:
    DevicePriorityStrategy(DeviceType preferred, DeviceType lessPreferred)
        : preferredDevice(preferred), lessPreferredDevice(lessPreferred) {}

    void resolve(std::vector<std::shared_ptr<ICommand>>& commands) override {
        bool preferredFound = false;
        bool lessPreferredFound = false;
        GameAction actionFromPreferred; // 假设冲突只针对同一动作

        for (const auto& cmd : commands) {
            if (cmd->getSourceDevice() == preferredDevice) {
                preferredFound = true;
                actionFromPreferred = cmd->getAction(); // 记录优先设备触发的动作
                break; // 简单示例：一旦找到优先设备，就假设它赢了
            }
        }
        if(preferredFound){
            commands.erase(std::remove_if(commands.begin(), commands.end(),
                                        [&](const std::shared_ptr<ICommand>& cmd) {
                                            // 移除所有由次选设备触发的、且与优先设备动作相同的命令
                                            return cmd->getSourceDevice() == lessPreferredDevice && cmd->getAction().name == actionFromPreferred.name;
                                        }),
                        commands.end());
            std::cout << "Applied DevicePriorityStrategy: " << static_cast<int>(preferredDevice) 
                      << " over " << static_cast<int>(lessPreferredDevice) 
                      << " for action '" << actionFromPreferred.name << "'" << std::endl;
        }
    }
    std::string getName() const override { return "DevicePriority"; }
private:
    DeviceType preferredDevice;
    DeviceType lessPreferredDevice;
};


// 冲突解析器
class ConflictResolver {
public:
    void addStrategy(std::shared_ptr<IConflictResolutionStrategy> strategy);
    
    // 应用所有注册的策略来解析命令冲突
    std::vector<std::shared_ptr<ICommand>> resolve(std::vector<std::shared_ptr<ICommand>>& commands);

private:
    std::vector<std::shared_ptr<IConflictResolutionStrategy>> strategies;
};

#endif // CONFLICT_RESOLVER_H