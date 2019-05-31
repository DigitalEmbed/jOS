//! Example 01 - Blink a LED
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

#include <jOS.h>
#include <GPIO.h>

/*!
  Tasks declarations.
*/
task_t tBlink;                                              // Task handle declaration.
uint8_t ui8Blink(void* vpArgs){                             // All task functions must have this scope of function.
  static uint8_t ui8LEDState = 0;                           // Static variables never will be deleted!
  if (ui8LEDState == 0){
    ui8LEDState = 1;
    vGenerateLogicLevel(4, HIGH);
  }
  else{
    ui8LEDState = 0;
    vGenerateLogicLevel(4, LOW);
  }
  return TASK_END;                                          // All tasks must returns anything. See the vCheckTaskReturn() function in the TaskScheduler.c file in the jOS/TaskManager/TaskScheduler directory to include more task returns.
}

/*!
  Main function.
*/
int main(){
  /*!
    Memory manager initialization.
  */
  if (ui8MemoryManagerInit() != MEMORY_MANAGER_INITIALIZED){
    vSystemRestart();
  }
  
  /*!
    Hardwares initialization.
  */
  vSetPinMode(4, DIGITAL_OUTPUT);

  /*!
    Tasks installations.
  */
  uint8_t ui8TaskStatus = ui8AddTask(&tBlink,               // This parameter defines the handle of this task.
                                     &ui8Blink,             // This parameter defines the function of this task.
                                     "Blink",               // This parameter defines the name of this task.
                                     NULL,                  // This parameter defines the arguments of this task.
                                     1,                     // This parameter defines the priority of this task.
                                     500,                   // This parameter defines the reexecution time of this task.
                                     ABLED);                // This parameter defines whether this task will start enabled or not.
  
  if (ui8TaskStatus != TASK_ADDED){
    vSystemRestart();
  }
   
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
