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

#ifndef Semaphores_h
#define Semaphores_h

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdio.h>
#include <inttypes.h>
#include "jOS.h"

//! Macros: Semaphore Satus
/*!
  This macros are for facilitate the use of this library.
*/
#define   BINNARY                           0
#define   COUNTING                          1
#define   MUTEX                             2

#define   SEMAPHORE_IDLE                    10
#define   SEMAPHORE_BUSY                    11

#define   TAKED_SEMAPHORE                   21
#define   RETURNED_SEMAPHORE                22
#define   TASK_HOLDER                       23
#define   TASK_NOT_HOLDER                   24

#define   SEMAPHORE_INITIALIZED             40
#define   SEMAPHORE_NOT_INITIALIZED         41
#define   NO_SUCH_MEMORY_FOR_SEMAPHORE      42
#define   TASK_SCHEDULED                    43
#define   TASK_SCHEDULED_NOT_REMOVED        44
#define   MUTEX_NOT_POSSIBLE                45
#define   TASK_BUFFER_NEEDLESS              46
#define   TASK_PERIOD_CHANGE_ERROR          47
#define   TASK_PRIORITY_CHANGE_ERROR        48
#define   TASK_EXECUTION_CHANGE_ERROR       49

//! Type Definition: semaphore_t
/*!
  This typedef exist for organization purpose. This type is equivalent of a 8-bit unsigned integer.
*/
typedef struct {
  uint8_t ui8SemaphoreStatus;
  uint8_t ui8SemaphoreType;
  uint8_t ui8SemaphoreAddress;
  uint16_t ui16ResetTime;
  task_t* tpTaskHolder;
  list_t* tlpPendingTasksList;
} semaphore_t;

void vSemaphoreManagerInit(void);                                                                           /*!< void type function. */
uint8_t ui8CreateSemaphore(semaphore_t* smpSemaphore, uint8_t ui8SemaphoreType, uint16_t ui16ResetTime);    /*!< 8-bits integer type function. */
uint8_t ui8TakeSemaphore(semaphore_t* smpSemaphore);                                                        /*!< 8-bits integer type function. */
uint8_t ui8ReturnSemaphore(semaphore_t* smpSemaphore);                                                      /*!< 8-bits integer type function. */

#ifdef __cplusplus
  }
#endif

#endif
