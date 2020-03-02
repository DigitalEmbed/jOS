#include <jOS.h>

#if defined(__TASK_SCHEDULER_ENABLE__) &&\
defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)

  #include "./Task.h"

  /*!
    Prototype private "methods".
  */
  static void __delayMS(uint16_t ui16TimeMS);
  static void __waitFunction(bool bCondition);
  static void __yield(void);

  #if defined(__AVR__)
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
    .setPriority = &Task_setPriority,
    .delayMS = &__delayMS,
    .waitFor = &__waitFunction,
    .waitUntil = &__waitFunction,
    .yield = &__yield,
  };

  static void __delayMS(uint16_t ui16TimeMS){}
  static void __waitFunction(bool bCondition){}
  static void __yield(void) {}

#endif
