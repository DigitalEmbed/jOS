#include "./Semaphores.h"

/*!
  Semaphore object "constructor".
*/
const semaphore_manager_t Semaphore PROGMEM = {
    .ui8Begin = ui8CreateSemaphore,
    .ui8Take = ui8TakeSemaphore,
    .ui8Return = ui8ReturnSemaphore,
    .vSetWatchdog = &vSemaphoreManagerInit
};