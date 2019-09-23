#include "./Task.h"

/*!
  Task object "constructor".
*/
const task_manager_t Task PROGMEM = {
  .vDelete = &vDeleteTask,
  .vDisable = &vDisableTask,
  .vEnable = &vEnableTask,
  .vForceExecution = &vForceTaskExecution,
  .vRestartTimer = &vRestartTimerTask,
  .vEnableAll = &vEnableAllTasks,
  .vDisableAll = &vDisableAllTasks,
  .vpGetArguments = &vpGetTaskArguments,
  .vChangeArguments = &vChangeTaskArguments,
  .tFind = &tFindTask,
  .vChangePeriod = &vChangeTaskPeriod,
  .vChangePriority = &vChangeTaskPriority
};