#include <jOS.h>

#if defined(__TASK_SCHEDULER_ENABLE__) &&\
defined(__AMOUNT_OF_TASKS__) && (__AMOUNT_OF_TASKS__ > 0) &&\
defined(__TASK_NAME_SIZE__) && (__TASK_NAME_SIZE__ > 0) &&\
defined(__MINIMUM_THREAD_PRIORITY__) && (__MINIMUM_THREAD_PRIORITY__ > 0) &&\
defined(__MINIMUM_TASK_TIMEOUT_MS__) && (__MINIMUM_TASK_TIMEOUT_MS__ > 0) &&\
defined(__SCHEDULER_DEFAULT_MODE__) && (__SCHEDULER_DEFAULT_MODE__ >= SCHEDULER_MODE_ROUND_ROBIN) && (__SCHEDULER_DEFAULT_MODE__ <= SCHEDULER_MODE_FULL_PRIORITY)

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
  #if defined(__AVR__)
    const scheduler_manager_t Scheduler PROGMEM = {
  #else
    const scheduler_manager_t Scheduler = {
  #endif
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

      #if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
      defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)
        .Semaphores = {
          .attachCallback = &Semaphore_attachWatchdogCallback,
          .detachCallback = &Semaphore_detachWatchdogCallback
        },
      #endif

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
    
    #if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
    defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)
      Semaphore_initWatchdog();
    #endif

    Scheduler_enable();
    while(1){
      Scheduler_start();
    }
  }

#endif
