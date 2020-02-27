//! Example 03 - Sharing a variable between tasks
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

#define ui16AnalogRead(ui8Pin) 0
#define vGeneratePWM(ui8Pin, ui8Value)

#define PREVIOUS  0
#define ACTUAL    1

/*!
  Tasks declarations.
*/
task_t tAnalogRead;                                                     // Task handle declaration.
void vAnalogRead(void *vpArgs){                                         // All task functions must have this scope of function.
  if (vpArgs != NULL){                                                  // Checking whether the argument exists
    static uint16_t* ui16pAnalogValue = (uint16_t*) vpArgs;
    ui16pAnalogValue[PREVIOUS] = ui16pAnalogValue[ACTUAL];
    ui16pAnalogValue[ACTUAL] = ui16AnalogRead(0);
    if (ui16pAnalogValue[PREVIOUS] != ui16pAnalogValue[ACTUAL]){
      Task_enable(Task_find("PWM"));                                    // "GeneratePWM" task enabling...
    }
  }
}

task_t tUpdatePWM;                                                      // Task handle declaration.
void vUpdatePWM(void* vpArgs){                                          // All task functions must have this scope of function.
  if (vpArgs != NULL){                                                  // Checking whether the argument exists
    static uint16_t* ui16pPWMValue = (uint16_t*) vpArgs;
    vGeneratePWM(6, (ui16pPWMValue[ACTUAL] >> 2));                      
  }
}

/*!
  Main function.
*/
int main(){

  /*!
    Variables allocation.
  */
  uint16_t ui16pSharedVariable[2] = {0};

  /*!
    Tasks installation.
  */
  newTimer(tAnalogRead, "ADC", &vAnalogRead, &ui16pSharedVariable, 100, 500, 2, TASK_MODE_REPEAT, TASK_STATUS_ENABLED);
  newTimer(tUpdatePWM, "PWM", &vUpdatePWM, &ui16pSharedVariable, 10, 100, 1, TASK_MODE_RUN_ONCE, TASK_STATUS_ENABLED);

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
