/*
 * Author: Benoît Barbier
 */

#pragma once

#include <stdexcept>

class ICommand {
   public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};
