//! jOS Version 2.0b
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

#ifndef TaskScheduler_H
#define TaskScheduler_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <inttypes.h>
#include "./Configs.h"
#include "./System.h"

//! Macros: Task Satus
/*!
  This macros are for facilitate the use of this library.
*/
#define     EMPTY                               0
#define     ENABLED                             1
#define     DISABLED                            2
#define     READY                               3

//! Macros: Scheduler Satus
/*!
  This macros are for facilitate the use of this library.
*/
#define     STOPED                              0
#define     PAUSED                              1
#define     RUNNING                             2

//! Macros: Scheduler Modes
/*!
  This macros are for facilitate the use of this library.
*/
#define     ROUND_ROBIN_MODE                    0   
#define     RELATIVE_PRIORITY_MODE              1
#define     ABSOLUTE_PRIORITY_MODE              2
#define     FULL_PRIORITY_MODE                  3

//! Macros: Cooperative Mode
/*!
  This macros are for facilitate the use of this library.
*/
#define     COOPERATIVE_MODE                    0

//! Macros: Task Types
/*!
  This macros are for facilitate the use of this library.
*/
#define     TIMER_TYPE                          0
#define     COOPERATIVE_THREAD_TYPE             1

//! Macros: Thread and Timer types
/*!
  This macros are for facilitate the use of this library.
*/
#define     thread_t                            task_return_t
#define     timer_t                             task_return_t

//! Type Definition Enumeration: task_return_t
/*!
  This typedef exist for organization purpose.
*/
typedef enum{
  END = 0,
  SLEEP,
  RESTART
} task_return_t;

//! Type Definition: task_handler_t
/*!
  This typedef exist for organization purpose.
*/
typedef struct {
  task_return_t (*tTaskFunction)(void*);                                                                                                                                        /*!< Void function pointer type. */
  void* vpArguments;                                                                                                                                                            /*!< Void pointer type. */
  char cpTaskName[SIZE_OF_TASK_NAME];                                                                                                                                           /*!< Char pointer type. */
  uint8_t ui8TaskType;                                                                                                                                                          /*!< 8-bit integer type. */
  uint8_t ui8RealAddress;                                                                                                                                                       /*!< 8-bit integer type. */
  uint8_t ui8VirtualAddress;                                                                                                                                                    /*!< 8-bit integer type. */
  uint8_t ui8Priority;                                                                                                                                                          /*!< 8-bit integer type. */
  uint8_t ui8Status;                                                                                                                                                            /*!< 8-bit integer type. */
  uint16_t ui16Line;                                                                                                                                                            /*!< 16-bit integer type. */
  uint16_t ui16Period;                                                                                                                                                          /*!< 16-bit integer type. */
  uint16_t ui16TimeCounter;                                                                                                                                                     /*!< 16-bit integer type. */
} task_handler_t;

//! Macro: Timer Creator
/*!
  Create a timer type task_t.
  \param cpTaskName a char string vector. It's the task name.
  \param tTaskFunction a void function pointer. It's the task callback.
  \param vpArguments a void pointer. It's the task callback argument.
  \param ui16Period is a 16-bit unsigned integer. It's the callback repetition time.
  \param ui8Priority is a 8-bit unsigned integer. It's the task priority.
  \param ui8Status is a 8-bit unsigned integer. It's the task start status (ENABLED or DISABLED).
  \return Returns the task pointer.
*/
#define     tCreateTimer(cpTaskName, tTaskFunction, vpArguments, ui16Period, ui8Priority, ui8Status)            tCreateTask(cpTaskName, tTaskFunction, vpArguments, ui16Period, ui8Priority, ui8Status);


//! Macro: Thread Creator
/*!
  Create a thread type task_t.
  \param cpTaskName a char string vector. It's the task name.
  \param tTaskFunction a void function pointer. It's the task callback.
  \param vpArguments a void pointer. It's the task callback argument.
  \param ui8Priority is a 8-bit unsigned integer. It's the task priority.
  \param ui8Status is a 8-bit unsigned integer. It's the task start status (ENABLED or DISABLED).
  \return Returns the task pointer.
*/
#define     tCreateThread(cpTaskName, tTaskFunction, vpArguments, ui8Priority, ui8Status)                       tCreateTask(cpTaskName, tTaskFunction, vpArguments, COOPERATIVE_MODE, ui8Priority, ui8Status);

//! Type Definition: task_t
/*!
  This typedef exist for organization purpose.
*/
typedef task_handler_t* task_t;

extern task_t tCurrentTask;                                                                                                                                                     /*!< task_t type. */

task_t tCreateTask(const char* cpTaskName, task_return_t (*tTaskFunction)(void*), void* vpArguments, uint16_t ui16Period, uint8_t ui8Priority, uint8_t ui8Status);              /*!< task_t type. */
void vDeleteTask(task_t tTask);                                                                                                                                                 /*!< void type. */
void vSchedulerInterruption(void);                                                                                                                                              /*!< void type. */
void vEnableScheduler(uint8_t ui8SchedulerMode);                                                                                                                                /*!< void type. */
void vStartScheduler(void);                                                                                                                                                     /*!< void type. */
void vDisableTask(task_t tTask);                                                                                                                                                /*!< void type. */
void vEnableTask(task_t tTask);                                                                                                                                                 /*!< void type. */
void vForceTaskExecution(task_t tTask);                                                                                                                                         /*!< void type. */
void vRestartTimerTask(task_t tTask);                                                                                                                                           /*!< void type. */
void vEnableAllTasks(void);                                                                                                                                                     /*!< void type. */
void vDisableAllTasks(void);                                                                                                                                                    /*!< void type. */
void* vpGetTaskArguments(task_t tTask);                                                                                                                                         /*!< void type. */
void vChangeTaskArguments(task_t tTask, void* vpArguments);                                                                                                                     /*!< void type. */
task_t tFindTask(const char* cpTaskName);                                                                                                                                       /*!< task_t type. */
void vChangeTaskPeriod(task_t tTask, uint16_t ui16Period);                                                                                                                      /*!< void type. */
void vChangeTaskPriority(task_t tTask, uint8_t ui8Priority);                                                                                                                    /*!< void type. */

#ifdef __cplusplus
  }
#endif

#endif
