#ifdef ARDUINO_ARCH_RP2040
    #include "OpenKNXHardware.h"
    #define DALI_TX 17
    #define DALI_RX 16
#elif defined(ARDUINO_ARCH_ESP32)
    #define DALI_TX 7
    #define DALI_RX 20
    #include "OpenKNXHardware.h"
#endif