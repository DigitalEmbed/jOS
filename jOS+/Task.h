//! winTerface Version 1.0b
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

#ifndef Task_Class_H
#define Task_Class_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <jOS.h>
#include <avr/pgmspace.h>

//! Macro: Thread "Object Constructor"
/*!
  "Construct" a task_t "object".
  \param cpTaskName a char string vector. It's the task name.
  \param tTaskFunction a void function pointer. It's the task callback.
  \param vpArguments a void pointer. It's the task callback argument.
  \param ui8Priority is a 8-bit unsigned integer. It's the task priority.
  \param ui8Status is a 8-bit unsigned integer. It's the task start status (ENABLED or DISABLED).
  \return Returns the object.
*/
#define     NewThread(cpTaskName, tTaskFunction, vpArguments, ui8Priority, ui8Status)                   tCreateTask(cpTaskName, tTaskFunction, vpArguments, COOPERATIVE_MODE, ui8Priority, ui8Status);

//! Macro: Task "Object Constructor"
/*!
  "Construct" a task_t "object".
  \param cpTaskName a char string vector. It's the task name.
  \param tTaskFunction a void function pointer. It's the task callback.
  \param vpArguments a void pointer. It's the task callback argument.
  \param ui16Period is a 16-bit unsigned integer. It's the callback repetition time.
  \param ui8Priority is a 8-bit unsigned integer. It's the task priority.
  \param ui8Status is a 8-bit unsigned integer. It's the task start status (ENABLED or DISABLED).
  \return Returns the object.
*/
#define     NewTimer(cpTaskName, tTaskFunction, vpArguments, ui16Period, ui8Priority, ui8Status)        tCreateTask(cpTaskName, tTaskFunction, vpArguments, ui16Period, ui8Priority, ui8Status);

//! Type Definition: task_manager_t
/*!
  This is a "class" of task_manager_t type.
*/
typedef struct{
  void (*vDelete)(task_t tTask);                                                    /*!< void "method". */
  void (*vDisable)(task_t tTask);                                                   /*!< void "method". */
  void (*vEnable)(task_t tTask);                                                    /*!< void "method". */
  void (*vForceExecution)(task_t tTask);                                            /*!< void "method". */
  void (*vRestartTimer)(task_t tTask);                                              /*!< void "method". */
  void (*vEnableAll)(void);                                                         /*!< void "method". */
  void (*vDisableAll)(void);                                                        /*!< void "method". */
  void* (*vpGetArguments)(task_t tTask);                                            /*!< void "method". */
  void (*vChangeArguments)(task_t tTask, void* vpArguments);                        /*!< void "method". */
  task_t (*tFind)(const char* cpTaskName);                                          /*!< task_t "method". */
  void (*vChangePeriod)(task_t tTask, uint16_t ui16Period);                         /*!< void "method". */
  void (*vChangePriority)(task_t tTask, uint8_t ui8Priority);                       /*!< void "method". */
} task_manager_t;

extern const task_manager_t Task PROGMEM;                                           /*!< Task manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
