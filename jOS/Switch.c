#include "Switch.h"
#include "jOS.h"

//! Macros: Switch Operations Satus
/*!
  This macros are for facilitate the use of this library.
*/
#define   NOT_BUSY    0
#define   BUSY        1

//! Bit Vector: Switch
/*!
  This bit vector is for managing switches positions in the switch manager.
*/
xCreateBitVector(bvSwitches, AMOUNT_OF_SWITCHES);

//! Task Vector: Switch Task Vector
/*!
  This vector is for switches manager.
*/
task_t* tpSwitchTaskVector[AMOUNT_OF_SWITCHES] = {NULL};

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

//! Function: Switch Taker
/*!
  Take a Switch.
  \param sSwitch is a switch_t type.
  \return Returns SWITCH_BUSY or TAKED_SWITCH.
*/
uint8_t ui8TakeSwitch(switch_t sSwitch){
  if (ui8ReadBitVector(bvSwitches, sSwitch) == BUSY){
    return SWITCH_BUSY;
  }
  else{
    vSetBitVector(bvSwitches, sSwitch);
    tpSwitchTaskVector[sSwitch] = tpGetCurrentTask();
    return TAKED_SWITCH;
  }
}

//! Function: Switch Returner
/*!
  Return a Switch.
  \param sSwitch is a switch_t type.
  \return Returns RETURNED_SWITCH.
*/
uint8_t ui8ReturnSwitch(switch_t sSwitch){
  vEraseBitVector(bvSwitches, sSwitch);
  tpSwitchTaskVector[sSwitch] = NULL;
  return RETURNED_SWITCH;
}

//! Function: Switch Status Getter
/*!
  Get a Switch status.
  \param sSwitch is a switch_t type.
  \return Returns BUSY or NOT_BUSY.
*/
uint8_t ui8GetSwitchStatus(switch_t sSwitch){
  return ui8ReadBitVector(bvSwitches, sSwitch);
}

//! Function: Check Holder Task
/*!
  Check if the current task is the switch keeper.
  \param sSwitch is a switch_t type.
  \return Returns TASK_HOLDER or TASK_NOT_HOLDER.
*/
uint8_t ui8IsThisMySwitch(switch_t sSwitch){
  if (ui8CheckCurrentTask(tpSwitchTaskVector[sSwitch])){
    return TASK_HOLDER;
  }
  else{
    return TASK_NOT_HOLDER;
  }
}
