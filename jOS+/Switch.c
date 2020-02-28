#include "./Switch.h"

/*!
  Switch object "constructor".
*/
const switch_manager_t Switch PROGMEM = {
  .turnOn = &Switch_turnOn,
  .turnOff = &Switch_turnOff,
  .getStatus = &Switch_getStatus
};