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

#ifndef Task_Class_H
#define Task_Class_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <jOS.h>

#if defined (__AVR)
  #include <avr/pgmspace.h>
#endif

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
} task_manager_t;

#if defined(__AVR)
  extern const task_manager_t Task PROGMEM;                                         /*!< Task manager "object". */
#else
  extern const task_manager_t Task;                                                 /*!< Task manager "object". */
#endif

#ifdef __cplusplus
  }
#endif

#endif
