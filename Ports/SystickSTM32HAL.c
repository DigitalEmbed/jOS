#include "../jOS/Configs.h"

#if defined(SystickSTM32HAL)

  #include "./System.h"
  #include "main.h"

  /*!
    This variable is the interrupt time. Do not forget to fill it!
  */
  const uint8_t __ui8TaskTickMS = 1;
  const uint8_t __ui8SemaphoreTickMS = 10;

  /*!
    This variables is the interrupt callbacks!
  */
  void (*vfSemaphoresManagerCallback)(void) = NULL;
  void (*vfTaskSchedulerCallback)(void) = NULL;

  /*!
    Function: Callback System Interruption
  */
  void HAL_SYSTICK_Callback(void){
	static uint8_t ui8Counter = 0;
    if (vfTaskSchedulerCallback != NULL){
      vfTaskSchedulerCallback();
    }
    if (ui8Counter == 9){
      ui8Counter = 0;
      if (vfSemaphoresManagerCallback != NULL){
        vfSemaphoresManagerCallback();
      }
    }
    else{
      ui8Counter++;
    }
  }

  //! Function: Editable System Timer Scheduler Interrupt Configuration
  /*!
    Edit this function to configure interruptions timer.
  */
  void System_taskTimerConfiguration(void (*vSchedulerInterruption)(void)){
    vfTaskSchedulerCallback = vSchedulerInterruption;
  }

  //! Function: Editable System Timer Semaphore Interrupt Configuration
  /*!
    Edit this function to configure interruptions scheduler.
  */
  void System_semaphoreTimerConfiguration(void (*vSemaphoresInterruption)(void)){
    vfSemaphoresManagerCallback = vSemaphoresInterruption;
  }

  //! Function: Editable System Sleep Configuration
  /*!
    Edit this function to configure sleep mode.
  */
  void System_sleepConfiguration(void){
	__HAL_DBGMCU_FREEZE_IWDG();
	HAL_DBGMCU_EnableDBGSleepMode();
  }

  //! Function: Editable System Sleep
  /*!
    Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
  */
  void System_sleep(void){
	__WFI();
  }

  //! Function: Editable System Sleep
  /*!
    Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
  */
  void System_enableHardwareWatchdog(void){
  }

  //! Function: Editable System Wake Up
  /*!
    Edit this function to wake up your microcontroller.
  */
  void System_disableHardwareWatchdog(void){
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to restart your microcontroller.
  */
  void System_restart(void){
	HAL_NVIC_SystemReset();
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to enable all interrupts on your microcontroller.
  */
  void System_enableInterrupts(void){
  }

#endif
