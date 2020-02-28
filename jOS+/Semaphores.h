//! jOS Version 3.0b
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

#if defined (__AVR)
  #include <avr/pgmspace.h>
#endif

//! Type Definition: semaphore_manager_t
/*!
  This is a "class" of semaphore_manager_t type.
*/
typedef struct {
  semaphore_status_t (*take)(semaphore_t smObjectSemaphore);                    /*!< uint8_t "method". */
  semaphore_status_t (*giveBack)(semaphore_t smObjectSemaphore);                /*!< uint8_t "method". */
} semaphore_manager_t;

#if defined(__AVR)
  extern const semaphore_manager_t Semaphore PROGMEM;                           /*!< Semaphore manager "object". */
#else
  extern const semaphore_manager_t Semaphore;                                   /*!< Semaphore manager "object". */
#endif

#ifdef __cplusplus
  }
#endif

#endif
