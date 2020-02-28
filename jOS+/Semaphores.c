#include "./Semaphores.h"

/*!
  Semaphore object "constructor".
*/
#if defined(__AVR)
  const semaphore_manager_t Semaphore PROGMEM = {
#else
  const semaphore_manager_t Semaphore = {
#endif
    .take = Semaphore_take,
    .giveBack = Semaphore_return,
};