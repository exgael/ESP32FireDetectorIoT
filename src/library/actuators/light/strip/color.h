/*
 * Author: Benoît Barbier
 */

#pragma once

#include <stdint.h>

/**
 * @brief This class represents a color based on RGB Channels, 0 to 255 per
 * channel.
 */
class Color {
   public:
    Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}

    uint8_t getRed() const { return red; }
    uint8_t getGreen() const { return green; }
    uint8_t getBlue() const { return blue; }

    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Orange() { return Color(255, 165, 0); }
    static Color White() { return Color(255, 255, 255); }

   private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
