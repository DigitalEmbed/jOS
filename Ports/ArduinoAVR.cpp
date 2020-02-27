#include "./Configs.h"

#if defined(ArduinoAVR)

  #include "./System.h"
  #include <EmbeddedTools.h>
  #include <avr/sleep.h>
  #include <avr/interrupt.h>
  #include <avr/wdt.h>
  #include <avr/io.h>

  /*!
    This variable is the interrupt time. Do not forget to fill it!
  */
  const uint8_t __ui8TaskTickMS = 10;
  const uint8_t __ui8SemaphoreTickMS = 50;

  /*!
    This variables is the interrupt callbacks!
  */
  void (*vfSemaphoresManagerCallback)(void) = NULL;
  void (*vfTaskSchedulerCallback)(void) = NULL;

  /*!
    Function: Callback System Interruption
  */
  ISR(TIMER1_OVF_vect){
    TCNT1 = 63036;
    static uint8_t ui8Counter = 0;
    if (vfTaskSchedulerCallback != NULL){
      vfTaskSchedulerCallback();
    }
    if (ui8Counter == 4){
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
    TCCR1B |= (1 << CS11)|(1 << CS10);
    /*!
      Enables timer compare interrupt
    */
    TIMSK1 |= (1 << TOIE1);
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
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
  }

  //! Function: Editable System Sleep
  /*!
    Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
  */
  void System_sleep(void){
    sleep_cpu();
    sleep_enable();
  }

  //! Function: Editable System Sleep
  /*!
    Edit this function to put your microcontroller to sleep. Remember to enable in a mode where timer interrupts are not turned off.
  */
  void System_enableHardwareWatchdog(void){
    wdt_enable(WDTO_8S);
  }

  //! Function: Editable System Wake Up
  /*!
    Edit this function to wake up your microcontroller.
  */
  void System_disableHardwareWatchdog(void){
    wdt_disable();
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to restart your microcontroller.
  */
  void System_restart(void){
    wdt_enable(WDTO_15MS);
    while(1);
  }

  //! Function: Editable System Restart
  /*!
    Edit this function to enable all interrupts on your microcontroller.
  */
  void System_enableInterrupts(void){
    sei();
  }

#endif