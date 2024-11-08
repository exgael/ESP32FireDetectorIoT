/*
 * Author: Benoît Barbier
 */

#pragma once

#include <queue>

#include "../../logger/index.h"
#include "ICommand.h"

class CommandQueue {
   public:
    CommandQueue();

    /**
     * @brief Add a command to the queue
     */
    void addCommand(ICommand* command) noexcept;

    /**
     * @brief Process all commands in the queue
     */
    void processCommands() noexcept;

   private:
    std::queue<ICommand*> commandQueue;

    Logger logger;
};
