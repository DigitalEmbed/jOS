#include "../jOS/Configs.h"

#if defined(NoMicrocontroller)

  #include "./System.h"

  /*!
    This variable is the interrupt time. Do not forget to fill it!
  */
  const uint8_t __ui8TaskTickMS = 1;
  const uint8_t __ui8SemaphoreTickMS = 1;

  /*!
    This variables is the interrupt callbacks!
  */
  void (*vfSemaphoresManagerCallback)(void) = NULL;
  void (*vfTaskSchedulerCallback)(void) = NULL;

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
  }

  //! Function: Editable System Sleep
  /*!
    Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
  */
  void System_sleep(void){
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
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to enable all interrupts on your microcontroller.
  */
  void System_enableInterrupts(void){
  }

#endif