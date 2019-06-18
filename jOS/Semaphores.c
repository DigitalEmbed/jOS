#include "Semaphores.h"
#include "TaskScheduler.h"
#include "jOS.h"
#include "Binnary.h"
#include "System.h"

uint16_t ui16pResetTime[AMOUNT_OF_SEMAPHORES] = {0};                            /*!< 16-bit integer type. */
semaphore_t* smpSemaphoresVector[AMOUNT_OF_SEMAPHORES] = {NULL};                /*!< semaphore_t pointer type. */
uint8_t ui8AmountOfSemaphores = 0;                                              /*!< 8-bit integer type. */

void vSemaphoresInterrupt(void);                                                /*!< void type function. */

//! Function: Semaphores Interrupt
/*!
  This is the semaphore manager: When a timer burst, this function is called. When semaphore time is 0, the system are restarted.
*/
void vSemaphoresInterrupt(void){
  uint8_t ui8Counter = 0;
  if(ui8AmountOfSemaphores < 1){
    return;
  }
  for (ui8Counter = 0 ; smpSemaphoresVector[ui8Counter] != NULL ; ui8Counter++){
    if (smpSemaphoresVector[ui8Counter]->ui8SemaphoreStatus != SEMAPHORE_IDLE){
      if (ui16pResetTime[ui8Counter] < ui8TickMS){
        vSystemRestart();
      }
      else{
        ui16pResetTime[ui8Counter] -= ui8TickMS;
      }
    }
  }
}

//! Function: Semaphore Manager Initializer
/*!
  Initialize the semaphore manager.
*/
void vSemaphoreManagerInit(void){
  vSystemTimerSemaphoresInterruption(&vSemaphoresInterrupt);
}

//! Function: Semaphore Initializer
/*!
  Initialize a Semaphore.
  \param smpSemaphore is a semaphore_t pointer type.
  \param ui8SemaphoreType is a 8-bit integer.
  \return Returns NO_SUCH_MEMORY_FOR_SEMAPHORE or SEMAPHORE_INITIALIZED.
*/
uint8_t ui8CreateSemaphore(semaphore_t* smpSemaphore, uint8_t ui8SemaphoreType, uint16_t ui16ResetTime){
  if (ui8AmountOfSemaphores >= AMOUNT_OF_SEMAPHORES){
    return NO_SUCH_MEMORY_FOR_SEMAPHORE;
  }
  smpSemaphore->ui8SemaphoreStatus = SEMAPHORE_IDLE;
  smpSemaphore->ui8SemaphoreType = ui8SemaphoreType;
  smpSemaphore->tlpPendingTasksList = NULL;
  smpSemaphore->tpTaskHolder = NULL;
  smpSemaphore->ui16ResetTime = ui16ResetTime;
  smpSemaphore->ui8SemaphoreAddress = ui8AmountOfSemaphores;
  smpSemaphoresVector[ui8AmountOfSemaphores] = smpSemaphore;
  ui8AmountOfSemaphores++;
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
