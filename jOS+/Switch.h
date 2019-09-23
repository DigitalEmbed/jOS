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

#ifndef Switch_Class_h
#define Switch_Class_h

#ifdef __cplusplus
  extern "C" {
#endif

#include <jOS.h>
#include <avr/pgmspace.h>

//! Macro: switch_t "object constructor"
/*!
  "Construct" a switch_t "object".
  \return Returns the object.
*/
#define   NewSwitch()   0

//! Type Definition: switch_manager_t
/*!
  This is a "class" of switch_manager_t type.
*/
typedef struct {
  uint8_t (*ui8Begin)(switch_t* swpObjectSwitch);                     /*!< uint8_t "method". */
  void (*vTurnOn)(switch_t* swObjectSwitch);                          /*!< void "method". */
  void (*vTurnOff)(switch_t* swObjectSwitch);                         /*!< void "method". */
  uint8_t (*ui8GetStatus)(switch_t* swObjectSwitch);                  /*!< uint8_t "method". */
} switch_manager_t;

extern const switch_manager_t Switch PROGMEM;                         /*!< Switch manager "object". */

#ifdef __cplusplus
  }
#endif

#endif
