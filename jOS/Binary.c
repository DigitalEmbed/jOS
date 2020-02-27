#include "./Configs.h"

  #if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
  defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)

  #include "./Binary.h"

  extern uint16_t ui16pResetTime[__AMOUNT_OF_SEMAPHORES__];

  //! Function: Semaphore Taker
  /*!
    Tries take a semaphore.
    \param smSemaphore is a semaphore_t pointer type.
    \return Returns TASK_HOLDER or SEMAPHORE_BUSY.
  */
  semaphore_status_t BinarySemaphore_take(semaphore_t smSemaphore){
    if (smSemaphore->tTaskHolder == NULL){
      smSemaphore->tTaskHolder = __tCurrentTask;
      smSemaphore->smsSemaphoreStatus = SEMAPHORE_STATUS_BUSY;
      smSemaphore->ui16TimeCounter = smSemaphore->ui16Timeout;
    }
    if (smSemaphore->tTaskHolder == __tCurrentTask){
      return SEMAPHORE_STATUS_TASK_HOLDER;
    }
    return SEMAPHORE_STATUS_BUSY;
  }

  //! Function: Semaphore Retorner
  /*!
    Tries return a semaphore.
    \param smSemaphore is a semaphore_t pointer type.
    \return Returns RETURNED_SEMAPHORE or SEMAPHORE_BUSY.
  */
  semaphore_status_t BinarySemaphore_return(semaphore_t smSemaphore){
    if (smSemaphore->tTaskHolder == __tCurrentTask){
      smSemaphore->smsSemaphoreStatus = SEMAPHORE_STATUS_IDLE;
      smSemaphore->tTaskHolder = NULL;
      return SEMAPHORE_STATUS_RETURNED_SEMAPHORE;
    }
    return SEMAPHORE_STATUS_BUSY;
  }

#endif
