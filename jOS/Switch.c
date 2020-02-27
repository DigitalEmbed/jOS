#include "./Configs.h"

#if defined(__SWITCHES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SWITCHES__) && (__AMOUNT_OF_SWITCHES__ > 0)

  #include "./Switch.h"
  #include <EmbeddedTools.h>
  #include <MemoryManager.h>

  //! Bit Vector: Switch
  /*!
    This bit vector is for managing switches positions in the switch manager.
  */
  newBitVector(bvSwitches, __AMOUNT_OF_SEMAPHORES__);

  //! Function: Switch Initializer
  /*!
    Initializes a Switch.
    \param swSwitch is a switch_t pointer type.
    \return Returns ERROR_SWITCH_NOT_INITIALIZED or SWITCH_INITIALIZED.
  */
  switch_status_t newSwitch(switch_t* swSwitch){
    static uint8_t ui8SwitchCounter = 0;
    if (ui8SwitchCounter >= __AMOUNT_OF_SEMAPHORES__){
      return SWITCH_STATUS_ERROR_NOT_INITIALIZED;
    }
    *swSwitch = ui8SwitchCounter;
    BitVector_clearBit(bvSwitches, ui8SwitchCounter);
    ui8SwitchCounter++;
    return SWITCH_STATUS_INITIALIZED;
  }

  //! Function: Turn On Switch
  /*!
    Turns on a Switch.
    \param swSwitch is a switch_t type.
  */
  void Switch_turnOn(switch_t swSwitch){
    BitVector_setBit(bvSwitches, swSwitch);
  }

  //! Function: Turn Off Switch
  /*!
    Turns off a Switch.
    \param swSwitch is a switch_t type.
  */
  void Switch_turnOff(switch_t swSwitch){
    BitVector_clearBit(bvSwitches, swSwitch);
  }

  //! Function: Switch Status Getter
  /*!
    Gets a Switch status.
    \param swSwitch is a switch_t type.
    \return Returns BUSY or NOT_BUSY.
  */
  switch_status_t Switch_getStatus(switch_t swSwitch){
    return (BitVector_readBit(bvSwitches, swSwitch)) == 1 ? SWITCH_STATUS_TURNED_ON : SWITCH_STATUS_TURNED_OFF;
  }

#endif