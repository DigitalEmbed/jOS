#include "./Binary.h"

extern uint16_t ui16pResetTime[AMOUNT_OF_SEMAPHORES];

//! Function: Semaphore Taker
/*!
  Try take a semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \return Returns TASK_HOLDER or SEMAPHORE_BUSY.
*/
uint8_t ui8TakeBinarySemaphore(semaphore_t* smpSemaphore){
  if (smpSemaphore->tTaskHolder == NULL){
    smpSemaphore->tTaskHolder = tCurrentTask;
    smpSemaphore->ui8SemaphoreStatus = SEMAPHORE_BUSY;
    ui16pResetTime[smpSemaphore->ui8SemaphoreAddress] = smpSemaphore->ui16ResetTime;
  }
  if (smpSemaphore->tTaskHolder == tCurrentTask){
    return TASK_HOLDER;
  }
  return SEMAPHORE_BUSY;
}

//! Function: Semaphore Retorner
/*!
  Tries return a semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \return Returns RETURNED_SEMAPHORE or SEMAPHORE_BUSY.
*/
uint8_t ui8ReturnBinarySemaphore(semaphore_t* smpSemaphore){
  if (smpSemaphore->tTaskHolder == tCurrentTask){
    smpSemaphore->ui8SemaphoreStatus = SEMAPHORE_IDLE;
    smpSemaphore->tTaskHolder = NULL;
    ui16pResetTime[smpSemaphore->ui8SemaphoreAddress] = smpSemaphore->ui16ResetTime;
    return RETURNED_SEMAPHORE;
  }
  return SEMAPHORE_BUSY;
}
