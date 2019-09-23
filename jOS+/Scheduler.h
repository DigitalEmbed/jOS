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

#ifndef Scheduler_Class_H
#define Scheduler_Class_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <jOS.h>
#include <avr/pgmspace.h>

//! Type Definition: scheduler_manager_t
/*!
  This is a "class" of scheduler_manager_t type.
*/
typedef struct {
  void (*vSetRoundRobinMode)(void);                                   /*!< void "method". */
  void (*vSetRelativeMode)(void);                                     /*!< void "method". */
  void (*vSetAbsoluteMode)(void);                                     /*!< void "method". */
  void (*vSetFullAbsoluteMode)(void);                                 /*!< void "method". */
  void (*vStart)(void);                                               /*!< void "method". */
} scheduler_manager_t;

extern const scheduler_manager_t Scheduler PROGMEM;                   /*!< Scheduler manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
