#include "InputProcessor.h"
#include "DeviceEvent.h"
#include "ActionMap.h"
#include "Command.h" // Assuming Command classes like MoveCommand, JumpCommand exist
#include "DeviceManager.h"
#include "ConflictResolver.h"
#include <iostream> // For debug output

InputProcessor::InputProcessor(ActionMap* actionMap, DeviceManager* deviceManager, ConflictResolver* conflictResolver)
    : actionMap_(actionMap), deviceManager_(deviceManager), conflictResolver_(conflictResolver) {
    if (!actionMap_) {
        std::cerr << "Error: ActionMap is null in InputProcessor constructor." << std::endl;
        // Potentially throw an exception or handle error appropriately
    }
    if (!deviceManager_) {
        std::cerr << "Error: DeviceManager is null in InputProcessor constructor." << std::endl;
    }
    if (!conflictResolver_) {
        std::cerr << "Error: ConflictResolver is null in InputProcessor constructor." << std::endl;
    }
    // std::cout << "InputProcessor initialized with ActionMap, DeviceManager, and ConflictResolver." << std::endl;
}

void InputProcessor::processInput(const DeviceEvent& event) {
    if (!actionMap_) {
        std::cerr << "Error: ActionMap is not initialized in processInput." << std::endl;
        return;
    }
    // std::cout << "InputProcessor::processInput called for device: " << static_cast<int>(event.device)
    //           << " code: " << event.code << " value: " << event.value << std::endl;

    GameAction action = actionMap_->getAction(event.device, event.code);

    if (action != GameAction::Unknown) {
        // std::cout << "Mapped event to action: " << static_cast<int>(action) << std::endl;
        // Example: Create and queue a command based on the action
        // std::shared_ptr<ICommand> command;
        // switch (action) {
        //     case GameAction::Jump:
        //         command = std::make_shared<JumpCommand>();
        //         break;
        //     // Add other cases for different actions
        //     default:
        //         std::cout << "Action " << static_cast<int>(action) << " not yet handled for command creation." << std::endl;
        //         break;
        // }
        // if (command) {
        //     commandQueue.push(command);
        //     std::cout << "Command for action " << static_cast<int>(action) << " pushed to queue." << std::endl;
        // }
    } else {
        // std::cout << "No action mapped for this event." << std::endl;
    }
}

void InputProcessor::processEvents(const std::vector<DeviceEvent>& events) {
    for (const auto& event : events) {
        processInput(event); // Reuse single event processing logic
    }
}

std::vector<std::shared_ptr<ICommand>> InputProcessor::getCommands() {
    std::vector<std::shared_ptr<ICommand>> commands;
    while (!commandQueue.empty()) {
        commands.push_back(commandQueue.front());
        commandQueue.pop();
    }
    // std::cout << "Retrieved " << commands.size() << " commands from queue." << std::endl;
    return commands;
}

std::vector<std::shared_ptr<ICommand>> InputProcessor::generateCommandsForEvent(const DeviceEvent& event) {
    if (!actionMap_) {
        std::cerr << "Error: ActionMap is not initialized in generateCommandsForEvent." << std::endl;
        return {};
    }
    std::vector<std::shared_ptr<ICommand>> commands;
    GameAction action = actionMap_->getAction(event.device, event.code);
    if (action != GameAction::Unknown) {
        // Similar command creation logic as in processInput
        // std::shared_ptr<ICommand> command;
        // switch (action) { /* ... */ }
        // if (command) commands.push_back(command);
        // std::cout << "Generated command(s) for event (direct generation)." << std::endl;
    }
    return commands;
}