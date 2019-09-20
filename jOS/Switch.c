#include "./Switch.h"

#include "./Configs.h"
#include <EmbeddedTools.h>

//! Bit Vector: Switch
/*!
  This bit vector is for managing switches positions in the switch manager.
*/
xCreateBitVector(bvSwitches, AMOUNT_OF_SWITCHES);

//! Function: Switch Initializer
/*!
  Initialize a Switch.
  \param sSwitch is a switch_t pointer type.
  \return Returns ERROR_SWITCH_NOT_INITIALIZED or SWITCH_INITIALIZED.
*/
uint8_t ui8SwitchInit(switch_t* spSwitch){
  static uint8_t ui8SwitchCounter = 0;
  if (ui8SwitchCounter >= AMOUNT_OF_SWITCHES){
    return ERROR_SWITCH_NOT_INITIALIZED;
  }
  *spSwitch = ui8SwitchCounter;
  vEraseBitVector(bvSwitches, ui8SwitchCounter);
  ui8SwitchCounter++;
  return SWITCH_INITIALIZED;
}

//! Function: Turn On Switch
/*!
  Turn on a Switch.
  \param sSwitch is a switch_t type.
*/
void vTurnOnSwitch(switch_t* sSwitch){
  vSetBitVector(bvSwitches, *(sSwitch));
}

//! Function: Turn Off Switch
/*!
  Turn off a Switch.
  \param sSwitch is a switch_t type.
*/
void vTurnOffSwitch(switch_t* sSwitch){
  vEraseBitVector(bvSwitches, *(sSwitch));
}

//! Function: Switch Status Getter
/*!
  Get a Switch status.
  \param sSwitch is a switch_t type.
  \return Returns BUSY or NOT_BUSY.
*/
uint8_t ui8GetSwitchStatus(switch_t* sSwitch){
  return ui8ReadBitVector(bvSwitches, *(sSwitch));
}
