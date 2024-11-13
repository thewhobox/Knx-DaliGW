#ifdef ARDUINO_ARCH_RP2040
    #include "OpenKNXHardware.h"
#elif defined(ARDUINO_ARCH_ESP32)
    // #define PROG_LED_PIN 2
    // #define PROG_LED_PIN_ACTIVE_ON HIGH
    // #define PROG_BUTTON_PIN 7
    // #define PROG_BUTTON_PIN_INTERRUPT_ON FALLING
    // #define SAVE_INTERRUPT_PIN 6
    // #define INFO_LED_PIN 3
    // #define INFO_LED_PIN_ACTIVE_ON HIGH
    // #define KNX_SERIAL Serial1
    // #define KNX_UART_RX_PIN 1
    // #define KNX_UART_TX_PIN 0

    #include "OpenKNXHardware.h"
#endif