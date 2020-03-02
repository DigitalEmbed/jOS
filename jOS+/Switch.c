#include <jOS.h>

#if defined(__SWITCHES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SWITCHES__) && (__AMOUNT_OF_SWITCHES__ > 0)

  #include "./Switch.h"

  /*!
    Switch object "constructor".
  */
  #if defined(__AVR__)
    const switch_manager_t Switch PROGMEM = {
  #else
    const switch_manager_t Switch = {
  #endif
    .turnOn = &Switch_turnOn,
    .turnOff = &Switch_turnOff,
    .getStatus = &Switch_getStatus
  };

#endif