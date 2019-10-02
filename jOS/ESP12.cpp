#include "./System.h"

#if defined(ESP12)

#include <Arduino.h>

/*!
  This variable is the interrupt time. Do not forget to fill it!
*/
const uint8_t ui8TickMS = 1;

/*!
  This variables is the interrupt callbacks!
*/
void (*vSemaphoresManagerCallBack)(void) = NULL;
void (*vTaskSchedulerCallBack)(void) = NULL;

/*!
  Function: CallBack System Interruption
*/
void ICACHE_RAM_ATTR vSystemCallBack(void){
  timer1_write(ui8TickMS*80000);
  if (vTaskSchedulerCallBack != NULL){
    vTaskSchedulerCallBack();
  }
  if (vSemaphoresManagerCallBack != NULL){
    vSemaphoresManagerCallBack();
  }
}

//! Function: Editable System Timer Scheduler Interrupt Configuration
/*!
  Edit this function to configure interruptions timer.
*/
void vTaskTimerConfiguration(void (*vSchedulerInterruption)(void)){
  timer1_attachInterrupt(&vSystemCallBack);
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_SINGLE);
  timer1_write(ui8TickMS*80000);
  timer1_isr_init();
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
  ESP.deepSleep(timer1_read()/80);
  timer1_write(ui8TickMS*80000);
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vEnableWatchdog(void){
  ESP.wdtEnable(1000);
}

//! Function: Editable System Wake Up
/*!
  Edit this function to wake up your microcontroller.
*/
void vDisableWatchdog(void){
  ESP.wdtDisable();
}

//! Function: Editable System Restart
/*!
  Edit this function to restart your microcontroller.
*/
void vRestartSystem(void){
  ESP.restart();
}

#endif