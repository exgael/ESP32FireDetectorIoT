set(COMPONENT_SRCDIRS "src")             # Sets the directory for source files (e.g., `src`)
set(COMPONENT_ADD_INCLUDEDIRS "src")     # Sets the directory for include files

set(COMPONENT_REQUIRES                   # Specifies dependencies for this component
    "arduino-esp32"
    "Adafruit NeoPixel"
    "DallasTemperature"
    "OneWire"
    "ESPAsyncWebServer"
    "ArduinoJson"
)

register_component()                     # Registers the component with ESP-IDF, so it’s compiled and linked

# Define specific compile options for this component
target_compile_definitions(${COMPONENT_TARGET} PUBLIC -DESP32)
target_compile_options(${COMPONENT_TARGET} PRIVATE -fno-rtti)
