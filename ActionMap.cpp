#include "ActionMap.h"
#include <fstream>          // 用于文件读取
#include <iostream>         // 用于错误输出
#include "nlohmann/json.hpp" // 用于解析JSON，需要用户确保此库可用

// 使用 nlohmann/json 库的命名空间
using json = nlohmann::json;

void ActionMap::initialize(const std::string& bindingsFilePath) {
    loadBindings(bindingsFilePath);
}

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
}

std::vector<GameAction> ActionMap::getActions(const DeviceEvent& event) const {
    std::vector<GameAction> resultingActions;
    auto it = bindings.find({event.device, event.code});
    if (it != bindings.end()) {
        const auto& actionNameList = it->second;
        for (const std::string& actionName : actionNameList) {
            auto definedActionIt = definedActions.find(actionName);
            if (definedActionIt != definedActions.end()) {
                resultingActions.push_back(definedActionIt->second);
            }
        }
    }
    return resultingActions;
}