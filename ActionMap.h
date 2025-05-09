#ifndef ACTION_MAP_H
#define ACTION_MAP_H

#include "DeviceEvent.h"
#include <string>
#include <vector>
#include <map>
#include <utility> // For std::pair

// 代表一个逻辑动作，例如 "Attack", "Jump"
struct GameAction {
    std::string name;
    // 可以添加其他与动作相关的属性，如是否是持续性动作等

    // 新增：静态常量表示未知动作
    static const GameAction Unknown;

    // 新增：比较操作符
    bool operator==(const GameAction& other) const {
        return name == other.name;
    }
    bool operator!=(const GameAction& other) const {
        return !(*this == other);
    }
};

// 将物理输入映射到逻辑动作
class ActionMap {
public:
    // 获取单例实例
    static ActionMap& instance() {
        static ActionMap instance;
        return instance;
    }

    // 初始化动作映射（从配置文件加载）
    void initialize(const std::string& bindingsFilePath);

    // 根据设备事件查找对应的游戏动作
    GameAction getAction(DeviceType device, int code) const;

    // 获取所有绑定
    std::map<std::pair<DeviceType, int>, std::vector<std::string>> getAllBindings() const { return bindings; }

    // 根据设备事件获取所有对应的动作
    std::vector<GameAction> getActions(const DeviceEvent& event) const {
        std::vector<GameAction> actions;
        auto it = bindings.find({event.device, event.code});
        if (it != bindings.end()) {
            for (const auto& actionName : it->second) {
                GameAction action;
                action.name = actionName;
                actions.push_back(action);
            }
        }
        return actions;
    }

private:
    // 私有构造函数，防止外部创建实例
    ActionMap() = default;
    // 禁止拷贝和赋值
    ActionMap(const ActionMap&) = delete;
    ActionMap& operator=(const ActionMap&) = delete;

    // 存储绑定规则：(DeviceType, input_code) -> list_of_action_names
    std::map<std::pair<DeviceType, int>, std::vector<std::string>> bindings;
    // 存储动作定义：action_name -> GameAction
    std::map<std::string, GameAction> definedActions;

    void loadBindings(const std::string& filePath);
};

#endif // ACTION_MAP_H