//! Example 03 - Co-routines with a switch and another way to blink a LED
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

#include <GPIO.h>
#include <UART.h>
#include <jOS.h>

/*!
  tTask1 task declaration and implementation.
*/
task_t tTask1;
uint8_t ui8Task1(void* vpArgs){
  /*!
    Pointing out to sTaskFlag switch.
  */
  static switch_t* sTaskFlag = (switch_t*) vpArgs;

  /*!
    Starting co-routine.
  */
  coRoutine {
    /*!
      Waiting for "semaphore".
    */
    vWaitFor(ui8GetSwitchStatus(*sTaskFlag) == TURNED_OFF_SWITCH);

    /*!
      Executing the function when "semaphore" open.
    */
    printf("%s: Processing A\n", tpGetCurrentTask()->cpTaskName);

    /*!
      Returning "semaphore".
    */
    vTurnOnSwitch(*sTaskFlag);

    /*!
      Waiting for "semaphore".
    */
    vWaitFor(ui8GetSwitchStatus(*sTaskFlag) == TURNED_OFF_SWITCH);

    /*!
      Executing the function when "semaphore" open.
    */
    printf("%s: Processing B1\n", tpGetCurrentTask()->cpTaskName);
    printf("%s: Processing B2\n", tpGetCurrentTask()->cpTaskName);

    /*!
      Returning "semaphore".
    */
    vTurnOnSwitch(*sTaskFlag);

    /*!
      Waiting for "semaphore".
    */
    vWaitFor(ui8GetSwitchStatus(*sTaskFlag) == TURNED_OFF_SWITCH);

    /*!
      Executing the function when "semaphore" open.
    */
    printf("%s: Processing C\n", tpGetCurrentTask()->cpTaskName);

    /*!
      Returning "semaphore".
    */
    vTurnOnSwitch(*sTaskFlag);
    
  /*!
    Co-routine finalization (Without finalization, your code will not work).
  */
  } end;
}

/*!
  tTask2 task declaration and implementation (Very similar to tTask1).
*/
task_t tTask2;
uint8_t ui8Task2(void* vpArgs){
  static switch_t* sTaskFlag = (switch_t*) vpArgs;
  coRoutine {
    vWaitFor(ui8GetSwitchStatus(*sTaskFlag) == TURNED_ON_SWITCH);
    printf("%s: Executing A\n", tpGetCurrentTask()->cpTaskName);
    vTurnOffSwitch(*sTaskFlag);
    vWaitFor(ui8GetSwitchStatus(*sTaskFlag) == TURNED_ON_SWITCH);
    printf("%s: Executing B\n", tpGetCurrentTask()->cpTaskName);
    vTurnOffSwitch(*sTaskFlag);
    vWaitFor(ui8GetSwitchStatus(*sTaskFlag) == TURNED_ON_SWITCH);
    printf("%s: Executing C\n", tpGetCurrentTask()->cpTaskName);
    vTurnOffSwitch(*sTaskFlag);
  } end;
}

/*!
  tBlink task declaration and implementation.
*/
task_t tBlink;
uint8_t ui8Blink(void* vpArgs){
  vStartCoRoutine();
  while(1){
    /*!
      Turning on the LED.
    */
    vGenerateLogicLevel(13, HIGH);
    
    /*!
      Task pausing.
    */
    vTaskDelay(1000);
    
    /*!
      Turning off the LED.
    */
    vGenerateLogicLevel(13, LOW);
    
    /*!
      Task pausing.
    */
    vTaskDelay(1000);
  }
  vEndCoRoutine();
}

int main(void){
  /*!
    Hardwares initialization.
  */
  vSetPinMode(13, OUTPUT);
  vUARTInit(9600);

  /*!
    Memory manager initialization.
  */
  if (ui8MemoryManagerInit() != MEMORY_MANAGER_INITIALIZED){
    vSystemRestart();
  }

  /*!
    tTaskFlag switch declaration.
  */
  switch_t sTaskFlag;
  
  /*!
    Tasks installations.
  */
  ui8AddTask(&tTask1, &ui8Task1, "Task1", &sTaskFlag, 0, 100, ENABLED);
  ui8AddTask(&tTask2, &ui8Task2, "Task2", &sTaskFlag, 0, 100, ENABLED);
  ui8AddTask(&tBlink, &ui8Blink, "Blink", NULL, 0, 100, ENABLED);

  /*!
    Task manager initialization.
  */
  if (ui8TaskManagerInit() != TASK_MANAGER_INITIALIZED){
    vSystemRestart();
  }
  while(1){
    /*!
      Scheduling tasks.
    */
    vStartScheduler();
  }
}
