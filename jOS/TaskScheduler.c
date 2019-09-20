#include "./TaskScheduler.h"
#include <EmbeddedTools.h>
#include <string.h>

task_handler_t thCreatedTasks[AMOUNT_OF_TASKS] = {0};                             /*!< task_handler_t type. */
task_handler_t* thpBufferedTasks[AMOUNT_OF_TASKS] = {NULL};                       /*!< task_handler_t pointer type. */
task_t tCurrentTask = NULL;                                                       /*!< task_t type. */
uint8_t ui8CreatedTaskCounter = 0;                                                /*!< uint8_t type. */
uint8_t ui8EmptyTaskPosition = 0;                                                 /*!< uint8_t type. */
uint8_t ui8SchedulerStatus = 0;                                                   /*!< uint8_t type. */
uint8_t ui8ReadyTaskCounter = 0;                                                  /*!< uint8_t type. */
uint8_t ui8HighestPriority = 255;                                                 /*!< uint8_t type. */
uint8_t ui8HighestThreadPriority = 255;                                           /*!< uint8_t type. */
uint8_t ui8SchedulerType = ROUND_ROBIN_MODE;                                      /*!< uint8_t type. */

void vCheckTaskReturn(task_return_t trTaskReturn);                                /*!< void type function. */

//! Function: Scheduler Starter
/*!
  This function is called to check the return of the tasks that were performed. Decisions are made based on this return. Edit this function if it does not fully fit your project.
*/
void vCheckTaskReturn(task_return_t trTaskReturn){
  switch (trTaskReturn) {

    default:
      break;

    case END:
      break;

    case SLEEP:
      vSystemSleep();
      break;

    case RESTART:
      vRestartSystem();
      break;
  }
}

//! Function: Task Creator
/*!
  Creates and insert a task on task manager.
  \param cpTaskName is a char pointer type. This is the name of task.
  \param tTaskFunction is a task_return_t type. This is the function of the task.
  \param vpArguments is a void pointer type. This is the adress of arguments that are used in the task.
  \param ui8Priority is a 8-bit integer type. This is the priority of the task.
  \param ui16Period is a 16-bit integer type. This is the period that task will be executed.
  \param ui8Status is a 8-bit integer type. Put "ENABLE" on this argument if you want this task starts enabled or "DISABLED" in contrary case.
  \return Returns created task pointer.
*/
task_t tCreateTask(const char* cpTaskName, task_return_t (*tTaskFunction)(void*), void* vpArguments, uint16_t ui16Period, uint8_t ui8Priority, uint8_t ui8Status){
  uint8_t ui8Counter = 0;
  uint8_t ui8VirtualAddress = 0;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return NULL;
  }
  ui8SchedulerStatus = PAUSED;
  if ((ui8CreatedTaskCounter >= AMOUNT_OF_TASKS) || (ui8Status != ENABLED && ui8Status != DISABLED)){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return NULL;
  }
  for (ui8Counter = 0 ; ui8Counter < AMOUNT_OF_TASKS ; ui8Counter++){
    if (thCreatedTasks[ui8Counter].ui8Status != EMPTY && strncmp(cpTaskName, thCreatedTasks[ui8Counter].cpTaskName, SIZE_OF_TASK_NAME) == 0){
      ui8SchedulerStatus = ui8SchedulerStatusBuffer;
      return NULL;
    }
  }
  while(thCreatedTasks[ui8EmptyTaskPosition].ui8Status != EMPTY){
    ui8EmptyTaskPosition++;
  }
  task_handler_t* thpTask = &thCreatedTasks[ui8EmptyTaskPosition];
  memset(thpTask->cpTaskName, 0, SIZE_OF_TASK_NAME * sizeof(char));
  memcpy(thpTask->cpTaskName, cpTaskName, SIZE_OF_TASK_NAME * sizeof(char));
  thpTask->tTaskFunction = tTaskFunction;
  thpTask->vpArguments = vpArguments;
  thpTask->ui8Status = ui8Status;
  thpTask->ui8RealAddress = ui8EmptyTaskPosition;
  if (ui16Period == COOPERATIVE_MODE){
    thpTask->ui8TaskType = COOPERATIVE_THREAD_TYPE;
    thpTask->ui16Period = ui8TickMS;
    thpTask->ui8Priority = ui8Priority + MINIMAL_THREAD_PRIORITY;
    ui8HighestThreadPriority = iSmaller(ui8HighestThreadPriority, (ui8Priority + 1));
  }
  else{
    thpTask->ui8TaskType = TIMER_TYPE;
    thpTask->ui16Period = (ui16Period <= ui8TickMS) ? ui8TickMS : (ui16Period/ui8TickMS);
    thpTask->ui8Priority = ui8Priority;
  }
  for(ui8Counter = ui8CreatedTaskCounter ; ui8Counter > 0 ; ui8Counter --){
    if (thpBufferedTasks[ui8Counter - 1]->ui8Priority > thpTask->ui8Priority){
      thpBufferedTasks[ui8Counter] = thpBufferedTasks[ui8Counter - 1];
      thpBufferedTasks[ui8Counter]->ui8VirtualAddress++;
    }
    else{
      ui8VirtualAddress = ui8Counter;
      break;
    }
  }
  thpBufferedTasks[ui8VirtualAddress] = thpTask;
  thpTask->ui8VirtualAddress = ui8VirtualAddress;
  ui8CreatedTaskCounter++;
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
  return thpTask;
}

//! Function: Task Deleter
/*!
  Deletes a task from task manager.
  \param tTask is a task_t type.
*/
void vDeleteTask(task_t tTask){
  uint8_t ui8Counter = 0;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTask != thpBufferedTasks[tTask->ui8VirtualAddress] || tTask != &thCreatedTasks[tTask->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (thCreatedTasks[tTask->ui8RealAddress].ui8Status == READY){
    ui8ReadyTaskCounter--;
  }
  if (thCreatedTasks[tTask->ui8RealAddress].ui8TaskType == COOPERATIVE_THREAD_TYPE){
    ui8HighestThreadPriority -= thCreatedTasks[tTask->ui8RealAddress].ui8Priority;
  }
  thCreatedTasks[tTask->ui8RealAddress].ui8Status = EMPTY;
  for (ui8Counter = tTask->ui8VirtualAddress ; ui8Counter < ui8CreatedTaskCounter ; ui8Counter++){
    thpBufferedTasks[ui8Counter]->ui8VirtualAddress--;
    thpBufferedTasks[ui8Counter] = thpBufferedTasks[ui8Counter + 1];
  }
  thpBufferedTasks[ui8Counter] = NULL;
  ui8CreatedTaskCounter--;
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
  tTask = NULL;
}

//! Function: Scheduler Interrupt
/*!
  This is the task manager: When a timer burst, this function is called. Tasks that have their time equal to 0 are stored in a FIFO buffer and are scheduled to run.
*/
void vSchedulerInterruption(void) {
  static uint8_t ui8AmountOfTasks = 0;
  if (ui8SchedulerStatus == RUNNING){
    volatile uint8_t ui8Counter = 0;
    if (ui8AmountOfTasks != ui8CreatedTaskCounter && ui8SchedulerType != ROUND_ROBIN_MODE){
      ui8AmountOfTasks = ui8CreatedTaskCounter;
      for (ui8Counter = 0 ; ui8Counter < ui8CreatedTaskCounter ; ui8Counter++){
        if (thpBufferedTasks[ui8Counter]->ui8TaskType == COOPERATIVE_THREAD_TYPE){
          if (ui8SchedulerType == RELATIVE_PRIORITY_MODE){
            thpBufferedTasks[ui8Counter]->ui16Period = (thpBufferedTasks[ui8Counter]->ui8Priority - MINIMAL_THREAD_PRIORITY + 1)/ui8HighestThreadPriority;
          }
          else if(ui8SchedulerType == ABSOLUTE_PRIORITY_MODE){
            thpBufferedTasks[ui8Counter]->ui16Period = (thpBufferedTasks[ui8Counter]->ui8Priority - MINIMAL_THREAD_PRIORITY + 2 - ui8HighestThreadPriority);
          }
          else if (ui8SchedulerType == FULL_PRIORITY_MODE){
            thpBufferedTasks[ui8Counter]->ui16Period = thpBufferedTasks[ui8Counter]->ui8Priority - MINIMAL_THREAD_PRIORITY + 1;
          }
        }
      }
    }
    for (ui8Counter = 0 ; ui8Counter < ui8CreatedTaskCounter ; ui8Counter++) {
      if (thpBufferedTasks[ui8Counter]->ui8Status == ENABLED) {
        if (thpBufferedTasks[ui8Counter]->ui16TimeCounter > thpBufferedTasks[ui8Counter]->ui16Period){
          thpBufferedTasks[ui8Counter]->ui16TimeCounter = 0;
          thpBufferedTasks[ui8Counter]->ui8Status = READY;
          if (ui8SchedulerType != ROUND_ROBIN_MODE){
            ui8HighestPriority = iSmaller(ui8HighestPriority, thpBufferedTasks[ui8Counter]->ui8Priority);
          }
          ui8ReadyTaskCounter++;
        }
        else{
          thpBufferedTasks[ui8Counter]->ui16TimeCounter++;
        }
      }
    }
  }
}

//! Function: Scheduler Initializer
/*!
  Initializes scheduler.
*/
void vEnableScheduler(uint8_t ui8SchedulerMode){
  if (ui8SchedulerStatus == STOPED){
    vSystemSleepConfiguration();
    ui8SchedulerStatus = RUNNING;
    ui8SchedulerType = ui8SchedulerMode;
    vTaskTimerConfiguration(&vSchedulerInterruption);
  }
}

//! Function: Scheduler Starter
/*!
  If have a pending task on buffer, the task runs.
*/
void vStartScheduler(void){
  uint8_t ui8Counter = 0;
  if (ui8ReadyTaskCounter == 0){
    vSystemSleep();
  }
  else{
    while (ui8ReadyTaskCounter != 0) {
      if (thpBufferedTasks[ui8Counter]->ui8Status == READY){
        task_return_t trReturn = END;
        tCurrentTask = thpBufferedTasks[ui8Counter];
        if (tCurrentTask->tTaskFunction != NULL){
          vEnableWatchdog();
          trReturn = tCurrentTask->tTaskFunction(tCurrentTask->vpArguments);
          vDisableWatchdog();
        }
        vCheckTaskReturn(trReturn);
        if (tCurrentTask != NULL && tCurrentTask->ui8Status == READY && tCurrentTask->ui16TimeCounter == 0){
          tCurrentTask->ui8Status = ENABLED;
          ui8ReadyTaskCounter--;
        }
      }
      if (ui8SchedulerType != ROUND_ROBIN_MODE && (thpBufferedTasks[ui8Counter]->ui8Priority > ui8HighestPriority || ui8Counter >= (ui8CreatedTaskCounter - 1))){
        ui8Counter = 0;
        ui8HighestPriority = 255;
      }
      else{
        ui8Counter++;
      }
    }
  }
}

//! Function: Task Disabler
/*!
  Disables a task from task manager.
  \param tTask is a task_t type.
*/
void vDisableTask(task_t tTask){
  task_t tTaskBuffer = tTask;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTaskBuffer == NULL){
    tTaskBuffer = tCurrentTask;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer->ui8Status != EMPTY){
    tTaskBuffer->ui8Status = DISABLED;
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: Task Enabler
/*!
  Enables a task from task manager.
  \param tTask is a task_t type.
*/
void vEnableTask(task_t tTask){
  task_t tTaskBuffer = tTask;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTaskBuffer == NULL){
    tTaskBuffer = tCurrentTask;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer->ui8Status == DISABLED){
    tTaskBuffer->ui8Status = ENABLED;
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: Task Executer
/*!
  Forces task execution.
  \param tTask is a task_t type.
*/
void vForceTaskExecution(task_t tTask){
  task_t tTaskBuffer = tTask;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTaskBuffer == NULL){
    tCurrentTask->ui16TimeCounter = tTaskBuffer->ui16Period;
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer->ui8Status == ENABLED){
    tTaskBuffer->ui8Status = READY;
    tTaskBuffer->ui16TimeCounter = tTaskBuffer->ui16Period;
    ui8ReadyTaskCounter++;
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: Task Timer Restarter
/*!
  Restarts timer task.
  \param tTask is a task_t type.
*/
void vRestartTimerTask(task_t tTask){
  task_t tTaskBuffer = tTask;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTaskBuffer == NULL){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer->ui8Status != EMPTY && tTaskBuffer->ui8Status != DISABLED){
    if (tTaskBuffer->ui8Status == READY){
      tTaskBuffer->ui8Status = ENABLED;
      ui8ReadyTaskCounter--;
    }
    tTaskBuffer->ui16TimeCounter = tTaskBuffer->ui16Period;
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: All Tasks Enabler
/*!
  Enables all tasks from task manager.
  \param tTask is a task_t type.
*/
void vEnableAllTasks(void){
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  uint8_t ui8TaskCounter = 0;
  for(ui8TaskCounter = 0 ; thpBufferedTasks[ui8TaskCounter] != NULL ; ui8TaskCounter++){
    if (thpBufferedTasks[ui8TaskCounter]->ui8Status == DISABLED){
      thpBufferedTasks[ui8TaskCounter]->ui8Status = ENABLED;
    }
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: All Tasks Disabler
/*!
  Disables all tasks from task manager.
  \param tTask is a task_t type.
*/
void vDisableAllTasks(void){
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  uint8_t ui8TaskCounter = 0;
  for(ui8TaskCounter = 0 ; thpBufferedTasks[ui8TaskCounter] != NULL ; ui8TaskCounter++){
    if (thpBufferedTasks[ui8TaskCounter]->ui8Status != EMPTY){
      thpBufferedTasks[ui8TaskCounter]->ui8Status = DISABLED;
    }
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: Task Arguments Getter
/*!
  Gets task argument pointer.
  \param tTask is a task_t type.
*/
void* vpGetTaskArguments(task_t tTask){
  task_t tTaskBuffer = tTask;
  if (tTaskBuffer == NULL){
    tTaskBuffer = tCurrentTask;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    return NULL;
  }
  if (tTaskBuffer->ui8Status != EMPTY){
    return tTaskBuffer->vpArguments;
  }
  return NULL;
}

//! Function: Task Arguments Changer
/*!
  Changes task argument pointer.
  \param tTask is a task_t type.
*/
void vChangeTaskArguments(task_t tTask, void* vpArguments){
  task_t tTaskBuffer = tTask;
  if (tTaskBuffer == NULL){
    tTaskBuffer = tCurrentTask;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    return;
  }
  if (tTaskBuffer->ui8Status != EMPTY){
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
  for (ui8Counter = 0 ; ui8Counter < ui8CreatedTaskCounter ; ui8Counter++){
    if (thpBufferedTasks[ui8Counter]->ui8Status != EMPTY && strncmp(cpTaskName, thpBufferedTasks[ui8Counter]->cpTaskName, SIZE_OF_TASK_NAME) == 0){
      return thpBufferedTasks[ui8Counter];
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
void vChangeTaskPeriod(task_t tTask, uint16_t ui16Period){
  task_t tTaskBuffer = tTask;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTaskBuffer == NULL){
    tTaskBuffer = tCurrentTask;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer->ui8Status != EMPTY){
    tTaskBuffer->ui16Period = ui16Period/ui8TickMS;
    if (tTaskBuffer->ui8Status == READY && tTask != NULL){
      tTaskBuffer->ui8Status = ENABLED;
      tTaskBuffer->ui16TimeCounter = tTaskBuffer->ui16Period;
      ui8ReadyTaskCounter--;
    }
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}

//! Function: Task Priority Changer
/*!
  Change a task priority from task manager.
  \param tTask is a task_t type.
  \param ui8Priority is a 8-bit integer type. This is the priority of the task..
*/
void vChangeTaskPriority(task_t tTask, uint8_t ui8Priority){
  task_t tTaskBuffer = tTask;
  uint8_t ui8Counter = 0;
  uint8_t ui8SchedulerStatusBuffer = ui8SchedulerStatus;
  if (ui8SchedulerStatus == PAUSED){
    return;
  }
  ui8SchedulerStatus = PAUSED;
  if (tTaskBuffer == NULL){
    tTaskBuffer = tCurrentTask;
  }
  if (tTaskBuffer != thpBufferedTasks[tTaskBuffer->ui8VirtualAddress] || tTaskBuffer != &thCreatedTasks[tTaskBuffer->ui8RealAddress]){
    ui8SchedulerStatus = ui8SchedulerStatusBuffer;
    return;
  }
  if (tTaskBuffer->ui8Status != EMPTY){
    for (ui8Counter = tTask->ui8VirtualAddress + 1 ; ui8Counter < ui8CreatedTaskCounter ; ui8Counter++){
      if (thpBufferedTasks[ui8Counter]->ui8Priority >= ui8Priority){
        break;
      }
      else{
        thpBufferedTasks[ui8Counter - 1] = thpBufferedTasks[ui8Counter];
        thpBufferedTasks[ui8Counter - 1]->ui8VirtualAddress--;
      }
    }
    thpBufferedTasks[ui8Counter - 1] = tTask;
    tTask->ui8VirtualAddress = ui8Counter - 1;
  }
  ui8SchedulerStatus = ui8SchedulerStatusBuffer;
}