#include "./Switch.h"

/*!
  Switch object "constructor".
*/
#if defined(__AVR)
  const switch_manager_t Switch PROGMEM = {
#else
  const switch_manager_t Switch = {
#endif
  .turnOn = &Switch_turnOn,
  .turnOff = &Switch_turnOff,
  .getStatus = &Switch_getStatus
};