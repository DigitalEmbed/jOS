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

#ifndef TaskScheduler_H
#define TaskScheduler_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <inttypes.h>

//! Macros: Task Manager Initializer Status
/*!
  This macros leave the code more cleaner and understandable.
*/
#define     TASK_MANAGER_INITIALIZED            0
#define     BUFFER_MANAGER_ERROR                1
#define     TASK_SCHEDULED_BUFFER_ERROR         2

//! Macros: Task Manipulation Status
/*!
  This macros leave the code more cleaner and understandable.
*/
#define     EXISTING_TASK                       0
#define     TASK_ADDED                          1
#define     TASK_REMOVED                        2
#define     TASK_ENABLED                        3
#define     TASK_DISABLED                       4
#define     TASK_PERIOD_CHANGED                 5
#define     TASK_PRIORITY_CHANGED               6
#define     TASK_ARGUMENT_CHANGED               7
#define     TASK_EXECUTION_CHANGED              8
#define     TASK_PRIORIZED                      9
#define     CURRENT_TASK                        10
#define     TASK_TIMER_RESTARTED                11
#define     TASK_DEPRIVED                       12
#define     TASK_CREATED                        100

#define     ERROR_PRIORITY_FULL                 10
#define     ERROR_TASK_NOT_ADDED                11
#define     ERROR_TASK_NOT_REMOVED              12
#define     ERROR_TASK_NOT_ENABLED              13
#define     ERROR_TASK_NOT_DISABLED             14
#define     ERROR_TASK_PERIOD_NOT_CHANGED       15
#define     ERROR_TASK_PRIORITY_NOT_CHANGED     16
#define     ERROR_TASK_ARGUMENT_NOT_CHANGED     17
#define     ERROR_TASK_EXECUTION_NOT_CHANGED    18
#define     ERROR_TASK_NOT_PRIORIZED            19
#define     NOT_CURRENT_TASK                    20
#define     TASK_TIMER_NOT_RESTARTED            21
#define     ERROR_TASK_NOT_DEPRIVED             22
#define     NO_SUCH_MEMORY                      106

//! Macros: Task Status
/*!
  These macros are used in task returns. For each return, the operating system will behave - and this is editable in the vCheckTaskReturn(uint8_t ui8TaskReturn) function.
*/
#define     TASK_END                            1
#define     INSUFICIENT_MEMORY                  2
#define     INVALID_ARGUMENTS                   3
#define     NO_SLEEP                            4
#define     SYSTEM_RESTART                      5

//! Macros: Task Enabling Status
/*!
  This macros leave the code more cleaner and understandable.
*/
#define     DISABLED                            0
#define     ENABLED                             1
#define     PRIORIZED                           2
#define     NOT_INSTALLED                       3

//! Type Definition: pfunc_t
/*!
  This typedef is equivalent to a uint8_t function pointer with pointer void argument.
*/
typedef uint8_t (*pfunc_t)(void*);

//! Type Definition: task_t struct.
/*!
  This struct creates a task.
*/
typedef struct {
  pfunc_t pfFunction;                           /*!< Void function pointer type. */
  char* cpTaskName;                             /*!< Char pointer type. */
  void* vpArguments;                            /*!< Void pointer type. */
  uint8_t ui8Priority;                          /*!< 8-bit integer type. */
  uint16_t ui16Period;                          /*!< 16-bit integer type. */
  uint8_t ui8Status;                            /*!< 8-bit integer type. */
  uint8_t ui8TaskAddress;                       /*!< 8-bit integer type. */
  uint16_t ui16Line;                            /*!< 16-bit integer type. */
}task_t;

uint8_t ui8TaskManagerInit(void);                                                                                                                                     	/*!< 8-bit integer type function. */
uint8_t ui8AddTask(task_t* tpTask, pfunc_t pfFunction, const char* cpTaskName, void* vpArguments, uint8_t ui8Priority, uint16_t ui16Period, uint8_t ui8Status);     		/*!< 8-bit integer type function. */
uint8_t ui8RemoveTask(task_t* tpTask);                                                                                                                                	/*!< 8-bit integer type function. */
uint8_t ui8EnableTask(task_t* tpTask);                                                                                                                                	/*!< 8-bit integer type function. */
uint8_t ui8DisableTask(task_t* tpTask);                                                                                                                               	/*!< 8-bit integer type function. */
uint8_t ui8PriorizeTask(task_t* tpTask);                                                                                                                                /*!< 8-bit integer type function. */
uint8_t ui8DepriveTask(task_t* tpTask);                                                                                                                                 /*!< 8-bit integer type function. */
uint8_t ui8RestartTimerTask(task_t* tpTask);																																																														/*!< 8-bit integer type function. */
uint8_t ui8ChangeTaskPeriod(task_t* tpTask, uint16_t ui16Period);                                                                                                     	/*!< 8-bit integer type function. */
uint8_t ui8RestoreTaskPeriod(task_t* tpTask);                                                                                                                           /*!< 8-bit integer type function. */
uint8_t ui8ChangeTaskArgument(task_t* tpTask, void* vpArgument);                                                                                                      	/*!< 8-bit integer type function. */
task_t* tpGetCurrentTask(void);                                                                                                                                       	/*!< task_t pointer type function. */
task_t* tpFindTask(const char* cpTaskName);                                                                                                                           	/*!< task_t pointer type function. */
void* vpGetArguments(task_t* tpTask);                                                                                                                                 	/*!< void pointer type function. */
void vEnableAllTasks(void);                                                                                                                                           	/*!< void type function. */
void vDisableAllTasks(void);                                                                                                                                           	/*!< void type function. */
void vStartScheduler(void);                                                                                                                                           	/*!< void type function. */

#ifdef __cplusplus
  }
#endif

#endif
