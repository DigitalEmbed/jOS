//! winTerface Version 1.0b
/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copyleft license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -> Commercial use;
    -> Modification;
    -> Distribuition;
    -> Patent Use;
    -> Private Use;

  * Limitations:
    -> Liability;
    -> Warranty;

  * Conditions:
    -> License and copyright notice;
    -> Disclose source;
    -> State changes;
    -> Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#ifndef Semaphores_Class_h
#define Semaphores_Class_h

#ifdef __cplusplus
  extern "C" {
#endif

#include <jOS.h>
#include <avr/pgmspace.h>

//! Macro: semaphore_t "object constructor"
/*!
  "Construct" a semaphore_t "object".
  \return Returns the object.
*/
#define   NewSemaphore()   {0}

//! Type Definition: semaphore_manager_t
/*!
  This is a "class" of semaphore_manager_t type.
*/
typedef struct {
  uint8_t (*ui8Begin)(semaphore_t* smObjectSemaphore, uint8_t ui8SemaphoreType, uint16_t ui16ResetTime);            /*!< uint8_t "method". */
  uint8_t (*ui8Take)(semaphore_t* smObjectSemaphore);                                                               /*!< uint8_t "method". */
  uint8_t (*ui8Return)(semaphore_t* smObjectSemaphore);                                                             /*!< uint8_t "method". */
  void (*vSetWatchdog)(void);                                                                                       /*!< void "method". */
} semaphore_manager_t;

extern const semaphore_manager_t Semaphore PROGMEM;                                                                 /*!< Semaphore manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
