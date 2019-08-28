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

#ifndef jOS_H
#define jOS_H

#ifdef __cplusplus
  extern "C" {
#endif

//! Configuration: Task Manager
/*!
  Configure the jOS system task manager here.
*/
#define   TASK_NAME_SIZE                      18
#define   AMOUNT_OF_PRIORITIES                3
#define   AMOUNT_OF_TASK_IN_A_PRIORITY        5
#define   AMOUNT_OF_TASK_SCHEDULED            20
#define   AMOUNT_OF_PRIORIZED_TASKS           4
#define   AMOUNT_OF_SEMAPHORES                3
#define   SOFTWARE_WATCHDOG_TIME              200

//! Configuration: Semaphores Manager
/*!
  Configure the jOS system semaphore manager here.
*/
#define   AMOUNT_OF_SWITCHES                  8

//! Path: EmbeddedTools
/*!
  Path to EmbeddedTools library.
*/
#include <EmbeddedTools.h>

//! Path: Kernel
/*!
  Path to jOS system MemoryManager library.
*/
#include <MemoryManager.h>

//! Path: Tools
/*!
  Path to jOS system task manager libraries.
*/
#include "TaskScheduler.h"
#include "CoRoutines.h"
#include "Switch.h"
#include "Semaphores.h"

//! Path: System
/*!
  Path to jOS system libraries.
*/
#include "System.h"

#ifdef __cplusplus
  }
#endif

#endif
