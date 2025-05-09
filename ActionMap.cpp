#include "ActionMap.h"
#include <fstream>      // 用于文件读取
#include <sstream>      // 用于字符串解析
#include <nlohmann/json.hpp> // 用于JSON解析
#include <iostream>     // 新增：用于 std::cerr

// 新增：使用 nlohmann/json 库的命名空间
using json = nlohmann::json;

// 新增：定义静态成员 GameAction::Unknown
const GameAction GameAction::Unknown = {"Unknown"};

// ActionMap 单例实现（如果需要，通常在 .h 中内联或在此处定义）
// ActionMap& ActionMap::instance() {
//     static ActionMap inst;
//     return inst;
// }

void ActionMap::initialize(const std::string& bindingsFilePath) {
    loadBindings(bindingsFilePath);
}

// 修改：实现 getAction 方法
GameAction ActionMap::getAction(DeviceType device, int code) const {
    auto key = std::make_pair(device, code);
    auto it = bindings.find(key);
    if (it != bindings.end() && !it->second.empty()) {
        // 假设我们只关心第一个映射的动作名
        const std::string& actionName = it->second.front();
        auto actionIt = definedActions.find(actionName);
        if (actionIt != definedActions.end()) {
            return actionIt->second;
        }
    }
    return GameAction::Unknown; // 如果没有找到或动作未定义，返回Unknown
}

// 原有的 getActions 方法，如果不再需要可以考虑移除或注释掉
// std::vector<GameAction> ActionMap::getActions(const DeviceEvent& event) const {
//     std::vector<GameAction> resultActions;
//     auto key = std::make_pair(event.device, event.code);
//     auto it = bindings.find(key);
//     if (it != bindings.end()) {
//         for (const std::string& actionName : it->second) {
//             auto actionIt = definedActions.find(actionName);
//             if (actionIt != definedActions.end()) {
//                 resultActions.push_back(actionIt->second);
//             }
//         }
//     }
//     if (resultActions.empty()) {
//         // 如果没有找到特定动作，可以返回一个表示“无操作”或“未知”的GameAction
//         // resultActions.push_back({ "Unknown" }); 
//     }
//     return resultActions;
// }

void ActionMap::loadBindings(const std::string& filePath) {
    std::ifstream f(filePath);
    if (!f.is_open()) {
        std::cerr << "Error: Could not open bindings file: " << filePath << std::endl;
        return;
    }

    try {
        json data = json::parse(f);

        // 解析定义的动作
        if (data.contains("actions") && data["actions"].is_object()) {
            for (auto& [actionName, actionDetails] : data["actions"].items()) {
                GameAction ga;
                ga.name = actionName;
                // 可以从 actionDetails 中读取更多属性
                definedActions[actionName] = ga;
            }
        }

        // 解析绑定
        if (data.contains("bindings") && data["bindings"].is_object()) {
            for (auto& [deviceTypeStr, deviceBindings] : data["bindings"].items()) {
                DeviceType dt;
                if (deviceTypeStr == "Keyboard") dt = DeviceType::Keyboard;
                else if (deviceTypeStr == "Gamepad") dt = DeviceType::Gamepad;
                else if (deviceTypeStr == "Touch") dt = DeviceType::Touch;
                else {
                    std::cerr << "Warning: Unknown device type in bindings: " << deviceTypeStr << std::endl;
                    continue;
                }

                if (deviceBindings.is_object()) {
                    for (auto& [inputCodeStr, actionNames] : deviceBindings.items()) {
                        try {
                            int inputCode = std::stoi(inputCodeStr);
                            if (actionNames.is_array()) {
                                for (const auto& actionNameJson : actionNames) {
                                    if (actionNameJson.is_string()) {
                                        std::string actionName = actionNameJson.get<std::string>();
                                        if (definedActions.count(actionName)) { // 确保动作已定义
                                            bindings[{dt, inputCode}].push_back(actionName);
                                        } else {
                                            std::cerr << "Warning: Action '" << actionName << "' not defined, but used in binding." << std::endl;
                                        }
                                    }
                                }
                            }
                        } catch (const std::invalid_argument& ia) {
                            std::cerr << "Warning: Invalid input code in bindings: " << inputCodeStr << std::endl;
                        }
                    }
                }
            }
        }
    } catch (json::parse_error& e) {
        std::cerr << "Error parsing JSON bindings file: " << filePath << "\n" << e.what() << std::endl;
    }
    f.close();

    // 解析完成后，将 definedActions 中的内容填充到 GameAction 结构体中
    // （如果 GameAction 结构体需要更多来自 JSON 的数据）
    // 例如，如果 JSON 中有 "Jump": { "canBeHeld": true } 之类的信息
    // 可以在这里更新 definedActions["Jump"].canBeHeld = true;
}