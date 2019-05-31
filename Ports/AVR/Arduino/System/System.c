/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copyleft license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -> Commercial use;
    -> Modification;
    -> Distribuition;
    -> Patent Use;
    -> Private Use;

  * Limitations:
    -> Liability;
    -> Warranty;

  * Conditions:
    -> License and copyright notice;
    -> Disclose source;
    -> State changes;
    -> Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#include "System.h"
#include "../jOS.h"
#include "../MemoryManager/Bits/Bits.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

isr_t isrTimerInterrupt = NULL;

/*!
  This variable is the interrupt time. Do not forget to fill it!
*/
const uint8_t ui8TickMS = 10;

//! Function: Editable System Timer Interrupt Configuration
/*!
  Edit this function to configure interruptions timer.
*/
void vSystemTimerInterruptConfiguration(isr_t isrSchedulerInterrupt){
  cli();
  /*!
    TIMER 1 for interrupt frequency 1000 Hz:
  */
  TCCR1A = 0;
  TCCR1B = 0;
  /*!
    Set compare match register for 100 Hz (10ms) increments
    16000000 / (64 * 100) - 1 (must be <256)
  */
  TCNT1 = 63035;
  /*!
    Set CS11 and CS10 bits for 64 prescaler
  */
  vSetBit(TCCR1B, CS11);
  vSetBit(TCCR1B, CS10);
  /*!
    Enable timer compare interrupt
  */
  vSetBit(TIMSK1, TOIE1);
  isrTimerInterrupt = isrSchedulerInterrupt;
  sei();
}

ISR(TIMER1_OVF_vect){
  TCNT1 = 60535;
  if (isrTimerInterrupt != NULL){
    isrTimerInterrupt();
  }
}

//! Function: Editable System Sleep Configuration
/*!
  Edit this function to configure sleep mode.
*/
void vSystemSleepConfiguration(){
  set_sleep_mode(SLEEP_MODE_IDLE);
  cli();
  sleep_enable();
  sei();
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
  sleep_cpu();
  sleep_enable();
}

//! Function: Editable System Wake Up
/*!
  Edit this function to wake up your microcontroller.
*/
void vSystemWakeUp(){
  sleep_disable();
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
  vEraseBit(TIMSK1, OCIE1A);
}

//! Function: Editable System Resume Interruptions
/*!
  Edit this function so that it resume RTOS interruptios when it is called.
*/
void vSystemResumeRTOSInterrupts(){

  vSetBit(TIMSK1, OCIE1A);
}

//! Function: Editable System Suspend Interruptions
/*!
  Edit this function so that it suspends all interruptios when it is called.
*/
void vSystemSuspendAllInterrupts(){
  cli();
}

//! Function: Editable System Resume Interruptions
/*!
  Edit this function so that it resume all interruptios when it is called.
*/
void vSystemResumeAllInterrupts(){
  sei();
}

//! Function: Editable Initializer System Restart Timer
/*!
  In many microcontrollers there is no "automatic watchdog". To work around this, edit this function so that it schedules a system restart for a longer time than the watchdog software.
*/
void vSystemRestartTimerInit(){
  wdt_enable(WDTO_500MS);
}

//! Function: Editable Cancellation System Restart Timer
/*!
  Soon after performing the task, the system will perform this function to cancel the system restart that was previously scheduled. If the system fails and the watchdog software does not work, this auxiliary system will act.
*/
void vSystemRestartTimerStop(){
  wdt_disable();
}
