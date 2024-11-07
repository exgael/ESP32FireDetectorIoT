/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

class ICommand {
   public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};
