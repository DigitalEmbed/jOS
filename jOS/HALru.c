#include "./System.h"

#if defined(HALru)

#include <HALru.h>
#include <avr/wdt.h>
#include <EmbeddedTools.h>

/*!
  This variable is the interrupt time. Do not forget to fill it!
*/
const uint8_t ui8TickMS = 10;

/*!
  This variables is the interrupt callbacks!
*/
void (*vSemaphoresManagerCallBack)(void) = NULL;
void (*vTaskSchedulerCallBack)(void) = NULL;

//! Function: CallBack Scheduler Interruption
/*!
  Converting "void (*pfunc)(void*)" to "void (*pfunc)(void)"
*/
void vMasterTimerCallback(void* vpArguments) NestedMode {
  vTaskSchedulerCallBack();
} EndNestedMode;

//! Function: CallBack Semaphore Interruption
/*!
  Converting "void (*pfunc)(void*)" to "void (*pfunc)(void)"
*/
void vSubTimerACallback(void* vpArguments) NestedMode {
  vSemaphoresManagerCallBack();
} EndNestedMode;

//! Function: Editable System Timer Scheduler Interrupt Configuration
/*!
  Edit this function to configure interruptions timer.
*/
void vTaskTimerConfiguration(void (*vSchedulerInterruption)(void)){
  vTIMERInit(TIMER_2);
  vSetTIMERPeriodMS(TIMER_2, ui8TickMS);
  vTaskSchedulerCallBack = vSchedulerInterruption;
  vAttachTIMERInterrupt(TIMER_2, MASTER_TIMER, vMasterTimerCallback, NULL);
  vEnableTIMERInterrupt(TIMER_2, MASTER_TIMER);
  vEnableAllInterrupts();
}

//! Function: Editable System Timer Semaphore Interrupt Configuration
/*!
  Edit this function to configure interruptions scheduler.
*/
void vSemaphoreTimerConfiguration(void (*vSemaphoresInterruption)(void)){
  vSemaphoresManagerCallBack = vSemaphoresInterruption;
  vAttachTIMERInterrupt(TIMER_2, SUBTIMER_A, vSubTimerACallback, NULL);
  vEnableTIMERInterrupt(TIMER_2, SUBTIMER_A);
}

//! Function: Editable System Sleep Configuration
/*!
  Edit this function to configure sleep mode.
*/
void vSystemSleepConfiguration(void){
  vSetSleepMode(SLEEP_IDLE_MODE);
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vSystemSleep(void){
  vSleepEnable();
}

//! Function: Editable System Watchdog Enabler
/*!
  Edit this function to enable watchdog of your microcontroller.
*/
void vEnableWatchdog(void){
  vStartWatchdog(INTERRUPT_RESET_MODE, WATCHDOG_TIMER_1S);
}

//! Function: Editable System Watchdog Disabler
/*!
  Edit this function to disable watchdog of your microcontroller.
*/
void vDisableWatchdog(void){
  vStopWatchdog();
}

//! Function: Editable System Restart
/*!
  Edit this function to restart your microcontroller.
*/
void vRestartSystem(void){
  vForceSystemReset(INTERRUPT_RESET_MODE);
}

#endif
