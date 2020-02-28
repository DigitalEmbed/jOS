#include "./Task.h"

/*!
  Task object "constructor".
*/

#if defined(__AVR)
  const task_manager_t Task PROGMEM = {
#else
  const task_manager_t Task = {
#endif
  .disable = &Task_disable,
  .enable = &Task_enable,
  .forceExecution = &Task_forceExecution,
  .restartTimer = &Task_restartTimer,
  .enableAll = &Scheduler_enableAllTasks,
  .disableAll = &Scheduler_disableAllTasks,
  .getArguments = &Task_getArguments,
  .setArguments = &Task_setArguments,
  .find = &Task_find,
  .setPeriod = &Task_setPeriod,
  .setPriority = &Task_setPriority
};