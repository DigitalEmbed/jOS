//! jOS Version 3.0b
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

#ifndef __SEMAPHORES_H__
  #define __SEMAPHORES_H__

  #include "./Configs.h"

  #if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
  defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)

    #ifdef __cplusplus
      extern "C" {
    #endif
  
    #include <stdio.h>
    #include <inttypes.h>
    #include "./Task.h"
    #include "./Scheduler.h"

    //! Type Enumeration: Semaphore Type
    /*!
      These macros are for facilitate the use of this library.
    */
    typedef enum {
      SEMAPHORE_TYPE_BINARY = 0,
      SEMAPHORE_TYPE_COUNTING,
      SEMAPHORE_TYPE_MUTEX
    } semaphore_type_t;

    //! Type Enumeration: Semaphore Status
    /*!
      These macros are for facilitate the use of this library.
    */
    typedef enum {
      SEMAPHORE_STATUS_EMPTY = 0,
      SEMAPHORE_STATUS_CREATED,
      SEMAPHORE_STATUS_ERROR,
      SEMAPHORE_STATUS_RETURNED_SEMAPHORE,
      SEMAPHORE_STATUS_TASK_HOLDER,
      SEMAPHORE_STATUS_IDLE,
      SEMAPHORE_STATUS_BUSY
    } semaphore_status_t;

    //! Type Definition: Semaphore Structure
    /*!
      This typedef exist for organization purpose.
    */
    typedef struct {
      semaphore_status_t smsSemaphoreStatus;
      semaphore_type_t smtSemaphoreType;
      uint16_t ui16Timeout;
      uint16_t ui16TimeCounter;
      task_t tTaskHolder;
    } semaphore_structure_t;

    typedef semaphore_structure_t* semaphore_t;

    semaphore_status_t newSemaphore(semaphore_t* smSemaphore, semaphore_type_t smtSemaphoreType, uint16_t ui16Timeout);     /*!< semaphore_status_t type function. */
    semaphore_status_t Semaphore_take(semaphore_t smSemaphore);                                                             /*!< semaphore_status_t type function. */
    semaphore_status_t Semaphore_return(semaphore_t smSemaphore);                                                           /*!< semaphore_status_t type function. */
    void Semaphore_initWatchdog(void);                                                                                      /*!< void type function. */
    void Semaphore_attachWatchdogCallback(void (*vfWatchdogCallback)(void*), void* vpWatchdogCallbackArguments);            /*!< void type function. */
    void Semaphore_detachWatchdogCallback(void);                                                                            /*!< void type function. */

    #ifdef __cplusplus
      }
    #endif

  #endif  
#endif
