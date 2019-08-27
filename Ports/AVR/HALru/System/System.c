#include "System.h"
#include <jOS.h>
#include <HALru.h>
#include <avr/wdt.h>

/*!
  This variable is the interrupt callbacks!
*/
task_isr_t isrSemaphoreFunctionInterrupt = NULL;
task_isr_t isrSchedulerFunctionInterrupt = NULL;

/*!
  This variable is the interrupt time. Do not forget to fill it!
*/
const uint8_t ui8TickMS = 10;

//! Function: CallBack Scheduler Interruption
/*!
  Converting "void (*pfunc)(void*)" to "void (*pfunc)(void)"
*/
void vSchedulerInterruption(void* vpArgs) NestedMode{
  if (isrSchedulerFunctionInterrupt != NULL){
    isrSchedulerFunctionInterrupt();
  }
} EndNestedMode

//! Function: CallBack Semaphore Interruption
/*!
  Converting "void (*pfunc)(void*)" to "void (*pfunc)(void)"
*/
void vSemaphoreInterruption(void* vpArgs) NestedMode{
  if (isrSchedulerFunctionInterrupt != NULL){
    isrSemaphoreFunctionInterrupt();
  }
} EndNestedMode

//! Function: Editable System Timer Scheduler Interrupt Configuration
/*!
  Edit this function to configure interruptions timer.
*/
void vSystemTimerSchedulerInterruption(task_isr_t isrSchedulerInterruptCallback){
  /*!
    TIMER 2 for interrupt frequency 1000 Hz:
  */
  vTIMERInit(TIMER_2);
  vSetTIMERPeriodMS(TIMER_2, ui8TickMS);
  isrSchedulerFunctionInterrupt = isrSchedulerInterruptCallback;
  vAttachTIMERInterrupt(TIMER_2, MASTER_TIMER, vSchedulerInterruption, NULL);
  vEnableTIMERInterrupt(TIMER_2, MASTER_TIMER);
  vEnableAllInterrupts();
}

//! Function: Editable System Timer Semphores Interrupt Configuration
/*!
  Edit this function to configure interruptions timer.
*/
void vSystemTimerSemaphoresInterruption(task_isr_t isrSemaphoresInterruptCallback){
  /*!
    SUB-TIMER A (TIMER 2) for interrupt frequency 1000 Hz:
  */
  isrSemaphoreFunctionInterrupt = isrSemaphoresInterruptCallback;
  vAttachTIMERInterrupt(TIMER_2, SUBTIMER_A, vSemaphoreInterruption, NULL);
  vEnableTIMERInterrupt(TIMER_2, SUBTIMER_A);
}

//! Function: Editable System Sleep Configuration
/*!
  Edit this function to configure sleep mode.
*/
void vSystemSleepConfiguration(){
  vSetSleepMode(SLEEP_IDLE_MODE);
}

//! Function: Editable System Restart
/*!
  Edit this function to restart your microcontroller.
*/
void vSystemRestart(){
  wdt_enable(WDTO_15MS);
  while(1);
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vSystemSleep(){
  vSleepEnable();
}

//! Function: Editable System Wake Up
/*!
  Edit this function to wake up your microcontroller.
*/
void vSystemWakeUp(){
  vSleepDisable();
}

//! Function: Editable System Hardware Watchdog
/*!
  Edit this function to configure and start the hardware watchdog timer. Remember to set up the hardware watchdog timer for a longer time than the software watchdog
*/
void vSystemHardwareWatchdogConfiguration(){
}

//! Function: Editable System Suspend Interruptions
/*!
  Edit this function so that it suspends RTOS interruptios when it is called.
*/
void vSystemSuspendRTOSInterrupts(){
  vDisableTIMERInterrupt(TIMER_2, MASTER_TIMER);
}

//! Function: Editable System Resume Interruptions
/*!
  Edit this function so that it resume RTOS interruptios when it is called.
*/
void vSystemResumeRTOSInterrupts(){
  vEnableTIMERInterrupt(TIMER_2, MASTER_TIMER);
}

//! Function: Editable System Suspend Interruptions
/*!
  Edit this function so that it suspends all interruptios when it is called.
*/
void vSystemSuspendAllInterrupts(){
  vDisableAllInterrupts();
}

//! Function: Editable System Resume Interruptions
/*!
  Edit this function so that it resume all interruptios when it is called.
*/
void vSystemResumeAllInterrupts(){
  vEnableAllInterrupts();
}

//! Function: Editable Initializer System Restart Timer
/*!
  In many microcontrollers there is no "automatic watchdog". To work around this, edit this function so that it schedules a system restart for a longer time than the watchdog software.
*/
void vSystemRestartTimerInit(){
  wdt_enable(WDTO_1S);
}

//! Function: Editable Cancellation System Restart Timer
/*!
  Soon after performing the task, the system will perform this function to cancel the system restart that was previously scheduled. If the system fails and the watchdog software does not work, this auxiliary system will act.
*/
void vSystemRestartTimerStop(){
  wdt_disable();
}
