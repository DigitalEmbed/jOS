#include "./Semaphores.h"

/*!
  Semaphore object "constructor".
*/
const semaphore_manager_t Semaphore PROGMEM = {
    .take = Semaphore_take,
    .giveBack = Semaphore_return,
};