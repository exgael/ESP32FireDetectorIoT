/*
 * Author: Benoît Barbier
 */


#pragma once

class ISensor {
   public:
    virtual void initialize() = 0;
    virtual float readData() = 0;
    virtual ~ISensor() = default;
};
