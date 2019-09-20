#include "./System.h"

#if defined(NoMicrocontroller)

/*!
  This variable is the interrupt time. Do not forget to fill it!
*/
const uint8_t ui8TickMS = 10;

/*!
  This variables is the interrupt callbacks!
*/
void (*vSemaphoresManagerCallBack)(void) = NULL;
void (*vTaskSchedulerCallBack)(void) = NULL;

//! Function: Editable System Timer Scheduler Interrupt Configuration
/*!
  Edit this function to configure interruptions timer.
*/
void vTaskTimerConfiguration(void (*vSchedulerInterruption)(void)){
  vTaskSchedulerCallBack = vSchedulerInterruption;
}

//! Function: Editable System Timer Semaphore Interrupt Configuration
/*!
  Edit this function to configure interruptions scheduler.
*/
void vSemaphoreTimerConfiguration(void (*vSemaphoresInterruption)(void)){
  vSemaphoresManagerCallBack = vSemaphoresInterruption;
}

//! Function: Editable System Sleep Configuration
/*!
  Edit this function to configure sleep mode.
*/
void vSystemSleepConfiguration(void){
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vSystemSleep(void){
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vEnableWatchdog(void){
}

//! Function: Editable System Wake Up
/*!
  Edit this function to wake up your microcontroller.
*/
void vDisableWatchdog(void){
}

//! Function: Editable System Restart
/*!
  Edit this function to restart your microcontroller.
*/
void vRestartSystem(void){
}

#endif