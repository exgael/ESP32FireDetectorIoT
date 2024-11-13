/*
 * Author: Benoît Barbier
 */

#include "CommandQueue.h"

CommandQueue::CommandQueue() : logger("CommandQueue")
{
}

void CommandQueue::addCommand(ICommand *command) noexcept
{
    if (command) { // Only add non-null commands
        commandQueue.push(command);
    }
}

void CommandQueue::processCommands() noexcept
{
    try {
        while (!commandQueue.empty()) {
            ICommand *command = commandQueue.front();
            commandQueue.pop();

            // Ensure execute non-null commands
            if (command) {
                try {
                    command->execute();
                } catch (const std::exception &e) {
                    logger.error(
                        "Command execution failed: %s", std::string(e.what()));
                } catch (...) {
                    logger.error(
                        "Unknown error occurred during command execution.");
                }
                delete command;
            } else {
                logger.error("Encountered null command in command queue.");
            }
        }
    } catch (const std::exception &e) {
        logger.error(
            "Error in processing command queue: %s", std::string(e.what()));
    } catch (...) {
        logger.error(
            "Unknown error occurred while processing the command queue.");
    }
}
