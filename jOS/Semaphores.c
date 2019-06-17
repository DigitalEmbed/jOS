#include "Semaphores.h"
#include "TaskScheduler.h"
#include "jOS.h"
#include "Binnary.h"

//! Function: Semaphore Initializer
/*!
  Initialize a Semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \param ui8SemaphoreType is a 8-bit integer.
  \return Returns NO_SUCH_MEMORY_FOR_SEMAPHORE or SEMAPHORE_INITIALIZED.
*/
uint8_t ui8CreateSemaphore(semaphore_t* smpSemaphore, uint8_t ui8SemaphoreType){
  smpSemaphore->ui8SemaphoreStatus = SEMAPHORE_IDLE;
  smpSemaphore->ui8SemaphoreType = ui8SemaphoreType;
  smpSemaphore->tlpPendingTasksList = NULL;
  smpSemaphore->tpTaskHolder = NULL;
  smpSemaphore->ui8TaskHolderPriority = 0;
  smpSemaphore->ui16TaskHolderPeriod = 0;
  if (smpSemaphore->ui8SemaphoreType != BINNARY){
    smpSemaphore->tlpPendingTasksList = lpCreateTypedList(sizeof(void*));
    if (smpSemaphore->tlpPendingTasksList == NULL){
      return NO_SUCH_MEMORY_FOR_SEMAPHORE;
    }
  }
  return SEMAPHORE_INITIALIZED;
}

//! Function: Semaphore Taker
/*!
  Try take a semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \return Returns TASK_HOLDER or SEMAPHORE_BUSY.
*/
uint8_t ui8TakeSemaphore(semaphore_t* smpSemaphore){
  if (smpSemaphore != NULL){
    switch (smpSemaphore->ui8SemaphoreType){
      case BINNARY:
        return ui8TakeBinnarySemaphore(smpSemaphore);
      case COUNTING:
        return SEMAPHORE_NOT_INITIALIZED;
      case MUTEX:
        return SEMAPHORE_NOT_INITIALIZED;
      default:
        return SEMAPHORE_NOT_INITIALIZED;
    }
  }
  return SEMAPHORE_NOT_INITIALIZED;
}

//! Function: Semaphore Retorner
/*!
  Try return a semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \return Returns RETURNED_SEMAPHORE or SEMAPHORE_BUSY.
*/
uint8_t ui8ReturnSemaphore(semaphore_t* smpSemaphore){
  if (smpSemaphore != NULL){
    switch (smpSemaphore->ui8SemaphoreType){
      case BINNARY:
        return ui8ReturnBinnarySemaphore(smpSemaphore);
      case COUNTING:
        return SEMAPHORE_NOT_INITIALIZED;
      case MUTEX:
        return SEMAPHORE_NOT_INITIALIZED;
      default:
        return SEMAPHORE_NOT_INITIALIZED;
    }
  }
  return SEMAPHORE_NOT_INITIALIZED;
}
