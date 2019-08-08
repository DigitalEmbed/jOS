//! jOS Version 1.0b
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

#ifndef CoRoutines_H
#define CoRoutines_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "TaskScheduler.h"

#define   COUNTER_OVERFLOW            1
#define   COUNTER_RUNNING             0

#define   CoRoutine                   static task_t* tpCurrentTask = tpGetCurrentTask();\
                                      static uint8_t ui8TaskYield = 1;\
                                      if (ui8TaskYield != 1){\
                                        return SYSTEM_RESTART;\
                                      }\
                                      ui8TaskYield = 0;\
                                      switch(tpCurrentTask->ui16Line){\
                                        case 0:

#define   EndCoRoutine                }\
                                      ui8TaskYield = 1;\
                                      tpCurrentTask->ui16Line = 0;\
                                      return TASK_END;

#define   vTaskYield()                tpCurrentTask->ui16Line = __LINE__ ; ui8TaskYield = 1 ; return TASK_END ; case __LINE__: ui8TaskYield = 0;

#define   vWaitUntil(xCondition)      tpCurrentTask->ui16Line = __LINE__ ; case __LINE__: if ((xCondition)) {ui8TaskYield = 1 ; return TASK_END;} ui8TaskYield = 0;

#define   vWaitFor(xCondition)        tpCurrentTask->ui16Line = __LINE__ ; case __LINE__: if (!(xCondition)) {ui8TaskYield = 1 ; return TASK_END;} ui8TaskYield = 0;

#define   vTaskDelay(ui16Time)        ui8ChangeTaskPeriod(tpGetCurrentTask(), ui16Time) , tpCurrentTask->ui16Line = __LINE__ ; ui8TaskYield = 1 ; return TASK_END; case __LINE__: ui8RestoreTaskPeriod(tpGetCurrentTask()); ui8TaskYield = 0;

#define   CooperativeMode             {CoRoutine

#define   EndCooperativeMode          EndCoRoutine}

#ifdef __cplusplus
  }
#endif

#endif
