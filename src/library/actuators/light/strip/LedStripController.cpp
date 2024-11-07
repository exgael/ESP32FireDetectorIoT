/*
 * Author: Benoît Barbier
 */


#include "LedStripController.h"

LedStripController::LedStripController(int pin, int numLEDs)
    : strip(numLEDs, pin, NEO_GRB + NEO_KHZ800), numLEDs(numLEDs), actuatorPin(pin), logger("LedStripController") {}

void LedStripController::initialize() noexcept {
    strip.begin();
    strip.clear();
    logger.debug("Initialized with %d leds on pin %d.", numLEDs, actuatorPin);
}

void LedStripController::turnOn() noexcept {
    strip.clear();
    for (int i = 0; i < numLEDs; ++i) {
        strip.setPixelColor(i, strip.Color(Color::White().getRed(), Color::White().getGreen(),
                                           Color::White().getBlue()));  // White color
    }
    strip.show();
}

void LedStripController::turnOff() noexcept {
    strip.clear();
    strip.show();
}

void LedStripController::setBrightness(int brightness) noexcept {
    brightness = constrain(brightness, 0, 255);
    currentBrightness = brightness;
    strip.setBrightness(currentBrightness);
    strip.show();
}

void LedStripController::setColor(int index, const Color& color) noexcept {
    strip.clear();
    if (index >= 0 && index < numLEDs) {
        strip.setPixelColor(index, strip.Color(color.getRed(), color.getGreen(), color.getBlue()));
    }
    strip.show();
}

void LedStripController::setGaugePattern(const Color& color, float percentage, bool ascending) noexcept {
    // Ensure percentage is within 0-100%
    percentage = constrain(percentage, 0.0f, 100.0f);

    // Empty colors
    strip.clear();

    // Calculate the number of LEDs to light up
    int limitIndex = static_cast<int>((percentage / 100.0f) * numLEDs);

    // Cheating here i know, i am assuming
    limitIndex = (limitIndex == 0) ? 1 : limitIndex;

    // Light up LEDs according to the specified percentage
    if (ascending) {
        for (int i = 0; i < limitIndex; ++i) {
            strip.setPixelColor(i, strip.Color(color.getRed(), color.getGreen(), color.getBlue()));
        }
    } else {
        // Descending
        for (int i = numLEDs - 1; i >= numLEDs - limitIndex; --i) {
            strip.setPixelColor(i, strip.Color(color.getRed(), color.getGreen(), color.getBlue()));
        }
    }

    strip.show();
}
