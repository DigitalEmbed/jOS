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

#ifndef __TASK_CLASS_H__
  #define __TASK_CLASS_H__

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

    #include <stdbool.h>

    #if defined (__AVR__)
      #include <avr/pgmspace.h>
    #endif

    //! Macro: Coroutine/Thread Pauser
    /*!
      This macro pauses a coroutine block/thread in a void function in a time period.
      \param ui16TimeMS is a 16-bits unsigned integer. It's the time period that the function will be paused.
    */
    #define delayMS(ui16TimeMS)\
      delayMS(0);\
      Task_delayMS(ui16TimeMS)

    //! Macro: Coroutine/Thread Pauser
    /*!
      This macro pauses a coroutine block/thread in a void function until a condition isn't satisfied.
      \param xCondition is the condition that will be verified.
    */
    #define waitFor(xCondition)\
      waitFor(false);\
      Task_waitFor(xCondition)

    //! Macro: Coroutine/Thread Pauser
    /*!
      This macro pauses a coroutine block/thread in a void function until a condition is satisfied.
      \param xCondition is the condition that will be verified.
    */
    #define waitUntil(xCondition)\
      waitUntil(false);\
      Task_waitUntil(xCondition)

    //! Macro: Coroutine/Thread Context Changer
    /*!
      This macro pauses a coroutine block/thread in a void function.
    */
    #define changeContext()\
      yield();\
      Task_yield()

    //! Type Definition: task_manager_t
    /*!
      This is a "class" of task_manager_t type.
    */
    typedef struct{
      void (*disable)(task_t tTask);                                                    /*!< void "method". */
      void (*enable)(task_t tTask);                                                     /*!< void "method". */
      void (*forceExecution)(task_t tTask);                                             /*!< void "method". */
      void (*restartTimer)(task_t tTask);                                               /*!< void "method". */
      void (*enableAll)(void);                                                          /*!< void "method". */
      void (*disableAll)(void);                                                         /*!< void "method". */
      void* (*getArguments)(task_t tTask);                                              /*!< void "method". */
      void (*setArguments)(task_t tTask, void* vpArguments);                            /*!< void "method". */
      task_t (*find)(const char* cpTaskName);                                           /*!< task_t "method". */
      void (*setPeriod)(task_t tTask, uint16_t ui16Period);                             /*!< void "method". */
      void (*setPriority)(task_t tTask, uint8_t ui8Priority);                           /*!< void "method". */
      void (*delayMS)(uint16_t ui16TimeMS);												                      /*!< void "method". */
      void (*waitFor)(bool bCondition);													                        /*!< void "method". */
      void (*waitUntil)(bool bCondition);												                        /*!< void "method". */
      void (*yield)(void);																                              /*!< void "method". */
    } task_manager_t;

    #if defined(__AVR__)
      extern const task_manager_t Task PROGMEM;                                         /*!< Task manager "object". */
    #else
      extern const task_manager_t Task;                                                 /*!< Task manager "object". */
    #endif

    #ifdef __cplusplus
      }
    #endif

  #endif
#endif
