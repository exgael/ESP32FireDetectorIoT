/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

#include <queue>

#include "ICommand.h"

class CommandQueue {
   public:
    // Methods to add and process commands
    void addCommand(ICommand* command) noexcept;
    void processCommands() noexcept;

   private:
    std::queue<ICommand*> commandQueue;
};
