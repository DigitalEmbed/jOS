#include "./Scheduler.h"

/*!
  Prototype public "methods".
*/
static void vPublicStartScheduler(void);
static void vPublicSetRoundRobinScheduler(void);
static void vPublicSetAbsoluteScheduler(void);
static void vPublicSetFullAbsoluteScheduler(void);
static void vPublicStartScheduler(void);
static void vPublicSetRelativeScheduler(void);

/*!
  Scheduler object "constructor".
*/
const scheduler_manager_t Scheduler PROGMEM = {
  .vSetRoundRobinMode = &vPublicSetRoundRobinScheduler,
  .vSetRelativeMode = &vPublicSetRelativeScheduler,
  .vSetAbsoluteMode = &vPublicSetAbsoluteScheduler,
  .vSetFullAbsoluteMode = &vPublicSetFullAbsoluteScheduler,
  .vStart = &vPublicStartScheduler
};


//! Public Method: Round Robin Scheduler Setter
/*!
  Defines round robin algorithm on scheduler.
*/
static void vPublicSetRoundRobinScheduler(void){
  vEnableScheduler(ROUND_ROBIN_MODE);
}

//! Public Method: Relative Priority Scheduler Setter
/*!
  Defines relative priority algorithm on scheduler.
*/
static void vPublicSetRelativeScheduler(void){
  vEnableScheduler(RELATIVE_PRIORITY_MODE);
}

//! Public Method: Absolute Priority Scheduler Setter
/*!
  Defines absolute priority algorithm on scheduler.
*/
static void vPublicSetAbsoluteScheduler(void){
  vEnableScheduler(ABSOLUTE_PRIORITY_MODE);
}

//! Public Method: Full Absolute Priority Scheduler Setter
/*!
  Defines full absolute priority algorithm on scheduler.
*/
static void vPublicSetFullAbsoluteScheduler(void){
  vEnableScheduler(FULL_PRIORITY_MODE);
}

//! Public Method: Scheduler Starter
/*!
  Start task scheduler.
*/
static void vPublicStartScheduler(void){
  while(1){
    vStartScheduler();
  }
}