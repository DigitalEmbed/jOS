#include "./System.h"

#if defined(ArduinoAVR)

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>

/*!
  This variable is the interrupt time. Do not forget to fill it!
*/
const uint8_t ui8TickMS = 10;

/*!
  This variables is the interrupt callbacks!
*/
void (*vSemaphoresManagerCallBack)(void) = NULL;
void (*vTaskSchedulerCallBack)(void) = NULL;

/*!
  Function: CallBack System Interruption
*/
ISR(TIMER1_OVF_vect){
  TCNT1 = 60536;
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
  cli();
  /*!
    TIMER 1 for interrupt frequency 1000 Hz:
  */
  TCCR1A = 0;
  TCCR1B = 0;
  /*!
    Sets compare match register for 100 Hz (10ms) increments
    16000000 / (64 * 100) - 1 (must be <256)
  */
  TCNT1 = 63036;
  /*!
    Set CS11 and CS10 bits for 64 prescaler
  */
  vSetBit(TCCR1B, CS11);
  vSetBit(TCCR1B, CS10);
  /*!
    Enables timer compare interrupt
  */
  vSetBit(TIMSK1, TOIE1);
  vTaskSchedulerCallBack = vSchedulerInterruption;
  sei();
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
  set_sleep_mode(SLEEP_MODE_IDLE);
  cli();
  sleep_enable();
  sei();
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vSystemSleep(void){
  sleep_cpu();
  sleep_enable();
}

//! Function: Editable System Sleep
/*!
  Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
*/
void vEnableWatchdog(void){
  wdt_enable(WDTO_1S);
}

//! Function: Editable System Wake Up
/*!
  Edit this function to wake up your microcontroller.
*/
void vDisableWatchdog(void){
  wdt_disable();
}

//! Function: Editable System Restart
/*!
  Edit this function to restart your microcontroller.
*/
void vRestartSystem(void){
  wdt_enable(WDTO_15MS);
  while(1);
}

#endif