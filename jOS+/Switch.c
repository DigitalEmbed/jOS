#include "./Switch.h"

/*!
  Switch object "constructor".
*/
const switch_manager_t Switch PROGMEM = {
  .ui8Begin = &ui8SwitchInit,
  .vTurnOn = &vTurnOnSwitch,
  .vTurnOff = &vTurnOffSwitch,
  .ui8GetStatus = &ui8GetSwitchStatus
};