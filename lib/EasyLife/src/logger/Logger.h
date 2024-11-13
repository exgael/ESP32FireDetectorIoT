/*
 * Author: Benoît Barbier
 */

#pragma once

#include <Arduino.h>

enum LogLevel { LOG_LEVEL_NONE = 0, LOG_LEVEL_ERROR, LOG_LEVEL_WARN, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG };

// Like in java ~(*.*)~ kinda.
class Logger {
   public:
    Logger(const char* scope) : scope(scope) {}

    static void begin(unsigned long baudRate = 9600, LogLevel level = LOG_LEVEL_DEBUG);

    void setLevel(LogLevel level);
    LogLevel getLevel() const noexcept;

    void error(const char* format, ...) const noexcept;
    void warn(const char* format, ...) const noexcept;
    void info(const char* format, ...) const noexcept;
    void debug(const char* format, ...) const noexcept;

   private:
    static LogLevel currentLevel;
    const char* scope;

    void log(LogLevel level, const char* levelStr, const char* format, va_list args) const noexcept;
};
