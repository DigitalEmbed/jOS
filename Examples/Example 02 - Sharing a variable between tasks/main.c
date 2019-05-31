//! Example 02 - Sharing a variable between tasks
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

#include <jOS.h>
#include <PWM.h>
#include <ADC.h>

#define PREVIOUS  0
#define ACTUAL    1

/*!
  Tasks declarations.
*/
task_t tAnalogRead;                                                     // Task handle declaration.
uint8_t ui8AnalogRead(void *vpArgs){                                    // All task functions must have this scope of function.
  if (vpArgs != NULL){                                                  // Checking whether the argument exists
    static uint16_t* ui16pAnalogValue = (uint16_t*) vpArgs;
    ui16pAnalogValue[PREVIOUS] = ui16pAnalogValue[ACTUAL];
    ui16pAnalogValue[ACTUAL] = ui16AnalogRead(5);
    if (ui16pAnalogValue[PREVIOUS] != ui16pAnalogValue[ACTUAL]){
      if (ui8EnableTask(tpFindTask("GeneratePWM")) != TASK_ENABLED){    // "GeneratePWM" task enabling...
        return SYSTEM_RESTART;                                          // If "GeneratePWM" task not exist, restart the system.
      }
    }
  }
  return TASK_END;                                                      // The system will sleep until the next instruction.
}

task_t tGeneratePWM;                                                    // Task handle declaration.
uint8_t ui8GeneratePWM(void* vpArgs){                                   // All task functions must have this scope of function.
  if (vpArgs != NULL){                                                  // Checking whether the argument exists
    static uint16_t* ui16pPWMValue = (uint16_t*) vpArgs;
    vGeneratePWM(6, ui16pPWMValue[ACTUAL]);
  }
  if (ui8DisableTask(tpGetCurrentTask()) != TASK_DISABLED){
    return SYSTEM_RESTART;                                              // Restart the system if is not possible self disable...
  }
  return TASK_END;                                                      // The system will sleep until the next instruction.
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
  vSetPinMode(5, ANALOG_INPUT);
  vSetPinMode(6, PWM);

  /*!
    Variables allocation.
  */
  uint16_t ui16pSharedVariable[2] = {0};

  /*!
    Tasks installations.
  */
  if (ui8AddTask(&tAnalogRead, &ui8AnalogRead, "AnalogRead", &ui16pSharedVariable, 1, 100, ABLED) != TASK_ADDED){
    vSystemRestart();
  }  
  if (ui8AddTask(&tGeneratePWM, &ui8GeneratePWM, "GeneratePWM", &ui16pSharedVariable, 1, 50, UNABLED) != TASK_ADDED){
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
