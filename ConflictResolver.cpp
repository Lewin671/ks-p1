#include "ConflictResolver.h"

void ConflictResolver::addStrategy(std::shared_ptr<IConflictResolutionStrategy> strategy) {
    if (strategy) {
        strategies.push_back(strategy);
    }
}

std::vector<std::shared_ptr<ICommand>> ConflictResolver::resolve(std::vector<std::shared_ptr<ICommand>>& commands) {
    // 创建一份命令的拷贝进行处理，以免修改原始列表（除非策略明确要这样做）
    // 或者，如果策略被设计为直接修改传入的列表，则不需要拷贝
    // 这里我们假设策略会直接修改传入的列表
    for (const auto& strategy : strategies) {
        if (strategy) {
            strategy->resolve(commands); // 应用每个策略
        }
    }
    return commands; // 返回处理（可能被修改）后的命令列表
}