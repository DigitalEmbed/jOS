//! Example 01 - Blink a LED with Thread
/*!
  This simply example demonstrates the use of task manager.

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

/*!
  Tasks declarations.
*/
task_t tBlink;                                      // Task handle declaration.
void vBlink(void* vpArgs) THREAD {                  // All task functions must have this scope of function.
  static bool bLEDState = GPIO_OUTPUT_LOW;          // Static variables never will be deleted! In a task, statics variables is almost mandatory.
  vGenerateLogicLevel(13, bLEDState);
  bLEDState = !bLEDState;
  Task_delayMS(1000);                               // Pause this task for 1000ms.
} END_THREAD

/*!
  Main function.
*/
int main(){

  /*!
    Hardwares initialization.
  */
  vSetPinMode(13, GPIO_MODE_OUTPUT);

  /*!
    Tasks installations.
  */
  task_status_t tsTaskStatus = newThread(
    tBlink,                 // This parameter defines the handle of this task.
    "Blink",                // This parameter defines the name of this task.
    &vBlink,                // This parameter defines the function of this task.
    NULL,                   // This parameter defines the arguments of this task.
    500,                    // This parameter defines the software watchdog timeout.
    1,                      // This parameter defines the priority of this task.
    TASK_MODE_REPEAT,       // This parameter defines if this task will run once a time or repeatedly.
    TASK_STATUS_ENABLED     // This parameter defines whether this task will start enabled or not.
  );

  if (tsTaskStatus != TASK_STATUS_CREATED){
    System_restart();
  }
  
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
