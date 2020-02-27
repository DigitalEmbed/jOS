#include "./Configs.h"

#if defined(__TASK_SCHEDULER_ENABLE__) &&\
defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)

  #include "./Task.h"
  #include <string.h>
  #include <EmbeddedTools.h>
  #include <Scheduler.h>

  extern task_handler_t __thCreatedTasks[__AMOUNT_OF_TASKS__];                            /*!< task_handler_t type. */
  extern task_handler_t* __thpBufferedTasks[__AMOUNT_OF_TASKS__];                         /*!< task_handler_t pointer type. */
  extern task_t __tCurrentTask;                                                           /*!< task_t type. */
  extern uint8_t __ui8CreatedTaskCounter;                                                 /*!< uint8_t type. */
  extern uint8_t __ui8EmptyTaskPosition;                                                  /*!< uint8_t type. */
  extern scheduler_status_t __ssSchedulerStatus;                                          /*!< uint8_t type. */
  extern uint8_t __ui8ReadyTaskCounter;                                                   /*!< uint8_t type. */
  extern uint8_t __ui8HighestThreadPriority;                                              /*!< uint8_t type. */

  //! Function: Task Creator
  /*!
    Creates and insert a task on task manager.
    \param tTask is a task_t type.
    \param cpTaskName is a char pointer type. This is the name of task.
    \param vfTaskFunction is a task_return_t type. This is the function of the task.
    \param vpArguments is a void pointer type. This is the adress of arguments that are used in the task.
    \param ui8Priority is a 8-bit integer type. This is the priority of the task.
    \param ui16Period is a 16-bit integer type. This is the period that task will be executed.
    \param tsStatus is a 8-bit integer type. Put "ENABLE" on this argument if you want this task starts enabled or "TASK_STATUS_DISABLED" in contrary case.
    \return Returns created task pointer.
  */
  task_status_t newTask(task_t* tTask, const char* cpTaskName, void (*vfTaskFunction)(void*), void* vpArguments, uint16_t ui16Period, uint16_t ui16Timeout, uint8_t ui8Priority, task_status_t tsStatus){
    uint8_t ui8Counter = 0;
    uint8_t ui8VirtualAddress = 0;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return TASK_STATUS_ERROR;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if ((__ui8CreatedTaskCounter >= __AMOUNT_OF_TASKS__) || (tsStatus != TASK_STATUS_ENABLED && tsStatus != TASK_STATUS_DISABLED)){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return TASK_STATUS_ERROR;
    }
    for (ui8Counter = 0 ; ui8Counter < __AMOUNT_OF_TASKS__ ; ui8Counter++){
      if (__thCreatedTasks[ui8Counter].tsStatus != TASK_STATUS_EMPTY && strncmp(cpTaskName, __thCreatedTasks[ui8Counter].cpTaskName, __TASK_NAME_SIZE__) == 0){
        __ssSchedulerStatus = __ssSchedulerStatusBuffer;
        return TASK_STATUS_ERROR;
      }
    }
    while(__thCreatedTasks[__ui8EmptyTaskPosition].tsStatus != TASK_STATUS_EMPTY){
      __ui8EmptyTaskPosition++;
    }
    (*tTask) = &__thCreatedTasks[__ui8EmptyTaskPosition];
    memset((*tTask)->cpTaskName, 0, __TASK_NAME_SIZE__ * sizeof(char));
    memcpy((*tTask)->cpTaskName, cpTaskName, __TASK_NAME_SIZE__ * sizeof(char));
    (*tTask)->ui16StartLine = 0;
    (*tTask)->ui16TimeCounter = 0;
    (*tTask)->vfTaskFunction = vfTaskFunction;
    (*tTask)->vpArguments = vpArguments;
    (*tTask)->tsStatus = tsStatus;
    (*tTask)->ui8RealAddress = __ui8EmptyTaskPosition;
    (*tTask)->ui16Timeout = (ui16Timeout > (__MINIMUM_TASK_TIMEOUT_MS__)) ? (ui16Timeout/__ui8TickMS) : ((__MINIMUM_TASK_TIMEOUT_MS__)/__ui8TickMS);
    if (ui16Period == COOPERATIVE_MODE){
      (*tTask)->ttTaskType = TASK_TYPE_COOPERATIVE_THREAD;
      (*tTask)->ui16Period = __ui8TickMS;
      (*tTask)->ui8Priority = ui8Priority + __MINIMUM_THREAD_PRIORITY__;
      __ui8HighestThreadPriority = SoftMath_smaller(__ui8HighestThreadPriority, (ui8Priority + 1));
    }
    else{
      (*tTask)->ttTaskType = TASK_TYPE_TIMER;
      (*tTask)->ui16Period = (ui16Period <= __ui8TickMS) ? __ui8TickMS : (ui16Period/__ui8TickMS);
      (*tTask)->ui8Priority = ui8Priority;
    }
    for(ui8Counter = __ui8CreatedTaskCounter ; ui8Counter > 0 ; ui8Counter --){
      if (__thpBufferedTasks[ui8Counter - 1]->ui8Priority > (*tTask)->ui8Priority){
        __thpBufferedTasks[ui8Counter] = __thpBufferedTasks[ui8Counter - 1];
        __thpBufferedTasks[ui8Counter]->ui8VirtualAddress++;
      }
      else{
        ui8VirtualAddress = ui8Counter;
        break;
      }
    }
    __thpBufferedTasks[ui8VirtualAddress] = (*tTask);
    (*tTask)->ui8VirtualAddress = ui8VirtualAddress;
    __ui8CreatedTaskCounter++;
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
    return TASK_STATUS_CREATED;
  }

  #if defined(__TASK_DESTRUCTOR_ENABLE__)

    //! Function: Task Destructor
    /*!
      Deletes a task from task manager.
      \param tTask is a task_t type.
    */
    void deleteTask(task_t tTask){
      uint8_t ui8Counter = 0;
      task_t tTaskBuffer = tTask;
      scheduler_status_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
      if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
        return;
      }
      __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
      if (tTaskBuffer == NULL){
        tTaskBuffer = __tCurrentTask;
      }
      if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
        __ssSchedulerStatus = __ssSchedulerStatusBuffer;
        return;
      }
      if (__thCreatedTasks[tTaskBuffer->ui8RealAddress].tsStatus == TASK_STATUS_READY){
        __ui8ReadyTaskCounter--;
      }
      if (__thCreatedTasks[tTaskBuffer->ui8RealAddress].ttTaskType == TASK_TYPE_COOPERATIVE_THREAD){
        __ui8HighestThreadPriority -= __thCreatedTasks[tTaskBuffer->ui8RealAddress].ui8Priority;
      }
      __thCreatedTasks[tTaskBuffer->ui8RealAddress].tsStatus = TASK_STATUS_EMPTY;
      for (ui8Counter = tTaskBuffer->ui8VirtualAddress ; ui8Counter < __ui8CreatedTaskCounter ; ui8Counter++){
        __thpBufferedTasks[ui8Counter]->ui8VirtualAddress--;
        __thpBufferedTasks[ui8Counter] = __thpBufferedTasks[ui8Counter + 1];
      }
      __thpBufferedTasks[ui8Counter] = NULL;
      __ui8CreatedTaskCounter--;
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      tTask = NULL;
    }
    
  #endif

  //! Function: Task Disabler
  /*!
    Disables a task from task manager.
    \param tTask is a task_t type.
  */
  void Task_disable(task_t tTask){
    task_t tTaskBuffer = tTask;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if (tTaskBuffer == NULL){
      tTaskBuffer = __tCurrentTask;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer->tsStatus != TASK_STATUS_EMPTY){
      tTaskBuffer->tsStatus = TASK_STATUS_DISABLED;
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: Task Enabler
  /*!
    Enables a task from task manager.
    \param tTask is a task_t type.
  */
  void Task_enable(task_t tTask){
    task_t tTaskBuffer = tTask;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if (tTaskBuffer == NULL){
      tTaskBuffer = __tCurrentTask;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer->tsStatus == TASK_STATUS_DISABLED){
      tTaskBuffer->tsStatus = TASK_STATUS_ENABLED;
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: Task Executer
  /*!
    Forces task execution.
    \param tTask is a task_t type.
  */
  void Task_forceExecution(task_t tTask){
    task_t tTaskBuffer = tTask;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if (tTaskBuffer == NULL){
      __tCurrentTask->ui16TimeCounter = tTaskBuffer->ui16Period;
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer->tsStatus == TASK_STATUS_ENABLED){
      tTaskBuffer->tsStatus = TASK_STATUS_READY;
      tTaskBuffer->ui16TimeCounter = tTaskBuffer->ui16Period;
      __ui8ReadyTaskCounter++;
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: Task Timer Restarter
  /*!
    Restarts timer task.
    \param tTask is a task_t type.
  */
  void Task_restartTimer(task_t tTask){
    task_t tTaskBuffer = tTask;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if (tTaskBuffer == NULL){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer->tsStatus != TASK_STATUS_EMPTY && tTaskBuffer->tsStatus != TASK_STATUS_DISABLED){
      if (tTaskBuffer->tsStatus == TASK_STATUS_READY){
        tTaskBuffer->tsStatus = TASK_STATUS_ENABLED;
        __ui8ReadyTaskCounter--;
      }
      tTaskBuffer->ui16TimeCounter = tTaskBuffer->ui16Period;
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: Task Arguments Getter
  /*!
    Gets task argument pointer.
    \param tTask is a task_t type.
  */
  void* Task_getArguments(task_t tTask){
    task_t tTaskBuffer = tTask;
    if (tTaskBuffer == NULL){
      tTaskBuffer = __tCurrentTask;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      return NULL;
    }
    if (tTaskBuffer->tsStatus != TASK_STATUS_EMPTY){
      return tTaskBuffer->vpArguments;
    }
    return NULL;
  }

  //! Function: Task Arguments Changer
  /*!
    Changes task argument pointer.
    \param tTask is a task_t type.
  */
  void Task_setArguments(task_t tTask, void* vpArguments){
    task_t tTaskBuffer = tTask;
    if (tTaskBuffer == NULL){
      tTaskBuffer = __tCurrentTask;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      return;
    }
    if (tTaskBuffer->tsStatus != TASK_STATUS_EMPTY){
      tTaskBuffer->vpArguments = vpArguments;
    }
  }

  //! Function: Task Finder
  /*!
    This function is a task finder.
    \param cpTaskName is a char pointer type. This is the name of task.
    \return Returns the pointer of task or NULL if the task don't exist.
  */
  task_t tFindTask(const char* cpTaskName){
    uint8_t ui8Counter = 0;
    if (cpTaskName == NULL){
      return NULL;
    }
    for (ui8Counter = 0 ; ui8Counter < __ui8CreatedTaskCounter ; ui8Counter++){
      if (__thpBufferedTasks[ui8Counter]->tsStatus != TASK_STATUS_EMPTY && strncmp(cpTaskName, __thpBufferedTasks[ui8Counter]->cpTaskName, __TASK_NAME_SIZE__) == 0){
        return __thpBufferedTasks[ui8Counter];
      }
    }
    return NULL;
  }

  //! Function: Task Period Changer
  /*!
    Change a task period from task manager.
    \param tTask is a task_t type.
    \param ui16Period is a 16-bit integer type. This is the period that task will be executed.
  */
  void Task_setPeriod(task_t tTask, uint16_t ui16Period){
    task_t tTaskBuffer = tTask;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if (tTaskBuffer == NULL){
      tTaskBuffer = __tCurrentTask;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer->tsStatus != TASK_STATUS_EMPTY){
      tTaskBuffer->ui16Period = ui16Period/__ui8TickMS;
      if (tTaskBuffer->tsStatus == TASK_STATUS_READY && ui16Period > tTaskBuffer->ui16Period){
        tTaskBuffer->tsStatus = TASK_STATUS_ENABLED;
        tTaskBuffer->ui16TimeCounter = tTaskBuffer->ui16Period;
        __ui8ReadyTaskCounter--;
      }
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

  //! Function: Task Priority Changer
  /*!
    Change a task priority from task manager.
    \param tTask is a task_t type.
    \param ui8Priority is a 8-bit integer type. This is the priority of the task..
  */
  void Task_setPriority(task_t tTask, uint8_t ui8Priority){
    task_t tTaskBuffer = tTask;
    uint8_t ui8Counter = 0;
    uint8_t __ssSchedulerStatusBuffer = __ssSchedulerStatus;
    if (__ssSchedulerStatus == SCHEDULER_STATUS_PAUSED){
      return;
    }
    __ssSchedulerStatus = SCHEDULER_STATUS_PAUSED;
    if (tTaskBuffer == NULL){
      tTaskBuffer = __tCurrentTask;
    }
    if (tTaskBuffer != __thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &__thCreatedTasks[tTaskBuffer->ui8RealAddress]){
      __ssSchedulerStatus = __ssSchedulerStatusBuffer;
      return;
    }
    if (tTaskBuffer->tsStatus != TASK_STATUS_EMPTY){
      for (ui8Counter = tTask->ui8VirtualAddress + 1 ; ui8Counter < __ui8CreatedTaskCounter ; ui8Counter++){
        if (__thpBufferedTasks[ui8Counter]->ui8Priority >= ui8Priority){
          break;
        }
        else{
          __thpBufferedTasks[ui8Counter - 1] = __thpBufferedTasks[ui8Counter];
          __thpBufferedTasks[ui8Counter - 1]->ui8VirtualAddress--;
        }
      }
      __thpBufferedTasks[ui8Counter - 1] = tTask;
      tTask->ui8VirtualAddress = ui8Counter - 1;
    }
    __ssSchedulerStatus = __ssSchedulerStatusBuffer;
  }

#endif