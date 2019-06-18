#include "Binnary.h"

extern uint16_t ui16pResetTime[AMOUNT_OF_SEMAPHORES];

//! Function: Semaphore Taker
/*!
  Try take a semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \return Returns TASK_HOLDER or SEMAPHORE_BUSY.
*/
uint8_t ui8TakeBinnarySemaphore(semaphore_t* smpSemaphore){
  if (smpSemaphore->tpTaskHolder == NULL){
    smpSemaphore->tpTaskHolder = tpGetCurrentTask();
    smpSemaphore->ui8SemaphoreStatus = SEMAPHORE_BUSY;
    ui16pResetTime[smpSemaphore->ui8SemaphoreAddress] = smpSemaphore->ui16ResetTime;
  }
  if (smpSemaphore->tpTaskHolder == tpGetCurrentTask()){
    return TASK_HOLDER;
  }
  return SEMAPHORE_BUSY;
}

//! Function: Semaphore Retorner
/*!
  Try return a semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \return Returns RETURNED_SEMAPHORE or SEMAPHORE_BUSY.
*/
uint8_t ui8ReturnBinnarySemaphore(semaphore_t* smpSemaphore){
  if (smpSemaphore->tpTaskHolder == tpGetCurrentTask()){
    smpSemaphore->ui8SemaphoreStatus = SEMAPHORE_IDLE;
    smpSemaphore->tpTaskHolder = NULL;
    ui16pResetTime[smpSemaphore->ui8SemaphoreAddress] = smpSemaphore->ui16ResetTime;
    return RETURNED_SEMAPHORE;
  }
  return SEMAPHORE_BUSY;
}
