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

#ifndef __SCHEDULER_CLASS_H__
  #define __SCHEDULER_CLASS_H__

  #include <jOS.h>

  #if defined(__TASK_SCHEDULER_ENABLE__) &&\
  defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
  defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
  defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
  defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
  defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)

    #ifdef __cplusplus
      extern "C" {
    #endif

    #if defined(__AVR__)
      #include <avr/pgmspace.h>
    #endif

    typedef struct {
      void (*attachCallback)(void (*vfWatchdogCallback)(void*), void* vpWatchdogCallbackArguments);       /*!< void "method". */
      void (*detachCallback)(void);                                                                       /*!< void "method". */
    } watchdog_scheduler_manager_t;

    typedef struct {
      watchdog_scheduler_manager_t Tasks;

      #if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
      defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)
        watchdog_scheduler_manager_t Semaphores;
      #endif

    } watchdog_object_t;

    //! Type Definition: scheduler_manager_t
    /*!
      This is a "class" of scheduler_manager_t type.
    */
    typedef struct {
      void (*setRoundRobinMode)(void);                                                                    /*!< void "method". */
      void (*setRelativeMode)(void);                                                                      /*!< void "method". */
      void (*setAbsoluteMode)(void);                                                                      /*!< void "method". */
      void (*setFullAbsoluteMode)(void);                                                                  /*!< void "method". */
      void (*start)(void);                                                                                /*!< void "method". */
      watchdog_object_t Watchdog;                                                                         /*!< watchdog_scheduler_manager_t "object". */
    } scheduler_manager_t;

    #if defined(__AVR__)
      extern const scheduler_manager_t Scheduler PROGMEM;                                                 /*!< Scheduler manager "object". */
    #else
      extern const scheduler_manager_t Scheduler;                                                         /*!< Scheduler manager "object". */
    #endif

    #ifdef __cplusplus
      }
    #endif

  #endif
#endif
