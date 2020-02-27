#include "./Configs.h"

#if defined(__TASK_SCHEDULER_ENABLE__) &&\
defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)

  #include "./Scheduler.h"
  #include <string.h>
  #include <EmbeddedTools.h>

  task_handler_t __thCreatedTasks[__AMOUNT_OF_TASKS__] = {{0}};                       /*!< task_handler_t type. */
  task_handler_t* __thpBufferedTasks[__AMOUNT_OF_TASKS__] = {NULL};                   /*!< task_handler_t pointer type. */
  scheduler_status_t __ssSchedulerStatus = 0;                                         /*!< scheduler_status_t type. */
  scheduler_mode_t __smSchedulerMode = __SCHEDULER_DEFAULT_MODE__;                    /*!< scheduler_mode_t type. */
  uint8_t __ui8CreatedTaskCounter = 0;                                                /*!< uint8_t type. */
  uint8_t __ui8EmptyTaskPosition = 0;                                                 /*!< uint8_t type. */
  uint8_t __ui8ReadyTaskCounter = 0;                                                  /*!< uint8_t type. */
  uint8_t ui8HighestPriority = 255;                                                   /*!< uint8_t type. */
  uint8_t __ui8HighestThreadPriority = 255;                                           /*!< uint8_t type. */
  task_t __tCurrentTask = NULL;                                                       /*!< task_t type. */
  static void (*__vfWatchdogCallback)(void* vpArguments) = NULL;                      /*!< void function pointer type. */
  static void* __vpWatchdogCallbackArguments = NULL;                                  /*!< void pointer type. */

  #if defined (__AUTO_INITIALIZATION_ENABLE__)
    void __Scheduler_autoEnableScheduler(void) ATTRIBUTE_CONSTRUCTOR(255);            /*!< void type. */
    void __Scheduler_autoStart(void) ATTRIBUTE_DESTRUCTOR(255);                       /*!< void type. */
  #endif

  //! Function: Scheduler Interrupt
  /*!
    This is the task manager: When a timer burst, this function is called. Tasks that have their time equal to 0 are stored in a FIFO buffer and are scheduled to run.
  */
  void ISR_schedulerInterruption(void) {
    static uint8_t ui8AmountOfTasks = 0;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_RUNNING){
      volatile uint8_t ui8Counter = 0;
      if (ui8AmountOfTasks != __ui8CreatedTaskCounter && __smSchedulerMode != SCHEDULER_MODE_ROUND_ROBIN){
        ui8AmountOfTasks = __ui8CreatedTaskCounter;
        for (ui8Counter = 0 ; ui8Counter < __ui8CreatedTaskCounter ; ui8Counter++){
          if (__thpBufferedTasks[ui8Counter]->ttTaskType == TASK_TYPE_COOPERATIVE_THREAD){
            if (__smSchedulerMode == SCHEDULER_MODE_RELATIVE_PRIORITY){
              __thpBufferedTasks[ui8Counter]->ui16Period = (__thpBufferedTasks[ui8Counter]->ui8Priority - __MINIMUM_THREAD_PRIORITY__ + 1)/__ui8HighestThreadPriority;
            }
            else if(__smSchedulerMode == SCHEDULER_MODE_ABSOLUTE_PRIORITY){
              __thpBufferedTasks[ui8Counter]->ui16Period = (__thpBufferedTasks[ui8Counter]->ui8Priority - __MINIMUM_THREAD_PRIORITY__ + 2 - __ui8HighestThreadPriority);
            }
            else if (__smSchedulerMode == SCHEDULER_MODE_FULL_PRIORITY){
              __thpBufferedTasks[ui8Counter]->ui16Period = __thpBufferedTasks[ui8Counter]->ui8Priority - __MINIMUM_THREAD_PRIORITY__ + 1;
            }
          }
        }
      }
      for (ui8Counter = 0 ; ui8Counter < __ui8CreatedTaskCounter ; ui8Counter++) {
        if (__thpBufferedTasks[ui8Counter]->tsStatus == TASK_STATUS_ENABLED) {
          if (__thpBufferedTasks[ui8Counter]->ui16TimeCounter > __thpBufferedTasks[ui8Counter]->ui16Period){
            __thpBufferedTasks[ui8Counter]->tsStatus = TASK_STATUS_READY;
            if (__smSchedulerMode != SCHEDULER_MODE_ROUND_ROBIN){
              ui8HighestPriority = SoftMath_smaller(ui8HighestPriority, __thpBufferedTasks[ui8Counter]->ui8Priority);
            }
            __ui8ReadyTaskCounter++;
          }
          else{
            __thpBufferedTasks[ui8Counter]->ui16TimeCounter++;
          }
        }
      }
      if (__tCurrentTask != NULL && __tCurrentTask->tsStatus == TASK_STATUS_RUNNING){
        if (__tCurrentTask->ui16TimeCounter > __tCurrentTask->ui16Timeout){
          if (__vfWatchdogCallback != NULL){
            __vfWatchdogCallback(__vpWatchdogCallbackArguments);
          }
          System_restart();
        }
        else{
          __tCurrentTask->ui16TimeCounter++;
        }
      }
    }
  }

  //! Function: Scheduler Mode Setter
  /*!
    Setts scheduler mode.
    \param smMode is a scheduler_mode_t type.
  */
  void Scheduler_setMode(scheduler_mode_t smMode){
    __smSchedulerMode = smMode;
  }

  //! Function: Scheduler Initializer
  /*!
    Initializes scheduler.
  */
  void Scheduler_enable(void){
    if (__ssSchedulerStatus == SCHEDULER_STATUS_STOPPED){
      System_sleepConfiguration();
      __ssSchedulerStatus = SCHEDULER_STATUS_RUNNING;
      System_taskTimerConfiguration(&ISR_schedulerInterruption);
    }
  }

  //! Function: Scheduler Starter
  /*!
    If have a pending task on buffer, the task runs.
  */
  void Scheduler_start(void){
    static uint8_t ui8FirstRun = 0;
    if (ui8FirstRun == 0){
      System_enableInterrupts();
      ui8FirstRun++;
    }
    if(__ssSchedulerStatus == SCHEDULER_STATUS_RUNNING){
      uint8_t ui8Counter = 0;
      if (__ui8ReadyTaskCounter == 0){
        System_sleep();
      }
      else{
        while (__ui8ReadyTaskCounter != 0) {
          if (__thpBufferedTasks[ui8Counter]->tsStatus == TASK_STATUS_READY){
            __tCurrentTask = __thpBufferedTasks[ui8Counter];
            __ui8ReadyTaskCounter--;
            __thpBufferedTasks[ui8Counter]->tsStatus = TASK_STATUS_RUNNING;
            __tCurrentTask->ui16TimeCounter = 0;
            if (__tCurrentTask->vfTaskFunction != NULL){
              System_enableHardwareWatchdog();
              __tCurrentTask->vfTaskFunction(__tCurrentTask->vpArguments);
              System_disableHardwareWatchdog();
            }
            if (__tCurrentTask->tsStatus == TASK_STATUS_RUNNING){
              __tCurrentTask->tsStatus = (__tCurrentTask->ui16TimeCounter >= __tCurrentTask->ui16Period) ? (TASK_STATUS_READY) : (TASK_STATUS_ENABLED);
              __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
              __ui8ReadyTaskCounter = (__tCurrentTask->tsStatus == TASK_STATUS_READY) ? (__ui8ReadyTaskCounter + 1) : __ui8ReadyTaskCounter;
              __ssSchedulerStatus = SCHEDULER_STATUS_RUNNING;
            }
          }
          if (__smSchedulerMode != SCHEDULER_MODE_ROUND_ROBIN && (__thpBufferedTasks[ui8Counter]->ui8Priority > ui8HighestPriority || ui8Counter >= (__ui8CreatedTaskCounter - 1))){
            ui8Counter = 0;
            ui8HighestPriority = 255;
          }
          else{
            ui8Counter++;
          }
        }
      }
    }
  }

  //! Function: All Tasks Enabler
  /*!
    Enables all tasks from task manager.
    \param tTask is a task_t type.
  */
  void Scheduler_enableAllTasks(void){
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    uint8_t ui8TaskCounter = 0;
    for(ui8TaskCounter = 0 ; __thpBufferedTasks[ui8TaskCounter] != NULL ; ui8TaskCounter++){
      if (__thpBufferedTasks[ui8TaskCounter]->tsStatus == TASK_STATUS_DISABLED){
        __thpBufferedTasks[ui8TaskCounter]->tsStatus = TASK_STATUS_ENABLED;
      }
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: All Tasks Disabler
  /*!
    Disables all tasks from task manager.
    \param tTask is a task_t type.
  */
  void Scheduler_disableAllTasks(void){
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    uint8_t ui8TaskCounter = 0;
    for(ui8TaskCounter = 0 ; __thpBufferedTasks[ui8TaskCounter] != NULL ; ui8TaskCounter++){
      if (__thpBufferedTasks[ui8TaskCounter]->tsStatus != TASK_STATUS_EMPTY){
        __thpBufferedTasks[ui8TaskCounter]->tsStatus = TASK_STATUS_DISABLED;
      }
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: Semaphore Watchdog Callback Attacher
  /*!
    Attaches a callback function on semaphore watchdog.
    \param vfWatchdogCallback is a void function pointer.
    \param vpWatchdogCallbackArguments is a void pointer.
  */
  void Scheduler_attachWatchdogCallback(void (*vfWatchdogCallback)(void*), void* vpWatchdogCallbackArguments){
    __vfWatchdogCallback = vfWatchdogCallback;
    __vpWatchdogCallbackArguments = vpWatchdogCallbackArguments;
  }

  //! Function: Semaphore Watchdog Callback Detacher
  /*!
    Detacher a callback function on semaphore watchdog.
  */
  void Scheduler_detachWatchdogCallback(void){
    Scheduler_attachWatchdogCallback(NULL, NULL);
  }

  #if defined (__AUTO_INITIALIZATION_ENABLE__)

    //! Function: Scheduler Starter
    /*!
      If have a pending task on buffer, the task runs.
    */
    void __Scheduler_autoStart(void){
      while(1){
        Scheduler_start();
      }
    }

    //! Function: Scheduler Initializer
    /*!
      Initializes scheduler.
    */
    void __Scheduler_autoEnableScheduler(void){
      Scheduler_enable();
    }

  #endif
#endif