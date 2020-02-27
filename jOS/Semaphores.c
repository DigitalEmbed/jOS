#include "./Configs.h"

#if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)

  #include "./Semaphores.h"
  #include "./Binary.h"
  #include <EmbeddedTools.h>

  //! Type Enumeration: Semaphore Watchdog Status
  /*!
    These macros are for facilitate the use of this library.
  */
  typedef enum{
    SEMAPHORE_WATCHDOG_STATUS_ENABLED = 0,
    SEMAPHORE_WATCHDOG_STATUS_DISABLED
  } semaphore_watchdog_status_t;

  static uint8_t __ui8AmountOfSemaphores = 0;                                                         /*!< 8-bit integer type. */
  static semaphore_structure_t __smSemaphoresVector[__AMOUNT_OF_SEMAPHORES__] = {0};                  /*!< semaphore_t pointer type. */
  static semaphore_watchdog_status_t __smwWatchdogStatus = SEMAPHORE_WATCHDOG_STATUS_DISABLED;        /*!< semaphore_watchdog_status_t type. */
  static void (*__vfWatchdogCallback)(void* vpArguments) = NULL;                                      /*!< void function pointer type. */
  static void* __vpWatchdogCallbackArguments = NULL;                                                  /*!< void pointer type. */

  void ISR_semaphoresWatchdog(void);                                                                  /*!< void type function. */

  #if defined (__AUTO_INITIALIZATION_ENABLE__)
    void __Semaphores_autoInitWatchdog(void) ATTRIBUTE_CONSTRUCTOR(10);                               /*!< void type function. */
  #endif

  //! Function: Semaphores Interrupt
  /*!
    This is the semaphore manager: When a timer burst, this function is called. When semaphore time is 0, the system are restarted.
  */
  void ISR_semaphoresWatchdog(void){
    uint8_t ui8Counter = 0;
    if(__ui8AmountOfSemaphores < 1 || __smwWatchdogStatus == SEMAPHORE_WATCHDOG_STATUS_DISABLED){
      return;
    }
    for (ui8Counter = 0 ; __smSemaphoresVector[ui8Counter].smsSemaphoreStatus != SEMAPHORE_STATUS_EMPTY ; ui8Counter++){
      if (__smSemaphoresVector[ui8Counter].smsSemaphoreStatus == SEMAPHORE_STATUS_BUSY){
        if (__smSemaphoresVector[ui8Counter].ui16TimeCounter < __ui8SemaphoreTickMS){
          if (__vfWatchdogCallback != NULL){
            __vfWatchdogCallback(__vpWatchdogCallbackArguments);
          }
          System_restart();
        }
        else{
          __smSemaphoresVector[ui8Counter].ui16TimeCounter--;
        }
      }
    }
  }

  //! Function: Semaphore Manager Initializer
  /*!
    Initializes the semaphore manager.
  */
  void Semaphore_initWatchdog(void){
    System_semaphoreTimerConfiguration(&ISR_semaphoresWatchdog);
    __smwWatchdogStatus = SEMAPHORE_WATCHDOG_STATUS_ENABLED;
  }

  //! Function: Semaphore Watchdog Callback Attacher
  /*!
    Attaches a callback function on semaphore watchdog.
    \param vfWatchdogCallback is a void function pointer.
    \param vpWatchdogCallbackArguments is a void pointer.
  */
  void Semaphore_attachWatchdogCallback(void (*vfWatchdogCallback)(void*), void* vpWatchdogCallbackArguments){
    __vfWatchdogCallback = vfWatchdogCallback;
    __vpWatchdogCallbackArguments = vpWatchdogCallbackArguments;
  }

  //! Function: Semaphore Watchdog Callback Detacher
  /*!
    Detacher a callback function on semaphore watchdog.
  */
  void Semaphore_detachWatchdogCallback(void){
    Semaphore_attachWatchdogCallback(NULL, NULL);
  }

  //! Function: Semaphore Initializer
  /*!
    Initializes a Semaphore.
    \param smSemaphore is a semaphore_t pointer type.
    \param smtSemaphoreType is a 8-bit integer.
    \return Returns NO_SUCH_MEMORY_FOR_SEMAPHORE or SEMAPHORE_INITIALIZED.
  */
  semaphore_status_t (newSemaphore)(semaphore_t* smSemaphore, semaphore_type_t smtSemaphoreType, uint16_t ui16Timeout){
    if (__ui8AmountOfSemaphores >= __AMOUNT_OF_SEMAPHORES__){
      return SEMAPHORE_STATUS_ERROR;
    }
    __smSemaphoresVector[__ui8AmountOfSemaphores].smsSemaphoreStatus = SEMAPHORE_STATUS_IDLE;
    __smSemaphoresVector[__ui8AmountOfSemaphores].smtSemaphoreType = smtSemaphoreType;
    __smSemaphoresVector[__ui8AmountOfSemaphores].tTaskHolder = NULL;
    __smSemaphoresVector[__ui8AmountOfSemaphores].ui16Timeout = (ui16Timeout < __MINIMUM_SEMAPHORE_TIMEOUT_MS__) ? (__MINIMUM_SEMAPHORE_TIMEOUT_MS__)/__ui8SemaphoreTickMS : ui16Timeout/__ui8SemaphoreTickMS;
    *smSemaphore = &(__smSemaphoresVector[__ui8AmountOfSemaphores]);
    __ui8AmountOfSemaphores++;
    return SEMAPHORE_STATUS_CREATED;
  }

  //! Function: Semaphore Taker
  /*!
    Try take a semaphore.
    \param smSemaphore is a semaphore_t pointer type.
    \return Returns TASK_HOLDER or SEMAPHORE_BUSY.
  */
  semaphore_status_t Semaphore_take(semaphore_t smSemaphore){
    if (smSemaphore != NULL && __smwWatchdogStatus == SEMAPHORE_WATCHDOG_STATUS_ENABLED){
      switch (smSemaphore->smtSemaphoreType){
        case SEMAPHORE_TYPE_BINARY:
          return BinarySemaphore_take(smSemaphore);
        case SEMAPHORE_TYPE_COUNTING:
          return SEMAPHORE_STATUS_ERROR;
        case SEMAPHORE_TYPE_MUTEX:
          return SEMAPHORE_STATUS_ERROR;
        default:
          return SEMAPHORE_STATUS_ERROR;
      }
    }
    return SEMAPHORE_STATUS_ERROR;
  }

  //! Function: Semaphore Retorner
  /*!
    Try return a semaphore.
    \param smSemaphore is a semaphore_t pointer type.
    \return Returns RETURNED_SEMAPHORE or SEMAPHORE_BUSY.
  */
  semaphore_status_t Semaphore_return(semaphore_t smSemaphore){
    if (smSemaphore != NULL && __smwWatchdogStatus == SEMAPHORE_WATCHDOG_STATUS_ENABLED){
      switch (smSemaphore->smtSemaphoreType){
        case SEMAPHORE_TYPE_BINARY:
          return BinarySemaphore_return(smSemaphore);
        case SEMAPHORE_TYPE_COUNTING:
          return SEMAPHORE_STATUS_ERROR;
        case SEMAPHORE_TYPE_MUTEX:
          return SEMAPHORE_STATUS_ERROR;
        default:
          return SEMAPHORE_STATUS_ERROR;
      }
    }
    return SEMAPHORE_STATUS_ERROR;
  }

  #if defined (__AUTO_INITIALIZATION_ENABLE__)
    //! Function: Semaphore Manager Auto Initializer
    /*!
      Initializes the semaphore manager.
    */
    void __Semaphores_autoInitWatchdog(void){
      Semaphore_initWatchdog();
    }
  #endif

#endif