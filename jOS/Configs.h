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

#ifndef __JOS_CONFIGS_H__
  #define __JOS_CONFIGS_H__

  #ifdef __cplusplus
    extern "C" {
  #endif

  //! Configuration: HAL
  /*!
    Set the hal system here.
    Availables:
    - NoMicrocontroller
    - ArduinoAVR
    - HALru
    - ESP12
  */
  #define ArduinoAVR

  //! Configuration: RTOS Auto Initialization
  /*!
    Enable or disable RTOS auto initialization here. All RTOS configurations functions are called after main(), and the scheduler function are called before main(). Some compilers don't support this.
      - __AUTO_INITIALIZATION_ENABLE__
      - __AUTO_INITIALIZATION_DISABLE__
  */
  #define __AUTO_INITIALIZATION_ENABLE__

  //! Configuration: Task Manager
  /*!
    Configure the jOS system task manager here. The available values for this macro are:
      - __TASK_SCHEDULER_ENABLE__
      - __TASK_SCHEDULER_DISABLE__
  */
  #define __TASK_SCHEDULER_ENABLE__

  #if defined(__TASK_SCHEDULER_ENABLE__)

    //! Configuration: Task Destructor
    /*!
      Enable or disable task destructor here. Sometimes it may not be a good idea to keep destroying tasks ... But if you need to, just change this macro.
        - __TASK_DESTRUCTOR_ENABLE__
        - __TASK_DESTRUCTOR_DISABLE__
    */
    #define __TASK_DESTRUCTOR_ENABLE__

    //! Configuration: Amount of Tasks
    /*!
      It's amount of task that can be installed on task scheduler.
    */
    #define __AMOUNT_OF_TASKS__ 3

    //! Configuration: Task Name Size
    /*!
      It is the number of characters that the task name can contain.
    */
    #define __TASK_NAME_SIZE__ 5

    //! Configuration: Minimum Thread Priority
    /*!
      All installed threads have a minimum priority to ensure the priority of timers.
    */
    #define __MINIMUM_THREAD_PRIORITY__ 100

    //! Configuration: Minimum Task Timout
    /*!
      Minimum software watchdog timeout for a task.
    */
    #define __MINIMUM_TASK_TIMEOUT_MS__ 100

    //! Configuration: Scheduler Default Mode
    /*!
      This macro defines the default mode for scheduler.
    */
    #define __SCHEDULER_DEFAULT_MODE__ SCHEDULER_MODE_ROUND_ROBIN
  #endif

  //! Configuration: Switches Manager
  /*!
    Configure the switch system task manager here. The available values for this macro are:
      - __SWITCHES_MANAGER_ENABLE__
      - __SWITCHES_MANAGER_DISABLE__
  */
  #define __SWITCHES_MANAGER_ENABLE__

  #if defined(__SWITCHES_MANAGER_ENABLE__)

    //! Configuration: Amount of Switches
    /*!
      It's amount of switches that can be installed on switch manager.
    */
    #define __AMOUNT_OF_SWITCHES__ 8

  #endif

  //! Configuration: Semaphores Manager
  /*!
    Configure the jOS system semaphore manager here. The available values for this macro are:
      - __SEMAPHORES_MANAGER_ENABLE__
      - __SEMAPHORES_MANAGER_DISABLE__
  */
  #define __SEMAPHORES_MANAGER_ENABLE__

  #if defined(__SEMAPHORES_MANAGER_ENABLE__)

    //! Configuration: Amount of Semaphores
    /*!
      It's amount of semaphores that can be installed on semaphore manager.
    */
    #define __AMOUNT_OF_SEMAPHORES__ 3

    //! Configuration: Minimum Semaphore Timout
    /*!
      Minimum software watchdog timeout for a semaphore.
    */
    #define __MINIMUM_SEMAPHORE_TIMEOUT_MS__ 100

  #endif

  #ifdef __cplusplus
    }
  #endif

#endif
