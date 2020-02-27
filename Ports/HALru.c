#include "../jOS/Configs.h"

#if defined(HALru)

  #include "./System.h"
  #include <HALru.h>
  #include <avr/wdt.h>
  #include <EmbeddedTools.h>

  /*!
    This variable is the interrupt time. Do not forget to fill it!
  */
  const uint8_t __ui8TickMS = 10;

  /*!
    This variables is the interrupt callbacks!
  */
  void (*vfSemaphoresManagerCallback)(void) = NULL;
  void (*vfTaskSchedulerCallback)(void) = NULL;

  //! Function: Callback Scheduler Interruption
  /*!
    Converting "void (*pfunc)(void*)" to "void (*pfunc)(void)"
  */
  void vMasterTimerCallback(void* vpArguments) NestedMode {
    vfTaskSchedulerCallback();
  } EndNestedMode;

  //! Function: Callback Semaphore Interruption
  /*!
    Converting "void (*pfunc)(void*)" to "void (*pfunc)(void)"
  */
  void vSubTimerACallback(void* vpArguments) NestedMode {
    vfSemaphoresManagerCallback();
  } EndNestedMode;

  //! Function: Editable System Timer Scheduler Interrupt Configuration
  /*!
    Edit this function to configure interruptions timer.
  */
  void System_taskTimerConfiguration(void (*vSchedulerInterruption)(void)){
    vTIMERInit(TIMER_2);
    vSetTIMERPeriodMS(TIMER_2, __ui8TickMS);
    vfTaskSchedulerCallback = vSchedulerInterruption;
    vAttachTIMERInterrupt(TIMER_2, MASTER_TIMER, vMasterTimerCallback, NULL);
  }

  //! Function: Editable System Timer Semaphore Interrupt Configuration
  /*!
    Edit this function to configure interruptions scheduler.
  */
  void System_semaphoreTimerConfiguration(void (*vSemaphoresInterruption)(void)){
    vfSemaphoresManagerCallback = vSemaphoresInterruption;
    vAttachTIMERInterrupt(TIMER_2, SUBTIMER_A, vSubTimerACallback, NULL);
  }

  //! Function: Editable System Sleep Configuration
  /*!
    Edit this function to configure sleep mode.
  */
  void System_sleepConfiguration(void){
    vSetSleepMode(SLEEP_IDLE_MODE);
  }

  //! Function: Editable System Sleep
  /*!
    Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
  */
  void System_sleep(void){
    vSleepEnable();
  }

  //! Function: Editable System Watchdog Enabler
  /*!
    Edit this function to enable watchdog of your microcontroller.
  */
  void System_enableHardwareWatchdog(void){
    vStartWatchdog(INTERRUPT_RESET_MODE, WATCHDOG_TIMER_1S);
  }

  //! Function: Editable System Watchdog Disabler
  /*!
    Edit this function to disable watchdog of your microcontroller.
  */
  void System_disableHardwareWatchdog(void){
    vStopWatchdog();
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to restart your microcontroller.
  */
  void System_restart(void){
    vForceSystemReset(INTERRUPT_RESET_MODE);
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to enable all interrupts on your microcontroller.
  */
  void System_enableInterrupts(void){
    vEnableTIMERInterrupt(TIMER_2, MASTER_TIMER);
    vEnableTIMERInterrupt(TIMER_2, SUBTIMER_A);
    vEnableAllInterrupts();
  }

#endif
