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

#include "./Configs.h"

//! System Message: Auto Initialization Macro Status
/*!
  Please don't modify this.
*/
#if defined(__AUTO_INITIALIZATION_ENABLE__)
  #pragma message "Auto initialization enabled!"
#endif

//! System Message: Task Scheduler Macro Status
/*!
  Please don't modify this.
*/
#if !defined(__TASK_SCHEDULER_ENABLE__)
  #pragma message "Task scheduler disabled!"
#endif

//! System Message: Task Destructor Macro Status
/*!
  Please don't modify this.
*/
#if defined(__TASK_DESTRUCTOR_ENABLE__)
  #pragma message "Task destructor enabled!"
#endif

//! System Message: Semaphore Macro Status
/*!
  Please don't modify this.
*/
#if !defined(__SEMAPHORES_MANAGER_ENABLE__)
  #pragma message "Semaphores manager disabled!"
#endif

//! System Message: Semaphore Macro Status
/*!
  Please don't modify this.
*/
#if !defined(__SWITCHES_MANAGER_ENABLE__)
  #pragma message "Switches manager disabled!"
#endif

//! System Message: Semaphore Errors
/*!
  Please don't modify this.
*/
#if (!(defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
  defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)))
  #error Invalid semaphores macros values! Please check __MINIMUM_SEMAPHORE_TIMEOUT_MS__ and __AMOUNT_OF_SEMAPHORES__ macros values.
#endif

//! System Message: Switches Errors
/*!
  Please don't modify this.
*/
#if (!(defined(__SWITCHES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SWITCHES__) && (__AMOUNT_OF_SWITCHES__ > 0)))
  #error Invalid switches macro values! Please check __AMOUNT_OF_SWITCHES__ macro value.
#endif

//! System Message: Tasks Errors
/*!
  Please don't modify this.
*/
#if (!(defined(__TASK_SCHEDULER_ENABLE__) &&\
defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)))
  #error Invalid task macros values! Please check __AMOUNT_OF_TASKS__, __TASK_NAME_SIZE__, __MINIMUM_THREAD_PRIORITY__, __MINIMUM_TASK_TIMEOUT_MS__ and __SCHEDULER_DEFAULT_MODE__ macros values.
#endif