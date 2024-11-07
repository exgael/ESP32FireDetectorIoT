/*
 * Author: Benoît Barbier
 * Created: 2024-11-07
 * Last Modified: 2024-11-07
 */


#include "Logger.h"

#include "../../library/index.h"

LogLevel Logger::currentLevel = LOG_LEVEL_DEBUG;

void Logger::begin(unsigned long baudRate, LogLevel level) {
    Serial.begin(baudRate);
    while (!Serial) {
    }  // Wait for serial port to connect.
    currentLevel = level;
}

void Logger::setLevel(LogLevel level) { currentLevel = level; }

LogLevel Logger::getLevel() const noexcept { return currentLevel; }

/**
 * base https://github.com/exgael/OthelloGameAgent/blob/main/src/Agent/agent.cpp
 */
void Logger::log(LogLevel level, const char* levelStr, const char* format, va_list args) const noexcept {
    if (level <= currentLevel) {
        // Get elapsed time
        unsigned long elapsedTimeMillis = millis();
        float elapsedTimeSeconds = elapsedTimeMillis / 1000.0;
        String timeStr = String(elapsedTimeSeconds);

        // Pad
        char paddedScope[21];
        snprintf(paddedScope, sizeof(paddedScope), "%-20s", scope);

        // Pad
        char paddedLevel[6];
        snprintf(paddedLevel, sizeof(paddedLevel), "%-5s", levelStr);

        // Print the log message with padded elements
        Serial.print("[");
        Serial.print(timeStr);
        Serial.print("s] ");
        Serial.print("[");
        Serial.print(paddedScope);
        Serial.print("] ");
        Serial.print("[");
        Serial.print(paddedLevel);
        Serial.print("] ");

        // Format and print the actual message
        char buffer[512];  // a bit big for the json payload to print
        vsnprintf(buffer, sizeof(buffer), format, args);
        Serial.println(buffer);
    }
}

void Logger::error(const char* format, ...) const noexcept {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_ERROR, "ERROR", format, args);
    va_end(args);
}

void Logger::warn(const char* format, ...) const noexcept {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_WARN, "WARN", format, args);
    va_end(args);
}

void Logger::info(const char* format, ...) const noexcept {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_INFO, "INFO", format, args);
    va_end(args);
}

void Logger::debug(const char* format, ...) const noexcept {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_DEBUG, "DEBUG", format, args);
    va_end(args);
}
