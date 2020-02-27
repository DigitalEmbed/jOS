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

#ifndef __TASK_H__
#define __TASK_H__

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

    #include <inttypes.h>

    //! Type Enumeration: Task Type
    /*!
      These macros are for facilitate the use of this library.
    */
    typedef enum {
      TASK_TYPE_TIMER = 0,
      TASK_TYPE_COOPERATIVE_THREAD
    } task_type_t;

    //! Type Enumeration: Task Satus
    /*!
      These macros are for facilitate the use of this library.
    */
    typedef enum {
      TASK_STATUS_EMPTY = 0,
      TASK_STATUS_ENABLED,
      TASK_STATUS_DISABLED,
      TASK_STATUS_READY,
      TASK_STATUS_RUNNING,
      TASK_STATUS_CREATED,
      TASK_STATUS_ERROR
    } task_status_t;

    //! Macros: Cooperative Mode
    /*!
      This macros is for facilitate the use of this library.
    */
    #define COOPERATIVE_MODE 0

    //! Type Definition: task_handler_t
    /*!
      This typedef exist for organization purpose.
    */
    typedef struct {
      void (*vfTaskFunction)(void*);                                                                                                                                                /*!< void function pointer type. */
      void* vpArguments;                                                                                                                                                            /*!< void pointer type. */
      char cpTaskName[__TASK_NAME_SIZE__];                                                                                                                                          /*!< char pointer type. */
      task_type_t ttTaskType;                                                                                                                                                       /*!< 8-bit integer type. */
      task_status_t tsStatus;                                                                                                                                                       /*!< 8-bit integer type. */
      uint8_t ui8RealAddress;                                                                                                                                                       /*!< 8-bit integer type. */
      uint8_t ui8VirtualAddress;                                                                                                                                                    /*!< 8-bit integer type. */
      uint8_t ui8Priority;                                                                                                                                                          /*!< 8-bit integer type. */
      uint16_t ui16StartLine;                                                                                                                                                       /*!< 16-bit integer type. */
      uint16_t ui16Line;                                                                                                                                                            /*!< 16-bit integer type. */
      uint16_t ui16Period;                                                                                                                                                          /*!< 16-bit integer type. */
      uint16_t ui16Timeout;                                                                                                                                                         /*!< 16-bit integer type. */
      uint16_t ui16TimeCounter;                                                                                                                                                     /*!< 16-bit integer type. */
    } task_handler_t;

    //! Type Definition: task_t
    /*!
      This typedef exist for organization purpose.
    */
    typedef task_handler_t* task_t;

    //! Macro: Thread Creator
    /*!
      Creates a thread and add it into task manager.
      \param tTask is a task_t type.
      \param cpTaskName a char string vector. It's the task name.
      \param vfTaskFunction a void function pointer. It's the task callback.
      \param vpArguments a void pointer. It's the task callback argument.
      \param ui8Priority is a 8-bit unsigned integer. It's the task priority.
      \param tsStatus is a 8-bit unsigned integer. It's the task start status (ENABLED or DISABLED).
      \return Returns the object.
    */
    #define newThread(tTask, cpTaskName, vfTaskFunction, vpArguments, ui16Timeout, ui8Priority, tsStatus)\
      newTask(&tTask, cpTaskName, vfTaskFunction, vpArguments, COOPERATIVE_MODE, ui16Timeout, ui8Priority, tsStatus);

    //! Macro: Timer Creator
    /*!
      Creates a timer and add it into task manager.
      \param tTask is a task_t type.
      \param cpTaskName a char string vector. It's the task name.
      \param vfTaskFunction a void function pointer. It's the task callback.
      \param vpArguments a void pointer. It's the task callback argument.
      \param ui16Period is a 16-bit unsigned integer. It's the callback repetition time.
      \param ui8Priority is a 8-bit unsigned integer. It's the task priority.
      \param tsStatus is a 8-bit unsigned integer. It's the task start status (ENABLED or DISABLED).
      \return Returns the object.
    */
    #define newTimer(tTask, cpTaskName, vfTaskFunction, vpArguments, ui16Period, ui16Timeout, ui8Priority, tsStatus)\
      newTask(&tTask, cpTaskName, vfTaskFunction, vpArguments, ui16Period, ui16Timeout, ui8Priority, tsStatus);

    task_status_t newTask(task_t* tTask, const char* cpTaskName, void (*vfTaskFunction)(void*), void* vpArguments, uint16_t ui16Period, uint16_t ui16Timeout, uint8_t ui8Priority, task_status_t tsStatus);       /*!< task_t type. */
    task_t Task_find(const char* cpTaskName);                                                                                                                                                                     /*!< task_t type. */
    void Task_disable(task_t tTask);                                                                                                                                                                              /*!< void type. */
    void Task_enable(task_t tTask);                                                                                                                                                                               /*!< void type. */
    void Task_forceExecution(task_t tTask);                                                                                                                                                                       /*!< void type. */
    void Task_restartTimer(task_t tTask);                                                                                                                                                                         /*!< void type. */
    void Task_setArguments(task_t tTask, void* vpArguments);                                                                                                                                                      /*!< void type. */
    void Task_setPeriod(task_t tTask, uint16_t ui16Period);                                                                                                                                                       /*!< void type. */
    void Task_setPriority(task_t tTask, uint8_t ui8Priority);                                                                                                                                                     /*!< void type. */
    void* Task_getArguments(task_t tTask);                                                                                                                                                                        /*!< void type. */

    #if defined(__TASK_DESTRUCTOR_ENABLE__)
      void deleteTask(task_t tTask);                                                                                                                                                                              /*!< void type. */
    #endif

    #ifdef __cplusplus
      }
    #endif

  #endif
#endif
