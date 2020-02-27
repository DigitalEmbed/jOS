//! Example 03 - Co-routines, threads, switches and another way to blink a LED
/*!
  This simply example demonstrates how to share a variable between tasks.

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

#include <stdint.h>
#include <stdbool.h>
#include <jOS.h>

#define vGenerateLogicLevel(ui8Pin, bLogicLevel)
#define vSetPinMode(ui8Pin, bMode)
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_INPUT 0
#define GPIO_OUTPUT_HIGH 1
#define GPIO_OUTPUT_LOW 0
#define vSerialBegin(ui32BaudRate)
#define vSendSerial(sString1)

/*!
  tTask1 task declaration and implementation.
*/
task_t tTask1;
void vTask1(void* vpArgs){
  /*!
    Pointing out to sTaskFlag switch.
  */
  static switch_t* sTaskFlag = (switch_t*) vpArgs;

  /*!
    Starting co-routine.
  */
  coroutine {
    /*!
      Waiting for "semaphore".
    */
    Task_waitFor(Switch_getStatus(*sTaskFlag) == SWITCH_STATUS_TURNED_OFF);

    /*!
      Executing the function when "semaphore" open.
    */
    vSendSerial("Processing A");

    /*!
      Returning "semaphore".
    */
    Switch_turnOn(*sTaskFlag);

    /*!
      Waiting for "semaphore".
    */
    Task_waitFor(Switch_getStatus(*sTaskFlag) == SWITCH_STATUS_TURNED_OFF);

    /*!
      Executing the function when "semaphore" open.
    */
    vSendSerial("Processing B1");
    vSendSerial("Processing B2");

    /*!
      Returning "semaphore".
    */
    Switch_turnOn(*sTaskFlag);

    /*!
      Waiting for "semaphore".
    */
    Task_waitFor(Switch_getStatus(*sTaskFlag) == SWITCH_STATUS_TURNED_OFF);

    /*!
      Executing the function when "semaphore" open.
    */
    vSendSerial("Processing C");

    /*!
      Returning "semaphore".
    */
    Switch_turnOn(*sTaskFlag);
    
  /*!
    Co-routine finalization (Without finalization, your code will not work).
  */
  } end_coroutine;
}

/*!
  tTask2 task declaration and implementation (Very similar to tTask1).
*/
task_t tTask2;
void vTask2(void* vpArgs) THREAD{
  static switch_t* sTaskFlag = (switch_t*) vpArgs;
  thread_loop {
    Task_waitFor(Switch_getStatus(*sTaskFlag) == SWITCH_STATUS_TURNED_ON);
    vSendSerial("Executing A");
    Switch_turnOff(*sTaskFlag);
    Task_waitFor(Switch_getStatus(*sTaskFlag) == SWITCH_STATUS_TURNED_ON);
    vSendSerial("Executing B");
    Switch_turnOff(*sTaskFlag);
    Task_waitFor(Switch_getStatus(*sTaskFlag) == SWITCH_STATUS_TURNED_ON);
    vSendSerial("Executing C");
    Switch_turnOff(*sTaskFlag);
  }
} END_THREAD

/*!
  tBlink task declaration and implementation.
*/
task_t tBlink;
void vBlink(void* vpArgs){
  coroutine {

    /*!
      This code block will run once a time.
    */
    once{
      vSetPinMode(13, GPIO_MODE_OUTPUT);
    } end_once

    /*!
      Turning on the LED.
    */
    vGenerateLogicLevel(13, GPIO_OUTPUT_LOW);
    
    /*!
      Task pausing.
    */
    Task_delayMS(1000);
    
    /*!
      Turning off the LED.
    */
    vGenerateLogicLevel(13, GPIO_OUTPUT_HIGH);
    
    /*!
      Task pausing.
    */
    Task_delayMS(1000);
  } end_coroutine;
}

/*!
  Main function.
*/
int main(){

  /*!
    Hardwares initialization.
  */
  vSerialBegin(9600);

  /*!
    tTaskFlag switch declaration.
  */
  switch_t sTaskFlag;
  newSwitch(&sTaskFlag);

  /*!
    Tasks installation.
  */
  newTimer(tTask1, "Task1", &vTask1, &sTaskFlag, 200, 100, 1, TASK_MODE_REPEAT, TASK_STATUS_ENABLED);
  newTimer(tBlink, "Blink", &vBlink, &sTaskFlag, 1000, 100, 1, TASK_MODE_REPEAT, TASK_STATUS_ENABLED);
  newThread(tBlink, "Task2", &vTask2, &sTaskFlag, 100, 1, TASK_STATUS_ENABLED);

  #if !(defined(__AUTO_INITIALIZATION_ENABLE__))

    /*!
      Task manager initialization.
    */
    Scheduler_setMode(SCHEDULER_MODE_ROUND_ROBIN);
    Scheduler_enable();
    
    while(1){
      /*!
        Scheduling tasks.
      */
      Scheduler_start();
    }

  #endif
}
