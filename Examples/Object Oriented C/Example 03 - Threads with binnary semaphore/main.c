//! Example 03 - Threads with binnary semaphore
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
#include <jOS+.h>

#define vSerialBegin(ui32BaudRate)
#define vSendSerial(sString)

/*!
  tTask1 task declaration and implementation.
*/
task_t tTask1;
void vTask1(void* vpArgs) THREAD{
  /*!
    Pointing out to smTaskFlag semaphore.
  */
  static semaphore_t smTaskFlag = (semaphore_t) vpArgs;

  /*!
    Starting co-routine.
  */
  thread_loop {
    /*!
      Waiting for "semaphore".
    */
    Task.waitFor(Semaphore.take(smTaskFlag) == SEMAPHORE_STATUS_TASK_HOLDER);

    /*!
      Executing the function when "semaphore open".
    */
    vSendSerial("Processing A");
    vSendSerial("Processing B");
    vSendSerial("Processing C");

    /*!
      Returning "semaphore".
    */
    Semaphore.giveBack(smTaskFlag);

    /*!
      Pausing the task.
    */
    Task.yield();
  }
} END_THREAD

/*!
  tTask2 task declaration and implementation (Very similar to tTask1).
*/
task_t tTask2;
void vTask2(void* vpArgs) THREAD{
  static semaphore_t smTaskFlag = (semaphore_t) vpArgs;
  thread_loop {
    Task.waitFor(Semaphore.take(smTaskFlag) == SEMAPHORE_STATUS_TASK_HOLDER);
    vSendSerial("Executing A");
    vSendSerial("Executing B");
    vSendSerial("Executing C");
    Semaphore.giveBack(smTaskFlag);
    Task.yield();
  };
} END_THREAD

int main(void){
  /*!
    Hardwares initialization.
  */
  vSerialBegin(9600);

  /*!
    tTaskFlag switch declaration.
  */
  semaphore_t smTaskFlag;
  newSemaphore(
    smTaskFlag,                   // Semaphore handler. 
    SEMAPHORE_TYPE_BINARY,        // Semaphore type.
    5000                          // Watchdog timeout.
  );

  /*!
    Tasks installations.
  */
  newThread(tTask1, "Task1", &vTask1, smTaskFlag, 100, 1, TASK_STATUS_ENABLED);
  newThread(tTask2, "Task2", &vTask2, smTaskFlag, 100, 1, TASK_STATUS_ENABLED);

  #if !(defined(__AUTO_INITIALIZATION_ENABLE__))

    /*!
      Scheduler initialization.
    */
    Scheduler.setRoundRobinMode();
    Scheduler.start();

  #endif
}
