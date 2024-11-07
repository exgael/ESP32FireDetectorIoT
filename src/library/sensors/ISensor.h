/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#pragma once

class ISensor {
   public:
    virtual void initialize() = 0;
    virtual float readData() = 0;
    virtual ~ISensor() = default;
};
