/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "CommandQueue.h"

// Add a command to the queue
void CommandQueue::addCommand(ICommand* command) noexcept { commandQueue.push(command); }

// Process all commands in the queue
void CommandQueue::processCommands() noexcept {
    while (!commandQueue.empty()) {
        ICommand* command = commandQueue.front();
        command->execute();
        commandQueue.pop();
        delete command;
    }
}
