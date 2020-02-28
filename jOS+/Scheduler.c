#include "./Scheduler.h"

/*!
  Prototype public "methods".
*/
static void __Scheduler_start(void);
static void __Scheduler_setRoundRobin(void);
static void __Scheduler_setAbsolute(void);
static void __Scheduler_setFullAbsolute(void);
static void __Scheduler_start(void);
static void __Scheduler_setRelative(void);

/*!
  Scheduler object "constructor".
*/
const scheduler_manager_t Scheduler PROGMEM = {
  .setRoundRobinMode = &__Scheduler_setRoundRobin,
  .setRelativeMode = &__Scheduler_setRelative,
  .setAbsoluteMode = &__Scheduler_setAbsolute,
  .setFullAbsoluteMode = &__Scheduler_setFullAbsolute,
  .start = &__Scheduler_start,
  .Watchdog = {
    .Tasks = {
      .attachCallback = &Scheduler_attachWatchdogCallback,
      .detachCallback = &Scheduler_detachWatchdogCallback
    },
    .Semaphores = {
      .attachCallback = &Semaphore_attachWatchdogCallback,
      .detachCallback = &Semaphore_detachWatchdogCallback
    },
  },
};

//! Public Method: Round Robin Scheduler Setter
/*!
  Defines round robin algorithm on scheduler.
*/
static void __Scheduler_setRoundRobin(void){
  Scheduler_setMode(SCHEDULER_MODE_ROUND_ROBIN);
}

//! Public Method: Relative Priority Scheduler Setter
/*!
  Defines relative priority algorithm on scheduler.
*/
static void __Scheduler_setRelative(void){
  Scheduler_setMode(SCHEDULER_MODE_RELATIVE_PRIORITY);
}

//! Public Method: Absolute Priority Scheduler Setter
/*!
  Defines absolute priority algorithm on scheduler.
*/
static void __Scheduler_setAbsolute(void){
  Scheduler_setMode(SCHEDULER_MODE_ABSOLUTE_PRIORITY);
}

//! Public Method: Full Absolute Priority Scheduler Setter
/*!
  Defines full absolute priority algorithm on scheduler.
*/
static void __Scheduler_setFullAbsolute(void){
  Scheduler_setMode(SCHEDULER_MODE_FULL_PRIORITY);
}

//! Public Method: Scheduler Starter
/*!
  Start task scheduler.
*/
static void __Scheduler_start(void){
  Semaphore_initWatchdog();
  Scheduler_enable();
  while(1){
    Scheduler_start();
  }
}