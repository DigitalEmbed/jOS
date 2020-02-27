//! Example 05 - Threads with binnary semaphore
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

#include <UART.h>
#include <jOS.h>

/*!
  tTask1 task declaration and implementation.
*/
task_t tTask1;
uint8_t ui8Task1(void* vpArgs){
  /*!
    Pointing out to smTaskFlag semaphore.
  */
  static semaphore_t* smTaskFlag = (semaphore_t*) vpArgs;

  /*!
    Starting co-routine.
  */
  CoRoutine {
    /*!
      Waiting for "semaphore".
    */
    vWaitFor(ui8TakeSemaphore(smTaskFlag) == TASK_HOLDER);

    /*!
      Executing the function when "semaphore open".
    */
    printf("%s: Processing A\n", tpGetCurrentTask()->cpTaskName);
    printf("%s: Processing B\n", tpGetCurrentTask()->cpTaskName);
    printf("%s: Processing C\n", tpGetCurrentTask()->cpTaskName);

    /*!
      Returning "semaphore".
    */
    ui8ReturnSemaphore(smTaskFlag);

    /*!
      Pausing the task.
    */
    vTaskYield();

  /*!
    Co-routine finalization (Without finalization, your code will not work).
  */
  } EndCoRoutine;
}

/*!
  tTask2 task declaration and implementation (Very similar to tTask1).
*/
task_t tTask2;
uint8_t ui8Task2(void* vpArgs){
  static semaphore_t* smTaskFlag = (semaphore_t*) vpArgs;
  CoRoutine {
    vWaitFor(ui8TakeSemaphore(smTaskFlag) == TASK_HOLDER);
    printf("%s: Executing A\n", tpGetCurrentTask()->cpTaskName);
    printf("%s: Executing B\n", tpGetCurrentTask()->cpTaskName);
    printf("%s: Executing C\n", tpGetCurrentTask()->cpTaskName);
    ui8ReturnSemaphore(smTaskFlag);
    vTaskYield();
  } EndCoRoutine;
}

int main(void){
  /*!
    Hardwares initialization.
  */
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
  semaphore_t smTaskFlag;
  if (ui8CreateSemaphore(&smTaskFlag, BINNARY) != SEMAPHORE_INITIALIZED){
    vSystemRestart();
  }

  /*!
    Tasks installations.
  */
  ui8AddTask(&tTask1, &ui8Task1, "Task1", &smTaskFlag, 0, 100, ENABLED);
  ui8AddTask(&tTask2, &ui8Task2, "Task2", &smTaskFlag, 0, 100, ENABLED);

  /*!
    Task manager initialization.
  */
  ui8TaskManagerInit();
  while(1){
    /*!
      Scheduling tasks.
    */
    vStartScheduler();
  }
}