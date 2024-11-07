/*
 * Author: Benoît Barbier
 */


#pragma once

#include <Adafruit_NeoPixel.h>

#include "../../../logger/index.h"
#include "../ILedActuator.h"
#include "Color.h"

class LedStripController : public ILedActuator {
   public:
    LedStripController(int pin, int numLEDs);

    /**
     * @brief procedure: Activate the strip, clear color memory, and trasmit
     * 'off/black' to RAM.
     */
    void initialize() noexcept;

    /**
     * @brief Turns on all LEDs with the color `white` as default.
     */
    void turnOn() noexcept override;

    /**
     * @brief Turns off by dumping all colors.
     */
    void turnOff() noexcept override;

    /**
     * @brief Modify the brightness of `all` leds in the strip.
     */
    void setBrightness(int brightness) noexcept override;

    /**
     * @brief Set a single color on a specific LED
     */
    void setColor(int index, const Color& color) noexcept;

    /**
     * @brief Set a gauge pattern with specified color, percentage, and order.
     *
     * Percentage is used as it enables to decouple the quantity from the actual
     * #of leds.
     */
    void setGaugePattern(const Color& color, float percentage, bool ascending = true) noexcept;

   private:
    Adafruit_NeoPixel strip;
    int numLEDs;
    int actuatorPin;
    size_t currentBrightness;
    Logger logger;
};
