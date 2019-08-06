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

#include "TaskScheduler.h"
#include <stdio.h>
#include <string.h>
#include "System.h"
#include "jOS.h"

//! Macros: Task Scheduler Status
/*!
  This macros leave the code more cleaner and understandable.
*/
#define   ON_HOLD         0
#define   RUNNING_TASK    1
#define   STOP_SCHEDULER  2

//! Type Definition: fargs_t
/*!
  This typedef is equivalent to a void function pointer.
*/
typedef void* fargs_t;

//! "Static" Variables: Task Storage
/*!
  These variables are for storing tasks in the task manager.
*/
volatile task_t* tpTaskArray[AMOUNT_OF_PRIORITIES][AMOUNT_OF_TASK_IN_A_PRIORITY] = {{NULL}};          /*!< task_t pointer type array. */
volatile task_t* tpPriorizedTaskVector[AMOUNT_OF_PRIORIZED_TASKS] = {NULL};                           /*!< task_t pointer type vector. */
volatile fargs_t faTaskArguments[AMOUNT_OF_PRIORITIES][AMOUNT_OF_TASK_IN_A_PRIORITY] = {{NULL}};      /*!< fargs_t type array. */

volatile uint16_t ui16TaskArrayTimer[AMOUNT_OF_PRIORITIES][AMOUNT_OF_TASK_IN_A_PRIORITY] = {{0}};     /*!< 16-bit integer type vector. */
volatile uint16_t ui16SystemTimer = 0;                                                                /*!< 16-bit integer type. */

volatile task_t* tpCurrentTask = NULL;                                                                /*!< task_t pointer type. */

volatile uint8_t ui8AmountOfTasks[AMOUNT_OF_PRIORITIES] = {0};                                        /*!< 8-bit integer type. */

volatile fargs_t faScheduledTasksArguments[AMOUNT_OF_TASK_SCHEDULED] = {NULL};                        /*!< fargs_t type vector. */
buffer_t* bpScheduledTasks = NULL;                                                                    /*!< buffer_t pointer type. */
task_t** tppScheduledTasksVector[AMOUNT_OF_TASK_SCHEDULED] = {NULL};                                  /*!< task_t type vector. */

volatile uint8_t ui8LastPriority = 0;                                                                 /*!< 8-bit integer type. */
volatile uint16_t ui16AmountOfPriorizedTasks = 0;                                                     /*!< 16-bit integer type. */

//! "Static" Variables: Task Storage
/*!
  This variable is for status scheduler manager.
*/
volatile uint8_t ui8SchedulerStatus = ON_HOLD;                                                        /*!< 8-bit integer type. */

void vSchedulerInterrupt(void);                                                                       /*!< Void type function. */
void vCheckTaskReturn(uint8_t ui8TaskReturn);                                                         /*!< Void type function. */

//! Function: Task Manager Initializer
/*!
  Initialize the task manager.
  \return Returns BUFFER_MANAGER_ERROR, TASK_SCHEDULED_BUFFER_ERROR or TASK_MANAGER_INITIALIZED.
*/
uint8_t ui8TaskManagerInit(){
  if (ui8BufferManagerInit() != BUFFER_INITIALIZED){
    return BUFFER_MANAGER_ERROR;
  }
  bpScheduledTasks = bpCreateGenericBuffer(tppScheduledTasksVector, QUEUE, sizeof(task_t**), AMOUNT_OF_TASK_SCHEDULED);
  if (bpScheduledTasks == NULL){
    return TASK_SCHEDULED_BUFFER_ERROR;
  }
  vSystemTimerSchedulerInterruption(&vSchedulerInterrupt);
  vSystemSleepConfiguration();
  vSystemHardwareWatchdogConfiguration();
  return TASK_MANAGER_INITIALIZED;
}

//! Function: Task Inserter
/*!
  Insert a task on task manager.
  \param tpTask is a task_t pointer type.
  \param pfFunction is a task_pfunc_t type. This is the function of the task.
  \param cpTaskName is a char pointer type. This is the name of task.
  \param vpArguments is a void pointer type. This is the adress of arguments that are used in the task.
  \param ui8Priority is a 8-bit integer type. This is the priority of the task.
  \param ui16Period is a 16-bit integer type. This is the period that task will be executed.
  \param ui8Status is a 8-bit integer type. Put "ABLE" on this argument case the task is started abled on system start, or "DISABED" in contrary case.
  \return Returns ERROR_TASK_NOT_ADDED, ERROR_PRIORITY_FULL, EXISTING_TASK or TASK_ADDED.
*/
uint8_t ui8AddTask(task_t* tpTask, task_pfunc_t pfFunction, const char* cpTaskName, void* vpArguments, uint8_t ui8Priority, uint16_t ui16Period, uint8_t ui8Status){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if (tpTask == NULL || pfFunction == NULL || ui8Priority >= AMOUNT_OF_PRIORITIES || (ui8Status != ENABLED && ui8Status != DISABLED && ui8Status != PRIORIZED)){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_ADDED;
  }
  if (ui8AmountOfTasks[ui8Priority] >= AMOUNT_OF_TASK_IN_A_PRIORITY){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_PRIORITY_FULL;
  }
  tpTask->cpTaskName = (char*) vpDBAlloc(sizeof(char), TASK_NAME_SIZE + 1);
  if (tpTask->cpTaskName == NULL){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return NO_SUCH_MEMORY;
  }
  memset(tpTask->cpTaskName, 0, TASK_NAME_SIZE + 1);
  memcpy(tpTask->cpTaskName, cpTaskName, sizeof(char) * TASK_NAME_SIZE);
  tpTask->pfFunction = pfFunction;
  tpTask->ui8Priority = ui8Priority;
  tpTask->ui16Period = ui16Period;
  tpTask->vpArguments = vpArguments;
  tpTask->ui16Line = 0;
  if (ui8Status == NOT_INSTALLED){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_CREATED;
  }
  else if (ui8Status == PRIORIZED){
    tpTask->ui8Status = ENABLED;
  }
  else{
    tpTask->ui8Status = ui8Status;
  }
  tpTask->ui8TaskAddress = ui8AmountOfTasks[ui8Priority];
  ui16TaskArrayTimer[ui8Priority][tpTask->ui8TaskAddress] = ui16Period;
  tpTaskArray[ui8Priority][tpTask->ui8TaskAddress] = tpTask;
  faTaskArguments[ui8Priority][tpTask->ui8TaskAddress] = vpArguments;
  ui8AmountOfTasks[ui8Priority]++;
  ui8LastPriority = iBigger(ui8LastPriority, ui8Priority);
  if (ui8Status == PRIORIZED){
    if (ui8PriorizeTask(tpTask) == TASK_PRIORIZED){
      ui8SchedulerStatus = ui8BufferSchedulerStatus;
      return TASK_ADDED;
    }
    else{
      ui8SchedulerStatus = ui8BufferSchedulerStatus;
      return ERROR_TASK_NOT_ADDED;
    }
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_ADDED;
  }
}

//! Function: Task Remover
/*!
  Remove a task from task manager.
  \param tpTask is a task_t pointer type.
  \return Returns ERROR_TASK_NOT_REMOVED or TASK_REMOVED.
*/
uint8_t ui8RemoveTask(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  uint8_t ui8Priority = tpTask->ui8Priority;
  uint8_t ui8TaskAddress = tpTask->ui8TaskAddress;
  uint8_t ui8Counter = 0;
  if (tpTask->ui8Status == NOT_INSTALLED){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_REMOVED;
  }
  else if (tpTask->ui8Status == PRIORIZED && ui8DepriveTask(tpTask) != TASK_DEPRIVED){
    return ERROR_TASK_NOT_REMOVED;
  }
  if(tpTaskArray[ui8Priority][ui8TaskAddress] != tpTask){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_REMOVED;
  }
  tpTask->ui8Status = NOT_INSTALLED;
  for (ui8Counter = tpTask->ui8TaskAddress ; (tpTask->ui8TaskAddress != ui8AmountOfTasks[ui8Priority] - 1) && (ui8Counter <= (ui8AmountOfTasks[ui8Priority] - 2)) ; ui8Counter++){
    tpTaskArray[ui8Priority][ui8Counter] = tpTaskArray[ui8Priority][ui8Counter + 1];
    faTaskArguments[ui8Priority][ui8Counter] = faTaskArguments[ui8Priority][ui8Counter + 1];
    ui16TaskArrayTimer[ui8Priority][ui8Counter] = ui16TaskArrayTimer[ui8Priority][ui8Counter + 1];
    tpTaskArray[ui8Priority][ui8Counter + 1]->ui8TaskAddress--;
  }
  tpTaskArray[ui8Priority][ui8Counter] = NULL;
  faTaskArguments[ui8Priority][ui8Counter] = NULL;
  ui16TaskArrayTimer[ui8Priority][ui8Counter] = 0;
  ui8AmountOfTasks[ui8Priority]--;
  if (tpTask->ui8Priority == ui8LastPriority){
    for (ui8Counter = ui8LastPriority ; ui8Counter > 0 ; ui8Counter--){
      if (ui8AmountOfTasks[ui8Counter] > 0){
        ui8LastPriority = ui8Counter;
        ui8SchedulerStatus = ui8BufferSchedulerStatus;
        return TASK_REMOVED;
      }
    }
  }
  ui8LastPriority = 0;
  ui8SchedulerStatus = ui8BufferSchedulerStatus;
  return TASK_REMOVED;
}

//! Function: Task Enabler
/*!
  Enable a task from task manager.
  \param tpTask is a task_t pointer type.
  \return Returns ERROR_TASK_NOT_ENABLED or TASK_ENABLED.
*/
uint8_t ui8EnableTask(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress]->ui8Status = ENABLED;
    ui16TaskArrayTimer[tpTask->ui8Priority][tpTask->ui8TaskAddress] = tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress]->ui16Period;
    if (ui8BufferSchedulerStatus == STOP_SCHEDULER){
      ui8SchedulerStatus = ON_HOLD;
    }
    else{
      ui8SchedulerStatus = ui8BufferSchedulerStatus;
    }
    return TASK_ENABLED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_ENABLED;
  }
}

//! Function: Task Disabler
/*!
  Disable a task from task manager.
  \param tpTask is a task_t pointer type.
  \return Returns ERROR_TASK_NOT_DISABLED or TASK_DISABLED.
*/
uint8_t ui8DisableTask(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress]->ui8Status = DISABLED;
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_DISABLED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_DISABLED;
  }
}

//! Function: Restart Timer Task
/*!
  Restart timer from a task.
  \param tpTask is a task_t pointer type.
  \return Returns TASK_TIMER_RESTARTED or TASK_TIMER_NOT_RESTARTED.
*/
uint8_t ui8RestartTimerTask(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    ui16TaskArrayTimer[tpTask->ui8Priority][tpTask->ui8TaskAddress] = tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress]->ui16Period;
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_TIMER_RESTARTED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_TIMER_NOT_RESTARTED;
  }
}

//! Function: Task Period Changer
/*!
  Change a task period from task manager.
  \param tpTask is a task_t pointer type.
  \param ui16Period is a 16-bit integer type. This is the period that task will be executed.
  \return Returns ERROR_TASK_PERIOD_NOT_CHANGED or TASK_PERIOD_CHANGED.
*/
uint8_t ui8ChangeTaskPeriod(task_t* tpTask, uint16_t ui16Period){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  uint8_t ui8TaskCounter = 0;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    ui16TaskArrayTimer[tpTask->ui8Priority][tpTask->ui8TaskAddress] = ui16Period;
    for (ui8TaskCounter = 0 ; ui8TaskCounter < AMOUNT_OF_TASK_SCHEDULED ; ui8TaskCounter++){
      if (*tppScheduledTasksVector[ui8TaskCounter] == tpTask){
        tppScheduledTasksVector[ui8TaskCounter] = NULL;
      }
    }
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_PERIOD_CHANGED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_PERIOD_NOT_CHANGED;
  }
}

//! Function: Task Period Restorer
/*!
  Change a task period from task manager.
  \param tpTask is a task_t pointer type.
  \return Returns ERROR_TASK_PERIOD_NOT_CHANGED or TASK_PERIOD_CHANGED.
*/
uint8_t ui8RestoreTaskPeriod(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    ui16TaskArrayTimer[tpTask->ui8Priority][tpTask->ui8TaskAddress] = tpTask->ui16Period;
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_PERIOD_CHANGED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_PERIOD_NOT_CHANGED;
  }
}

//! Function: Task Priorizer
/*!
  Priorize a task on task manager.
  \param tpTask is a task_t pointer type.
  \return Returns ERROR_TASK_NOT_PRIORIZED or TASK_PRIORIZED.
*/
uint8_t ui8PriorizeTask(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  uint8_t ui8TaskCounter = 0;
  if (tpTask->ui8Status != ENABLED){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_PRIORIZED;
  }
  if (tpTask->ui8Status == PRIORIZED){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_PRIORIZED;
  }
  if(ui16AmountOfPriorizedTasks < AMOUNT_OF_PRIORIZED_TASKS){
    tpTask->ui8Status = PRIORIZED;
    tpPriorizedTaskVector[ui16AmountOfPriorizedTasks] = tpTask;
    ui16AmountOfPriorizedTasks++;
    for (ui8TaskCounter = 0 ; ui8TaskCounter < AMOUNT_OF_TASK_SCHEDULED ; ui8TaskCounter++){
      if (*tppScheduledTasksVector[ui8TaskCounter] == tpTask){
        tppScheduledTasksVector[ui8TaskCounter] = NULL;
      }
    }
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_PRIORIZED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_PRIORIZED;
  }
}

//! Function: Task Depriver
/*!
  Deprive a task on task manager.
  \param tpTask is a task_t pointer type.
  \return Returns ERROR_TASK_NOT_DEPRIVED or TASK_DEPRIVED.
*/
uint8_t ui8DepriveTask(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  uint16_t ui16Counter = 0;
  uint8_t ui8TaskCounter = 0;
  if (tpTask->ui8Status != PRIORIZED){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_DEPRIVED;
  }
  for (ui16Counter = 0 ; ui16Counter < AMOUNT_OF_PRIORIZED_TASKS ; ui16Counter++){
    if(tpPriorizedTaskVector[ui16Counter] == tpTask){
      tpPriorizedTaskVector[ui16Counter] = NULL;
      tpTask->ui8Status = ENABLED;
      ui16AmountOfPriorizedTasks--;
      for (ui8TaskCounter = 0 ; ui8TaskCounter < AMOUNT_OF_TASK_SCHEDULED ; ui8TaskCounter++){
        if (*tppScheduledTasksVector[ui8TaskCounter] == tpTask){
          tppScheduledTasksVector[ui8TaskCounter] = NULL;
        }
      }
    }
    if (tpTask->ui8Status == ENABLED && ui16Counter < (AMOUNT_OF_PRIORIZED_TASKS - 1)){
      tpPriorizedTaskVector[ui16Counter] = tpPriorizedTaskVector[ui16Counter + 1];
      tpPriorizedTaskVector[ui16Counter + 1] = NULL;
    }
  }
  if (tpTask->ui8Status == ENABLED){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_DEPRIVED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_NOT_DEPRIVED;
  }
}

//! Function: Task Argument Changer
/*!
  Change a task argument from task manager.
  \param tpTask is a task_t pointer type.
  \param vpArguments is a void pointer type. This is the adress of arguments that are used in the task.
  \return Returns ERROR_TASK_ARGUMENT_NOT_CHANGED or TASK_ARGUMENT_CHANGED.
*/
uint8_t ui8ChangeTaskArgument(task_t* tpTask, void* vpArgument){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    faTaskArguments[tpTask->ui8Priority][tpTask->ui8TaskAddress] = vpArgument;
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return TASK_ARGUMENT_CHANGED;
  }
  else{
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return ERROR_TASK_ARGUMENT_NOT_CHANGED;
  }
}

//! Function: Get Current Task
/*!
  Get the current task.
  \param tpTask is a task_t pointer type.
  \return Returns the pointer of current task.
*/
task_t* tpGetCurrentTask(){
  return (task_t*) tpCurrentTask;
}

//! Function: Task Finder
/*!
  This function is a task finder.
  \param cpTaskName is a char pointer type. This is the name of task.
  \return Returns the pointer of task or NULL if the task don't exist.
*/
task_t* tpFindTask(const char* cpTaskName){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if (cpTaskName != NULL){
    uint8_t ui8PriorityCounter = 0;
    uint8_t ui8TaskCounter = 0;
    for (ui8PriorityCounter = 0 ; ui8PriorityCounter < ui8LastPriority ; ui8PriorityCounter++){
      for(ui8TaskCounter = 0 ; tpTaskArray[ui8PriorityCounter][ui8TaskCounter] != NULL ; ui8TaskCounter++){
        if (strcmp(tpTaskArray[ui8PriorityCounter][ui8TaskCounter]->cpTaskName, cpTaskName) == 0){
          ui8SchedulerStatus = ui8BufferSchedulerStatus;
          return (task_t*) tpTaskArray[ui8PriorityCounter][ui8TaskCounter];
        }
      }
    }
  }
  ui8SchedulerStatus = ui8BufferSchedulerStatus;
  return NULL;
}

//! Function: Arguments Getter
/*!
  Get the argument of a task.
  \param tpTask is a task_t pointer type.
  \return Returns the pointer of argument task or NULL if the task don't exist.
*/
void* vpGetArguments(task_t* tpTask){
  uint8_t ui8BufferSchedulerStatus = ui8SchedulerStatus;
  ui8SchedulerStatus = STOP_SCHEDULER;
  if(tpTaskArray[tpTask->ui8Priority][tpTask->ui8TaskAddress] == tpTask){
    ui8SchedulerStatus = ui8BufferSchedulerStatus;
    return faTaskArguments[tpTask->ui8Priority][tpTask->ui8TaskAddress];
  }
  ui8SchedulerStatus = ui8BufferSchedulerStatus;
  return NULL;
}

//! Function: Scheduler Interrupt
/*!
  This is the task manager: When a timer burst, this function is called. Tasks that have their time equal to 0 are stored in a FIFO buffer and are scheduled to run.
*/
void vSchedulerInterrupt(void){
  vSystemWakeUp();
  if (ui8SchedulerStatus == STOP_SCHEDULER){
    return;
  }
  static uint8_t ui8PriorityCounter = 0;
  static uint8_t ui8TaskCounter = 0;
  static uint16_t ui16PriorizedTaskCounter = 0;
  ui16SystemTimer++;
  if (ui16SystemTimer == SOFTWARE_WATCHDOG_TIME && ui8SchedulerStatus == RUNNING_TASK){
    vSystemRestart();
  }
  for (ui16PriorizedTaskCounter = 0 ; tpPriorizedTaskVector[ui16PriorizedTaskCounter] != NULL ; ui16PriorizedTaskCounter++){
    ui8PriorityCounter = tpPriorizedTaskVector[ui16PriorizedTaskCounter]->ui8Priority;
    ui8TaskCounter = tpPriorizedTaskVector[ui16PriorizedTaskCounter]->ui8TaskAddress;
    if(ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] == 0){
      if(ui8GetAmountOfPendingData(bpScheduledTasks) < AMOUNT_OF_TASK_SCHEDULED){
        ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] = tpPriorizedTaskVector[ui16PriorizedTaskCounter]->ui16Period;
        task_t* tpBuffer = (task_t*) tpPriorizedTaskVector[ui16PriorizedTaskCounter];
        vPushBufferData(bpScheduledTasks, &tpBuffer);
        faScheduledTasksArguments[ui8GetWritePosition(bpScheduledTasks)] = faTaskArguments[ui8PriorityCounter][ui8TaskCounter];
      }
    }
    else{
      if (ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] > (uint8_t) (ui8TickMS << 2)){
        ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] -= (ui8TickMS << 2);
      }
      else{
        ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] = 0;
      }
    }
  }
  for (ui8PriorityCounter = 0 ; ui8PriorityCounter <= ui8LastPriority ; ui8PriorityCounter++){
    for (ui8TaskCounter = 0 ; tpTaskArray[ui8PriorityCounter][ui8TaskCounter] != NULL ; ui8TaskCounter++){
      if (tpTaskArray[ui8PriorityCounter][ui8TaskCounter]->ui8Status == ENABLED){
        if(ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] == 0){
          if (ui8GetAmountOfPendingData(bpScheduledTasks) < AMOUNT_OF_TASK_SCHEDULED && tpTaskArray[ui8PriorityCounter][ui8TaskCounter] != NULL){
            ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] = tpTaskArray[ui8PriorityCounter][ui8TaskCounter]->ui16Period;
            task_t* tpBuffer = (task_t*) tpTaskArray[ui8PriorityCounter][ui8TaskCounter];
            vPushBufferData(bpScheduledTasks, &tpBuffer);
            faScheduledTasksArguments[ui8GetWritePosition(bpScheduledTasks)] = faTaskArguments[ui8PriorityCounter][ui8TaskCounter];
          }
        }
        else{
          if (ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] > ui8TickMS){
            ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] -= ui8TickMS;
          }
          else{
            ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] = 0;
          }
        }
      }
    }
  }
}

//! Function: Scheduler Starter
/*!
  If have a pending task on buffer, the task runs.
*/
void vStartScheduler(){
  task_t** tppBuffer = (task_t**) vpPullBufferData(bpScheduledTasks);
  while (ui8GetAmountOfPendingData(bpScheduledTasks) > 0 && *tppBuffer != NULL && (*tppBuffer)->ui8Status == DISABLED){
    tppBuffer = (task_t**) vpPullBufferData(bpScheduledTasks);
  }
  if (tppBuffer != NULL){
    if ((*tppBuffer)->ui8Status == PRIORIZED || (*tppBuffer)->ui8Status == ENABLED){
      void* vpBufferArguments = faScheduledTasksArguments[ui8GetReadPosition(bpScheduledTasks)];
      tpCurrentTask = *tppBuffer;
      ui16SystemTimer = 0;
      ui8SchedulerStatus = RUNNING_TASK;
      vSystemRestartTimerInit();
      register uint8_t ui8TaskReturn = tpCurrentTask->pfFunction(vpBufferArguments);
      vSystemRestartTimerStop();
      if (ui8SchedulerStatus == RUNNING_TASK){
        ui8SchedulerStatus = ON_HOLD;
      }
      ui16SystemTimer = 0;
      vCheckTaskReturn(ui8TaskReturn);
    }
    else{
      vSystemSleep();
    }
  }
  else{
    vSystemSleep();
  }
}

//! Function: All Tasks Enabler
/*!
  Enable all tasks from task manager.
*/
void vEnableAllTasks(void){
  ui8SchedulerStatus = STOP_SCHEDULER;
  uint8_t ui8PriorityCounter = 0;
  uint8_t ui8TaskCounter = 0;
  for (ui8PriorityCounter = 0 ; ui8PriorityCounter < ui8LastPriority ; ui8PriorityCounter++){
    for(ui8TaskCounter = 0 ; tpTaskArray[ui8PriorityCounter][ui8TaskCounter] != NULL ; ui8TaskCounter++){
      tpTaskArray[ui8PriorityCounter][ui8TaskCounter]->ui8Status = ENABLED;
      ui16TaskArrayTimer[ui8PriorityCounter][ui8TaskCounter] = tpTaskArray[ui8PriorityCounter][ui8TaskCounter]->ui16Period;
    }
  }
  ui8SchedulerStatus = ON_HOLD;
}

//! Function: All Tasks Disabler
/*!
  Disable all tasks from task manager.
*/
void vDisableAllTasks(void){
  ui8SchedulerStatus = STOP_SCHEDULER;
  uint8_t ui8PriorityCounter = 0;
  uint8_t ui8TaskCounter = 0;
  for (ui8PriorityCounter = 0 ; ui8PriorityCounter < ui8LastPriority ; ui8PriorityCounter++){
    for(ui8TaskCounter = 0 ; tpTaskArray[ui8PriorityCounter][ui8TaskCounter] != NULL ; ui8TaskCounter++){
      tpTaskArray[ui8PriorityCounter][ui8TaskCounter]->ui8Status = DISABLED;
    }
  }
}

//! Function: Scheduler Starter
/*!
  This function is called to check the return of the tasks that were performed. Decisions are made based on this return. Edit this function if it does not fully fit your project.
*/
void vCheckTaskReturn(uint8_t ui8TaskReturn){
  switch (ui8TaskReturn) {

    default:
      vSystemRestart();
      break;

    case TASK_END:
      vSystemSleep();
      break;

    case INSUFICIENT_MEMORY:
      vSystemRestart();
      break;

    case INVALID_ARGUMENTS:
      break;

    case NO_SLEEP:
      break;

    case SYSTEM_RESTART:
      vSystemRestart();
      break;
  }
}
