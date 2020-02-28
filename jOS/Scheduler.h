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

#ifndef __TASK_SCHEDULER_H__
#define __TASK_SCHEDULER_H__

  #include "./Configs.h"

  #if defined(__TASK_SCHEDULER_ENABLE__) &&\
  defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
  defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
  defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
  defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
  defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)

    #ifdef __cplusplus
      extern "C" {
    #endif

    #include <System.h>
    #include <stdint.h>
    #include "./Task.h"

    //! Type Enumeration: Scheduler Mode
    /*!
      These macros are for facilitate the use of this library.
    */
    typedef enum{
      SCHEDULER_MODE_ROUND_ROBIN = 0,
      SCHEDULER_MODE_RELATIVE_PRIORITY,
      SCHEDULER_MODE_ABSOLUTE_PRIORITY,
      SCHEDULER_MODE_FULL_PRIORITY
    } scheduler_mode_t;

    //! Type Enumeration: Scheduler Status
    /*!
      These macros are for facilitate the use of this library.
    */
    typedef enum {
      SCHEDULER_STATUS_STOPPED = 0,
      SCHEDULER_STATUS_PAUSED,
      SCHEDULER_STATUS_RUNNING
    } scheduler_status_t;

    extern task_t __tCurrentTask;                                                                                         /*!< task_t type. */

    void Scheduler_setMode(scheduler_mode_t smSchedulerMode);                                                             /*!< void type function. */
    void Scheduler_enable(void);                                                                                          /*!< void type function. */
    void Scheduler_start(void);                                                                                           /*!< void type function. */
    void Scheduler_enableAllTasks(void);                                                                                  /*!< void type function. */
    void Scheduler_disableAllTasks(void);                                                                                 /*!< void type function. */
    void Scheduler_attachWatchdogCallback(void (*vfWatchdogCallback)(void*), void* vpWatchdogCallbackArguments);          /*!< void type function. */
    void Scheduler_detachWatchdogCallback(void);                                                                          /*!< void type function. */

    #ifdef __cplusplus
      }
    #endif

  #endif
#endif
