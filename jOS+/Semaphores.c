#include <jOS.h>

#if defined(__SEMAPHORES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SEMAPHORES__) &&\
defined(__MINIMUM_SEMAPHORE_TIMEOUT_MS__) && (__AMOUNT_OF_SEMAPHORES__ > 0) && (__MINIMUM_SEMAPHORE_TIMEOUT_MS__ > 0)

  #include "./Semaphores.h"

  /*!
    Semaphore object "constructor".
  */
  #if defined(__AVR__)
    const semaphore_manager_t Semaphore PROGMEM = {
  #else
    const semaphore_manager_t Semaphore = {
  #endif
      .take = Semaphore_take,
      .giveBack = Semaphore_return,
  };

#endif